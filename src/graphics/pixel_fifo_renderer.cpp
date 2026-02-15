#include "pixel_fifo_renderer.hpp"
#include "ppu.hpp"

PixelFifoRenderer::PixelFifoRenderer(MMU* mmu, PPU* ppu)
    : _mmu(mmu), _ppu(ppu), _bgWindowPixelFetcher(&mmu->getVRAM(), ppu, _backgroundWindowFIFO),
      _spritePixelFetcher(&mmu->getVRAM(), ppu, _oamFIFO)
{
}

void PixelFifoRenderer::step()
{
    // If sprite fetcher is active, step it
    if (_bgFetcherPaused)
    {
        bool spriteFetchComplete = _spritePixelFetcher.step();
        if (spriteFetchComplete)
        {
            // Check if there are more sprites at the same X position
            Sprite* nextSprite = checkForSpriteHit();
            if (nextSprite != nullptr)
            {
                _spritePixelFetcher.startFetching(nextSprite, _ppu->getCurrentScanline());
                _fetchedSpriteIds.insert(nextSprite->getId());
            }
            else
            {
                // No more sprites at this X, resume background fetcher
                _bgFetcherPaused = false;
            }
        }
        return;
    }

    // Check for window trigger BEFORE stepping the fetcher
    // The window can be triggered when X >= WX - 7
    bool windowShouldBeActive = checkForWindowTrigger();

    if (!_windowActive && windowShouldBeActive)
    {
        // Window just became active - start window rendering
        startWindowRendering();
    }
    else if (_windowActive && !windowShouldBeActive)
    {
        // Window was active but condition is no longer met (WX moved off-screen or window disabled)
        // Switch back to background mode, but keep the window line counter
        _windowActive = false;
        _backgroundWindowFIFO.clear();
        _bgWindowPixelFetcher.reset();
        _bgWindowPixelFetcher.setMode(BackgroundWindowPixelFetcher::Mode::BACKGROUND);
    }

    // Step the background/window fetcher
    _bgWindowPixelFetcher.step();

    // Pop and render pixels only when we have enough and sprite fetcher is not active
    if (_backgroundWindowFIFO.size() >= 8)
    {
        // Discard first SCX % 8 pixels at the start of each scanline
        // Per Pan Docs: "At the start of each scanline, pixels from the first fetched
        // BG tile equal to SCX mod 8 are discarded"
        if (_pixelsToDiscard > 0)
        {
            _backgroundWindowFIFO.pop();
            if (!_oamFIFO.isEmpty())
            {
                _oamFIFO.pop();
            }
            _pixelsToDiscard--;
            return;
        }

        // Check if we need to start fetching a sprite BEFORE popping
        // This ensures the FIFO has pixels for sprite mixing
        if (_ppu->areSpritesEnabled())
        {
            Sprite* spriteHit = checkForSpriteHit();
            if (spriteHit != nullptr)
            {
                // Pause background fetcher and start sprite fetcher
                _bgFetcherPaused = true;
                _spritePixelFetcher.startFetching(spriteHit, _ppu->getCurrentScanline());
                _fetchedSpriteIds.insert(spriteHit->getId());
                return; // Don't pop pixel yet, sprite fetch will happen first
            }
        }

        // Check for window trigger again after FIFO has pixels
        // Window can be enabled mid-scanline by changing WX
        if (!_windowActive && checkForWindowTrigger())
        {
            startWindowRendering();
            return; // Don't pop yet, need to refetch for window
        }

        // Mix the BG and OAM FIFOs and get the pixel to render
        Pixel pixel = mixPixels();

        // Get the appropriate palette based on pixel source
        Palette* palette = nullptr;
        if (pixel.getSource() == Pixel::Source::SPRITE)
        {
            if (_mmu->isColorModeSupported())
            {
                palette = &_mmu->getColorPaletteMemoryMapperObj().getColorPalette(pixel.getPaletteId());
            }
            else
            {
                palette = _ppu->getPaletteObj(pixel.getPaletteId());
            }
        }
        else
        {
            if (_mmu->isColorModeSupported())
            {
                palette = &_mmu->getColorPaletteMemoryMapperBackground().getColorPalette(pixel.getPaletteId());
            }
            else
            {
                palette = _ppu->getPaletteBackground();
            }
        }

        byte colorId = pixel.getColorId();
        // On DMG, if BG/Window is disabled, use color 0 for background
        // On CGB, LCDC.0 has a different meaning (BG/Window priority)
        if (!_mmu->isColorModeSupported() && !_ppu->areBackgroundAndWindowEnabled() &&
            pixel.getSource() == Pixel::Source::BG_WINDOW)
        {
            colorId = 0;
        }

        _ppu->getTemporaryFrame().setPixel(_x, _ppu->getCurrentScanline(), palette->getColorForId(colorId));
        _x++;
    }
}

