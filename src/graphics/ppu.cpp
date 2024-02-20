
#include "ppu.hpp"
#include "cpu/interrupt_manager.hpp"
#include "graphics/lcd_status_register.hpp"
#include "graphics/palette/grayscale_palette.hpp"
#include "spdlog/spdlog.h"
#include "tilemap.hpp"
#include <algorithm>
#include <cassert>

const int PPU::SCREEN_WIDTH = 160;
const int PPU::SCREEN_HEIGHT = 144;

PPU::~PPU() = default;

void PPU::step(int nbrTicks)
{
    _ticksSpentInCurrentMode += nbrTicks;

    bool areLYCAndLYEqual = _lcdStatusRegister->areLYCAndLYEqual();
    _lcdStatusRegister->setLYCompareFlag(areLYCAndLYEqual);

    if (areLYCAndLYEqual && _lcdStatusRegister->isLYCompareStatInterruptEnabled() && !_LYCInterruptRaisedDuringScanline)
    {
        _interruptManager->raiseInterrupt(InterruptType::LCD_STAT);
        _LYCInterruptRaisedDuringScanline = true;
    }

    if (_currentMode == OAM_ACCESS && _ticksSpentInCurrentMode >= OAM_ACCESS_TICKS)
    {
        _spritesToRender.clear();

        if (areSpritesEnabled())
        {
            _spritesToRender = getSpritesThatShouldBeRendered(_currentScanline);
        }

        setMode(VRAM_ACCESS);
        _backgroundWindowFIFO.clear();
        _spritesFIFO.clear();
    }
    else if (_currentMode == VRAM_ACCESS)
    {
        renderPixel();
        if (_ticksSpentInCurrentMode >= VRAM_ACCESS_TICKS)
        {
            if (_lcdStatusRegister->isHBLANKStatInterruptEnabled())
            {
                _interruptManager->raiseInterrupt(InterruptType::LCD_STAT);
            }
            setMode(HBLANK);
            renderScanline(_currentScanline);
        }
    }
    else if (_currentMode == HBLANK && _ticksSpentInCurrentMode >= HBLANK_TICKS)
    {
        _currentScanline++;

        _LYCInterruptRaisedDuringScanline = false;

        if (_currentScanline == SCREEN_HEIGHT)
        {
            if (_lcdStatusRegister->isVBLANKStatInterruptEnabled())
            {
                _interruptManager->raiseInterrupt(InterruptType::LCD_STAT);
            }

            _interruptManager->raiseInterrupt(InterruptType::VBLANK);
            setMode(VBLANK);
            swapFrameBuffers();
        }
        else
        {
            if (_lcdStatusRegister->isOAMStatInterruptEnabled())
            {
                _interruptManager->raiseInterrupt(InterruptType::LCD_STAT);
            }

            setMode(OAM_ACCESS);
        }
    }
    else if (_currentMode == VBLANK && _ticksSpentInCurrentMode >= VBLANK_TICKS)
    {
        _currentScanline++;

        _ticksSpentInCurrentMode = 0;
        if (_currentScanline > MAX_SCANLINE_VALUE)
        {
            if (_lcdStatusRegister->isOAMStatInterruptEnabled())
            {
                _interruptManager->raiseInterrupt(InterruptType::LCD_STAT);
            }

            setMode(OAM_ACCESS);
            _currentScanline = 0;
            _windowLineCounter = 0;
        }
        _LYCInterruptRaisedDuringScanline = false;
    }

    _mmu.write(ADDR_SCANLINE, _currentScanline);
}

std::vector<Sprite*> PPU::getSpritesThatShouldBeRendered(int scanline)
{
    std::vector<Sprite*> spritesToRender = {};
    int nbrSpritesInScanline = 0;

    /*
     * In order to find which sprites to render, we need to check the coordinate of every sprite.
     * There's a maximum number of sprites that can be rendered per scanline.
     */
    for (int i = 0; i < NBR_SPRITES && nbrSpritesInScanline < MAX_NBR_SPRITES_PER_SCANLINE; ++i)
    {
        auto& sprite = _sprites[i];
        int spriteSz = spriteSize();
        int spriteStartVerticalPos = sprite->getYPositionOnScreen();
        int spriteEndVerticalPos = spriteStartVerticalPos + spriteSz;

        if (scanline >= spriteStartVerticalPos && scanline < spriteEndVerticalPos)
        {
            nbrSpritesInScanline++;
            /*
             * If the sprite is outside the screen bound then it's not going to be effectively rendered,
             * but it still counts as if it was rendered and should increment the number of sprites rendered.
             */
            if ((sprite->getXPositionOnScreen() + SingleTile::TILE_WIDTH) < 0 ||
                sprite->getXPositionOnScreen() >= SCREEN_WIDTH)
            {
                continue;
            }

            spritesToRender.push_back(sprite.get());
        }
    }

    if (nbrSpritesInScanline == MAX_NBR_SPRITES_PER_SCANLINE)
    {
        spdlog::debug("Rendering the maximum amount of {} sprites for scanline {}.", nbrSpritesInScanline, scanline);
    }

    return spritesToRender;
}

