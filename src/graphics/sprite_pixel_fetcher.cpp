#include "sprite_pixel_fetcher.hpp"
#include "memory/vram.hpp"
#include "ppu.hpp"
#include "tile.hpp"

#include <climits>

namespace
{
// Priority value used for transparent sprite pixels in the OAM FIFO
constexpr int TRANSPARENT_SPRITE_PRIORITY = INT_MAX;
} // namespace

SpritePixelFetcher::SpritePixelFetcher(VRAM* vram, PPU* ppu, PixelFIFO& oamFifo)
    : _ppu(ppu), _vram(vram), _oamFifo(oamFifo)
{
}

void SpritePixelFetcher::startFetching(Sprite* sprite, int scanline)
{
    _sprite = sprite;
    _scanline = scanline;
    _active = true;
    _currentStep = Step::GetTileId;
    _ticksInCurrentStep = 0;
}

bool SpritePixelFetcher::step()
{
    if (!_active)
    {
        return true;
    }

    switch (_currentStep)
    {
    case Step::GetTileId:
        stepGetTileId();
        break;
    case Step::GetTileDataLow:
        stepGetTileDataLow();
        break;
    case Step::GetTileDataHigh:
        stepGetTileDataHigh();
        break;
    case Step::Sleep:
        stepSleep();
        break;
    case Step::Push:
        pushToFifo();
        return true;
    }

    return false;
}

bool SpritePixelFetcher::isActive() const
{
    return _active;
}

void SpritePixelFetcher::reset()
{
    _active = false;
    _currentStep = Step::GetTileId;
    _ticksInCurrentStep = 0;
    _sprite = nullptr;
}

void SpritePixelFetcher::stepGetTileId()
{
    if (_ticksInCurrentStep == 1)
    {
        _tileId = _sprite->getTileId();
        int spriteHeight = _ppu->spriteSize();

        // For 8x16 sprites, mask the tile ID to get the top tile
        // The bottom tile is tileId | 0x01
        if (spriteHeight == 16)
        {
            _tileId &= 0xFE;
        }

        // Calculate which line of the tile we need
        _tileLine = _scanline - _sprite->getYPositionOnScreen();

        // Handle vertical flip
        if (_sprite->isFlippedVertically())
        {
            _tileLine = (spriteHeight - 1) - _tileLine;
        }

        // For 8x16 sprites, if we're in the bottom half, use the second tile
        if (spriteHeight == 16 && _tileLine >= 8)
        {
            _tileId |= 0x01;
            _tileLine -= 8;
        }

        // Get tile address - sprites always use tile set 0 (address 0x8000)
        _tileAddr = _tileId * SingleTile::BYTES_PER_TILE;

        // Get bank ID for CGB mode
        _bankId = _ppu->getMMU().isColorModeSupported() ? _sprite->getBankId() : 0;

        goToStep(Step::GetTileDataLow);
    }
    else
    {
        _ticksInCurrentStep++;
    }
}

void SpritePixelFetcher::stepGetTileDataLow()
{
    if (_ticksInCurrentStep == 1)
    {
        _dataLow = _vram->readFromBank(static_cast<word>(_tileAddr + _tileLine * 2), _bankId);
        goToStep(Step::GetTileDataHigh);
    }
    else
    {
        _ticksInCurrentStep++;
    }
}

void SpritePixelFetcher::stepGetTileDataHigh()
{
    if (_ticksInCurrentStep == 1)
    {
        _dataHigh = _vram->readFromBank(static_cast<word>(_tileAddr + _tileLine * 2 + 1), _bankId);
        goToStep(Step::Sleep);
    }
    else
    {
        _ticksInCurrentStep++;
    }
}

void SpritePixelFetcher::stepSleep()
{
    // Sleep step - do nothing for 2 ticks
    if (_ticksInCurrentStep == 1)
    {
        goToStep(Step::Push);
    }
    else
    {
        _ticksInCurrentStep++;
    }
}