Pixel PixelFifoRenderer::mixPixels()
{
    // Pop from both FIFOs
    Pixel bgPixel = _backgroundWindowFIFO.pop();

    // If OAM FIFO is empty or sprites disabled, return BG pixel
    if (_oamFIFO.isEmpty() || !_ppu->areSpritesEnabled())
    {
        return bgPixel;
    }

    Pixel oamPixel = _oamFIFO.pop();

    // If sprite pixel is transparent (color 0), always use BG pixel
    if (oamPixel.getColorId() == 0)
    {
        return bgPixel;
    }

    // CRITICAL: If BG pixel is color 0, sprite ALWAYS shows through
    // This is true regardless of any priority flags (OBJ-to-BG or BG-to-OAM)
    // BG color 0 is effectively "transparent" to sprites
    if (bgPixel.getColorId() == 0)
    {
        return oamPixel;
    }

    // At this point, both sprite and BG have non-zero (opaque) colors
    // Now we apply priority rules to determine which one wins
    bool useOamPixel = true;

    if (_mmu->isColorModeSupported())
    {
        // CGB priority rules (when LCDC.0 is 1 - BG/Window have priority enabled):
        // 1. If LCDC.0 is 0, sprites always have priority (already handled above via color 0 transparency)
        // 2. If BG tile has BG-to-OAM Priority attribute set (bit 7 in tilemap attributes), BG wins
        // 3. If sprite has OBJ-to-BG Priority bit set (bit 7 in OAM), BG wins
        //
        // Note: LCDC.0 on CGB doesn't disable BG, it disables BG/Window priority
        if (!_ppu->areBackgroundAndWindowDeprioritized())
        {
            // Check BG tile's BG-to-OAM priority attribute (stored in bgPixel's priority field)
            if (bgPixel.getPriority() != 0)
            {
                useOamPixel = false;
            }

            // Check sprite's OBJ-to-BG Priority bit
            // This is encoded as negative priority value in our implementation
            if (oamPixel.getPriority() < 0)
            {
                useOamPixel = false;
            }
        }
        // If LCDC.0 is 0 (areBackgroundAndWindowDeprioritized returns true),
        // sprites always have priority over BG (when both are non-transparent)
    }
    else
    {
        // DMG priority rules:
        // If sprite's OBJ-to-BG Priority bit is set, BG wins (sprite appears behind BG)
        // We use negative priority to indicate OBJ-to-BG Priority flag is set
        if (oamPixel.getPriority() < 0)
        {
            useOamPixel = false;
        }
    }

    return useOamPixel ? oamPixel : bgPixel;
}

int PixelFifoRenderer::getX() const
{
    return _x;
}