void PPU::renderScanline(int scanline)
{
    _scanline.clear();
    if (!isDisplayEnabled())
    {
        return;
    }

    if (_mmu.isColorModeSupported() || areBackgroundAndWindowEnabled())
    {
        renderScanlineBackground(scanline);

        if (isWindowEnabled())
        {
            renderScanlineWindow(scanline);
        }
    }

    if (areSpritesEnabled())
    {
        renderScanlineSprite(scanline);
    }

    for (const auto& [key, value] : _scanline)
    {
        _temporaryFrame.setPixel(key, scanline, value.getPalette()->getColorForId(value.getColorId()));
    }
}

void PPU::renderScanlineBackground(int scanline)
{
    byte scrollX = _mmu.read(ADDR_SCROLL_X);
    byte scrollY = _mmu.read(ADDR_SCROLL_Y);

    // Retrieve the tilemap we are going to use
    Tilemap background = getTileMap(backgroundTileMapIndex());

    renderScanlineBackgroundOrWindow(scanline, scrollX, scrollY, background, false);
}

void PPU::renderScanlineWindow(int scanline)
{
    // Window SCX value is shifted by 7 in the register
    byte scrollX = _mmu.read(WINDOW_ADDR_SCROLL_X) - 7;
    byte scrollY = _mmu.read(WINDOW_ADDR_SCROLL_Y);

    // If the window doesn't cover the current line or is outside of the screen we can skip the rendering
    if (scrollY > scanline || scrollX > SCREEN_WIDTH)
    {
        return;
    }

    // Retrieve the tilemap we are going to use
    Tilemap tilemap = getTileMap(windowTileMapIndex());

    renderScanlineBackgroundOrWindow(scanline, scrollX, scrollY, tilemap, true);
}

void PPU::renderScanlineBackgroundOrWindow(int scanline, byte scrollX, byte scrollY, Tilemap& tilemap, bool isWindow)
{
    /*
     * The tilemap is a 32x32 map of tiles of 8x8 pixels.
     * Since we are rendering only a line here, we can already compute which
     * line of the tilemap we are going to render.
     * We can compute the line by seeing how many tiles we span vertically.
     */
    int startLine = isWindow ? _windowLineCounter : (scanline + scrollY);
    int lineInTileMap = startLine / SingleTile::TILE_HEIGHT;
    lineInTileMap %= Tilemap::HEIGHT;
    assert(lineInTileMap < Tilemap::HEIGHT);

    int offsetInTileMap = lineInTileMap * Tilemap::WIDTH;

    /*
     * For each pixel in the line, we are going to retrieve the corresponding tile and copy the
     * pixels that corresponds in the current frame buffer.
     */

    // Background always starts from the top but window can be scrolled
    int startX = isWindow ? scrollX : 0;

    for (int x = startX; x < SCREEN_WIDTH; ++x)
    {
        int xIndexOffset = isWindow ? x - scrollX : scrollX + x;

        if (xIndexOffset >= Tilemap::WIDTH * SingleTile::TILE_WIDTH)
        {
            if (isWindow)
            {
                // The window rendering is clamped, if we go out of bound of the tilemap we stop rendering
                break;
            }
            else
            {
                // The background map is not clamped, if we go "too far right",
                // it should display tiles that are on the left.
                xIndexOffset %= Tilemap::WIDTH * SingleTile::TILE_WIDTH;
            }
        }
        assert(xIndexOffset < Tilemap::WIDTH * SingleTile::TILE_WIDTH);

        // We see how many tiles we span horizontally and add it to our offset to find the tile index
        int tileIndex = offsetInTileMap + (xIndexOffset / SingleTile::TILE_WIDTH);

        /*
         * Now that we retrieve the tile corresponding to the pixel, we need to copy the pixel
         * that corresponds to the one we are rendering.
         * Since scrollX have a 1-pixel resolution it means that we can sometimes render only part of a tile for a line.
         */
        int xOffsetTile = xIndexOffset % SingleTile::TILE_WIDTH;

        int startY = isWindow ? _windowLineCounter : (scanline + scrollY);
        int yOffsetTile = startY % SingleTile::TILE_HEIGHT;

        int bankId = 0;
        Palette* palette = &_paletteBackground;
        int bgPriority = 0;
        if (_mmu.isColorModeSupported())
        {
            Tilemap::TileInfo tileInfo = tilemap.getTileInfoForIndex(tileIndex);
            bankId = tileInfo.getVRAMBankId();
            palette = &_mmu.getColorPaletteMemoryMapperBackground().getColorPalette(tileInfo.getColorPaletteId());

            if (tileInfo.isFlippedHorizontally())
            {
                xOffsetTile = SingleTile::TILE_WIDTH - 1 - xOffsetTile;
            }

            if (tileInfo.isFlippedVertically())
            {
                yOffsetTile = SingleTile::TILE_HEIGHT - 1 - yOffsetTile;
            }

            bgPriority = tileInfo.isRenderedAboveSprites();
        }

        /*
         * We retrieve the pixel color by getting the original sprite color,
         * converting using the palette and converting it to a grayscale or color value.
         */
        byte colorValue = _mmu.getVRAM()
                              .getTileById(tilemap.getTileIdForIndex(tileIndex), backgroundAndWindowTileDataAreaIndex(),
                                           bankId, false)
                              .getColorData(xOffsetTile, yOffsetTile);
        _scanline[x] = Pixel(colorValue, palette, -1, bgPriority, Pixel::Source::BG_WINDOW);
    }

    if (isWindow)
    {
        // We increment the window line counter only when a scanline window is rendered
        _windowLineCounter++;
    }
}