void SpritePixelFetcher::pushToFifo()
{
    /*
     * Per Pan Docs:
     * "Before any mixing is done, if the OAM FIFO doesn't have at least 8 pixels in it
     * then transparent pixels with the lowest priority are pushed onto the OAM FIFO."
     */
    while (_oamFifo.size() < 8)
    {
        // Push transparent pixel (color 0) with lowest priority
        _oamFifo.push(Pixel(0, 0, Pixel::Source::SPRITE, TRANSPARENT_SPRITE_PRIORITY));
    }

    bool isFlippedHorizontally = _sprite->isFlippedHorizontally();
    bool hasBackgroundPriority = !_sprite->isRenderedOverBackgroundAndWindow();

    // Get palette information
    int paletteId;
    if (_ppu->getMMU().isColorModeSupported())
    {
        paletteId = _sprite->getColorPaletteId();
    }
    else
    {
        paletteId = _sprite->getGrayscalePaletteId();
    }

    // Calculate sprite priority for mixing
    // We need a combined priority that takes into account both X position (for DMG) or OAM index (for CGB)
    // and the BG priority flag. We use a combined value to ensure proper sorting.
    int spritePriority;
    if (_ppu->getMMU().isColorModeSupported())
    {
        // CGB: OAM index is the priority (lower index = higher priority)
        spritePriority = _sprite->getId();
    }
    else
    {
        // DMG: X coordinate is primary priority, OAM index is secondary (tiebreaker)
        // We combine them: X * 256 + OAM_index
        // This ensures X takes precedence, but if X is equal, lower OAM index wins
        // Since there are max 40 sprites (0-39), this won't overflow
        int xPriority = _sprite->getXPositionOnScreen() + 8; // Add 8 to make X always positive (range -8 to 167 -> 0 to 175)
        spritePriority = xPriority * 256 + _sprite->getId();
    }

    // If sprite has BG priority flag, encode it as negative priority
    // This is used during mixing to determine if BG should take precedence
    if (hasBackgroundPriority)
    {
        spritePriority = -spritePriority - 1; // Make negative, ensure it's at least -1
    }

    // Calculate which pixels of the sprite are visible based on X position
    int spriteXOnScreen = _sprite->getXPositionOnScreen();
    int startPixel = (spriteXOnScreen < 0) ? -spriteXOnScreen : 0;
    int rendererX = _ppu->getPixelFifoRenderer().getX();

    // Process each pixel of the sprite tile row
    for (int pixelX = startPixel; pixelX < 8; ++pixelX)
    {
        int fifoIndex = (spriteXOnScreen + pixelX) - rendererX;

        // Skip if outside FIFO bounds
        if (fifoIndex < 0 || fifoIndex >= static_cast<int>(_oamFifo.size()))
        {
            continue;
        }

        // Calculate which bit to read based on horizontal flip
        int bitPosition = isFlippedHorizontally ? pixelX : (7 - pixelX);
        byte colorId = ((_dataHigh >> bitPosition) & 0x01) << 1 | ((_dataLow >> bitPosition) & 0x01);

        // Get the current pixel in the OAM FIFO
        Pixel& existingPixel = _oamFifo.at(fifoIndex);

        /*
         * Per Pan Docs:
         * "If the target object pixel is not white and the pixel in the OAM FIFO is white,
         * or if the pixel in the OAM FIFO has higher priority than the target object's pixel,
         * then the pixel in the OAM FIFO is replaced with the target object's properties."
         *
         * Note: "white" means transparent (color 0) for sprites
         * Higher priority number = lower priority sprite (will be replaced)
         */
        bool shouldReplace = false;

        if (colorId != 0)
        {
            // New sprite pixel is not transparent
            if (existingPixel.getColorId() == 0)
            {
                // Existing pixel is transparent, always replace
                shouldReplace = true;
            }
            else if (_ppu->getMMU().isColorModeSupported())
            {
                // CGB: Compare OAM indices (lower index = higher priority)
                // Note: We use abs() because negative values indicate BG priority flag
                if (abs(spritePriority) < abs(existingPixel.getPriority()))
                {
                    shouldReplace = true;
                }
            }
            else
            {
                // DMG: Compare X coordinates (lower X = higher priority)
                // Sprites are already sorted, so earlier sprites have priority
                // The existing pixel should win if it's from a higher priority sprite
                if (abs(spritePriority) < abs(existingPixel.getPriority()))
                {
                    shouldReplace = true;
                }
            }
        }

        if (shouldReplace)
        {
            existingPixel = Pixel(colorId, paletteId, Pixel::Source::SPRITE, spritePriority);
        }
    }

    _active = false;
}

void SpritePixelFetcher::goToStep(SpritePixelFetcher::Step step)
{
    _currentStep = step;
    _ticksInCurrentStep = 0;
}