void PixelFifoRenderer::reset()
{
    _backgroundWindowFIFO.clear();
    _oamFIFO.clear();
    _x = 0;
    _bgWindowPixelFetcher.reset();
    _bgWindowPixelFetcher.setMode(BackgroundWindowPixelFetcher::Mode::BACKGROUND);
    _spritePixelFetcher.reset();
    _spritesToRender.clear();
    _fetchedSpriteIds.clear();
    _bgFetcherPaused = false;
    _windowActive = false;
    _windowTriggeredThisScanline = false;
    // Per Pan Docs: "At the start of each scanline, pixels from the first fetched
    // BG tile equal to SCX mod 8 are discarded"
    _pixelsToDiscard = _ppu->getScrollX() % 8;
}

void PixelFifoRenderer::setSpritesToRender(const std::vector<Sprite*>& sprites)
{
    _spritesToRender = sprites;
    _fetchedSpriteIds.clear();
}

PixelFIFO& PixelFifoRenderer::getBackgroundWindowFIFO()
{
    return _backgroundWindowFIFO;
}

PixelFIFO& PixelFifoRenderer::getOAMFIFO()
{
    return _oamFIFO;
}

bool PixelFifoRenderer::wasWindowTriggeredThisScanline() const
{
    return _windowTriggeredThisScanline;
}

Sprite* PixelFifoRenderer::checkForSpriteHit()
{
    // Check if any sprite should be fetched at the current X position
    // A sprite hit occurs when the current pixel X position equals the sprite's X position
    // (or X=0 for sprites that start off-screen to the left)
    for (Sprite* sprite : _spritesToRender)
    {
        // Skip sprites that have already been fetched
        if (_fetchedSpriteIds.count(sprite->getId()) > 0)
        {
            continue;
        }

        int spriteX = sprite->getXPositionOnScreen();

        // Calculate the X position where this sprite should be triggered
        // For sprites with X < 0 (partially off-screen left), trigger at X=0
        int triggerX = (spriteX < 0) ? 0 : spriteX;

        // Trigger fetch when we reach the sprite's starting position (or 0 for off-screen)
        if (_x == triggerX)
        {
            return sprite;
        }
    }

    return nullptr;
}

bool PixelFifoRenderer::checkForWindowTrigger()
{
    // Window should be active if ALL of these conditions are met:
    // 1. Window is enabled (LCDC bit 5)
    // 2. Current scanline >= WY (window Y position)
    // 3. Current X position >= WX - 7 (window X position, WX=7 means window starts at X=0)
    // 4. WX - 7 is on-screen (< 160)

    if (!_ppu->isWindowEnabled())
    {
        return false;
    }

    int scanline = _ppu->getCurrentScanline();
    int wy = _ppu->getWindowScrollY();
    int wx = _ppu->getWindowScrollX();

    // Check WY condition: scanline must be >= WY
    if (scanline < wy)
    {
        return false;
    }

    // Window X trigger position (WX=7 means window at screen X=0)
    int windowTriggerX = wx - 7;

    // Window must start at a valid on-screen position (0 to 159)
    // If windowTriggerX >= SCREEN_WIDTH, window doesn't trigger this scanline
    if (windowTriggerX >= PPU::SCREEN_WIDTH)
    {
        return false;
    }

    // Clamp negative trigger position to 0
    if (windowTriggerX < 0)
    {
        windowTriggerX = 0;
    }

    // Check if we've reached (or passed) the window's X position
    if (_x >= windowTriggerX)
    {
        return true;
    }

    return false;
}

void PixelFifoRenderer::startWindowRendering()
{
    // Per Pan Docs: "When rendering the window the background FIFO is cleared
    // and the fetcher is reset to step 1"

    // Clear the background FIFO
    _backgroundWindowFIFO.clear();

    // Reset and switch the fetcher to window mode
    _bgWindowPixelFetcher.reset();
    _bgWindowPixelFetcher.setMode(BackgroundWindowPixelFetcher::Mode::WINDOW);

    _windowActive = true;

    // Only increment window line counter the first time window is triggered on this scanline
    // Per ACID test: window can be shown, hidden, then shown again - the line counter
    // should continue from where it left off
    if (!_windowTriggeredThisScanline)
    {
        _windowTriggeredThisScanline = true;
    }
}