void PPU::renderScanlineSprite(int scanline)
{
    for (auto* sprite : _spritesToRender)
    {
        byte tileId = sprite->getTileId();
        bool isStackedTile = spriteSize() > SingleTile::TILE_HEIGHT;
        if (isStackedTile)
        {
            /*
             * If the sprite is made of two vertically stacked tiles,
             * the id of the tile should be masked to retrieve the id of the top tile.
             */
            tileId &= 0xFE;
        }

        int bankId = _mmu.isColorModeSupported() ? sprite->getBankId() : 0;

        Tile tile = _mmu.getVRAM().getTileById(tileId, 0, bankId, isStackedTile);

        int yCoordinateInTile = scanline - sprite->getYPositionOnScreen();
        if (sprite->isFlippedVertically())
        {
            yCoordinateInTile = tile.getHeight() - 1 - yCoordinateInTile;
        }

        for (int xOffset = 0; xOffset < tile.getWidth(); xOffset++)
        {
            int xCoordinateOnScreen = sprite->getXPositionOnScreen() + xOffset;
            // If we are outside the screen on the left we just skip the current pixel
            if (xCoordinateOnScreen < 0)
            {
                continue;
            }

            // If we are outside the screen on the right we can stop rendering the tile
            if (xCoordinateOnScreen > SCREEN_WIDTH)
            {
                break;
            }

            int xCoordinateInTile = xOffset;
            if (sprite->isFlippedHorizontally())
            {
                xCoordinateInTile = tile.getWidth() - 1 - xCoordinateInTile;
            }

            Palette* palette = sprite->getGrayscalePaletteId() ? &_paletteObj1 : &_paletteObj0;
            bool hasPriority = false;
            int spritePriority = 0;
            if (_mmu.isColorModeSupported())
            {
                spritePriority = sprite->getId();
                palette = &_mmu.getColorPaletteMemoryMapperObj().getColorPalette(sprite->getColorPaletteId());

                // We check if there was another sprite rendered before that has a higher priority
                if (_scanline.count(xCoordinateOnScreen) > 0 &&
                    _scanline[xCoordinateOnScreen].getSource() == Pixel::Source::SPRITE)
                {
                    hasPriority = false;
                }
                // Otherwise, we check if background/window could have priority
                else if (areBackgroundAndWindowDeprioritized())
                {
                    hasPriority = true;
                }
                // in case that background doesn't have priority and sprite have priority, sprite will take over
                else if (_scanline.count(xCoordinateOnScreen) > 0 &&
                         _scanline[xCoordinateOnScreen].getSource() == Pixel::Source::BG_WINDOW &&
                         _scanline[xCoordinateOnScreen].getBackgroundPriority() == 0 &&
                         sprite->isRenderedOverBackgroundAndWindow())
                {
                    hasPriority = true;
                }
            }
            else
            {
                // Smaller coordinates is a higher priority
                spritePriority = -sprite->getXPositionOnScreen();

                // If something was rendered before, we check if it should be overridden
                if (_scanline.count(xCoordinateOnScreen) > 0)
                {
                    Pixel pixel = _scanline[xCoordinateOnScreen];
                    // If we had a sprite with lower priority, we override it
                    if (pixel.getSource() == Pixel::Source::SPRITE && spritePriority > pixel.getSpritePriority())
                    {
                        hasPriority = true;
                    }

                    // If we had a non-white background/window and we can override it then we do
                    else if (pixel.getSource() == Pixel::Source::BG_WINDOW &&
                             (pixel.getColorId() > 0 && sprite->isRenderedOverBackgroundAndWindow()))
                    {
                        hasPriority = true;
                    }
                }
            }

            byte colorId = tile.getColorData(xCoordinateInTile, yCoordinateInTile);

            // We are copying the pixel if it's not white, white is treated as transparent
            bool isPixelOpaque = colorId != 0;

            bool isBackgroundWhite = _scanline.count(xCoordinateOnScreen) == 0 ||
                                     (_scanline.count(xCoordinateOnScreen) > 0 &&
                                      _scanline[xCoordinateOnScreen].getSource() == Pixel::Source::BG_WINDOW &&
                                      (_scanline[xCoordinateOnScreen].getColorId() == 0));

            if (isPixelOpaque && (isBackgroundWhite || hasPriority))
            {
                _scanline[xCoordinateOnScreen] = Pixel(colorId, palette, spritePriority, 0, Pixel::Source::SPRITE);
            }
        }
    }
}

void PPU::swapFrameBuffers()
{
    _lastRenderedFrame = _temporaryFrame;
    _frameId++;
}

PPU::PPU(MMU& mmu_, InterruptManager* interruptManager)
    : _mmu(mmu_), _interruptManager(interruptManager), _lcdStatusRegister(std::make_unique<LCDStatusRegister>(_mmu)),
      _paletteBackground(_mmu, ADDR_PALETTE_BG), _paletteObj0(_mmu, ADDR_PALETTE_OBJ0),
      _paletteObj1(_mmu, ADDR_PALETTE_OBJ1)
{
    reset();

    for (unsigned int i = 0; i < _sprites.size(); ++i)
    {
        _sprites[i] = std::make_unique<Sprite>(_mmu, i);
    }
}

Tilemap PPU::getTileMap(int index)
{
    word tileMapAddr = (index == 0) ? ADDR_MAP_0 : ADDR_MAP_1;
    return Tilemap(&_mmu.getVRAM(), tileMapAddr);
}

bool PPU::isDisplayEnabled() const
{
    return utils::isNthBitSet(_mmu.read(ADDR_LCD_PPU_CONTROL), 7);
}

int PPU::windowTileMapIndex() const
{
    return utils::isNthBitSet(_mmu.read(ADDR_LCD_PPU_CONTROL), 6);
}

bool PPU::isWindowEnabled() const
{
    return utils::isNthBitSet(_mmu.read(ADDR_LCD_PPU_CONTROL), 5);
}

sbyte PPU::backgroundAndWindowTileDataAreaIndex() const
{
    return static_cast<sbyte>(!utils::isNthBitSet(_mmu.read(ADDR_LCD_PPU_CONTROL), 4));
}

int PPU::backgroundTileMapIndex() const
{
    return utils::isNthBitSet(_mmu.read(ADDR_LCD_PPU_CONTROL), 3);
}

int PPU::spriteSize() const
{
    return utils::isNthBitSet(_mmu.read(ADDR_LCD_PPU_CONTROL), 2) ? 16 : 8;
}

bool PPU::areSpritesEnabled() const
{
    return utils::isNthBitSet(_mmu.read(ADDR_LCD_PPU_CONTROL), 1);
}

bool PPU::areBackgroundAndWindowEnabled() const
{
    return utils::isNthBitSet(_mmu.read(ADDR_LCD_PPU_CONTROL), 0);
}

bool PPU::areBackgroundAndWindowDeprioritized() const
{
    return !utils::isNthBitSet(_mmu.read(ADDR_LCD_PPU_CONTROL), 0);
}

void PPU::reset()
{
    _frameId = 0;
    _ticksSpentInCurrentMode = 0;
    setMode(OAM_ACCESS);
    _currentScanline = 0;
    _windowLineCounter = 0;
    _LYCInterruptRaisedDuringScanline = false;
    _temporaryFrame = RGBImage(SCREEN_HEIGHT, SCREEN_WIDTH);
    _lastRenderedFrame = RGBImage(SCREEN_HEIGHT, SCREEN_WIDTH);
}

void PPU::setMode(PPU::Mode value)
{
    _lcdStatusRegister->updateFlagMode(value);
    _currentMode = value;
    // TODO: Check if we should take into account modulo of ticks
    _ticksSpentInCurrentMode = 0;
}

void PPU::renderPixel()
{
    if (_backgroundWindowFIFO.isEmpty())
    {
        // TODO: Fill FIFO
    }

    if (_spritesFIFO.isEmpty())
    {
    }
}
