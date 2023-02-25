
#include "ppu.hpp"
#include "cpu/interrupt_manager.hpp"
#include "graphics/lcd_status_register.hpp"
#include "graphics/palette.hpp"
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
        setMode(VRAM_ACCESS);
    }
    else if (_currentMode == VRAM_ACCESS && _ticksSpentInCurrentMode >= VRAM_ACCESS_TICKS)
    {
        if (_lcdStatusRegister->isHBLANKStatInterruptEnabled())
        {
            _interruptManager->raiseInterrupt(InterruptType::LCD_STAT);
        }
        setMode(HBLANK);
        renderScanline(_currentScanline);
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

void PPU::renderScanline(int scanline)
{
    if (!isDisplayEnabled())
    {
        return;
    }

    if (areBackgroundAndWindowEnabled())
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
}

void PPU::renderScanlineBackground(int scanline)
{
    byte scrollX = _mmu.read(ADDR_SCROLL_X);
    byte scrollY = _mmu.read(ADDR_SCROLL_Y);

    // Retrieve the tilemap we are going to use
    Tilemap background = getTileMap(backgroundTileMapIndex());

    /*
     * The tilemap is a 32x32 map of tiles of 8x8 pixels.
     * Since we are rendering only a line here, we can already compute which
     * line of the tilemap we are going to render.
     * We can compute the line by seeing how many tiles we span vertically.
     */
    int lineInTileMap = (scanline + scrollY) / SingleTile::TILE_HEIGHT;
    lineInTileMap %= Tilemap::HEIGHT;
    assert(lineInTileMap < Tilemap::HEIGHT);

    int offsetInTileMap = lineInTileMap * Tilemap::WIDTH;

    /*
     * For each pixel in the line, we are going to retrieve the corresponding tile and copy the
     * pixels that corresponds in the current frame buffer.
     */
    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
        int xIndexOffset = scrollX + x;

        // The background map is not clamped, if we go "too far right",
        // it should display tiles that are on the left.
        if (xIndexOffset >= Tilemap::WIDTH * SingleTile::TILE_WIDTH)
        {
            xIndexOffset %= Tilemap::WIDTH * SingleTile::TILE_WIDTH;
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
        int yOffsetTile = (scanline + scrollY) % SingleTile::TILE_HEIGHT;

        /*
         * We retrieve the pixel color by getting the original sprite color,
         * converting using the palette and converting it to a grayscale value.
         */
        int colorValue = getTileById(background.getTileIdForIndex(tileIndex), backgroundAndWindowTileDataAreaIndex())
                             .getColorData(xOffsetTile, yOffsetTile);
        int convertedPaletteColor = _paletteBackground.convertColorId(colorValue);
        _temporaryFrame.setPixel(x, scanline, Palette::convertColorToGrayscale(convertedPaletteColor));
    }
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

    /*
     * The tilemap is a 32x32 map of tiles of 8x8 pixels.
     * Since we are rendering only a line here, we can already compute which
     * line of the tilemap we are going to render.
     * We can compute the line by seeing how many tiles we span vertically.
     */
    int lineInTileMap = _windowLineCounter / SingleTile::TILE_HEIGHT;
    lineInTileMap %= Tilemap::HEIGHT;
    assert(lineInTileMap < Tilemap::HEIGHT);

    int offsetInTileMap = lineInTileMap * Tilemap::WIDTH;

    /*
     * For each pixel in the line, we are going to retrieve the corresponding tile and copy the
     * pixels that corresponds in the current frame buffer.
     */
    for (int x = scrollX; x < SCREEN_WIDTH; ++x)
    {
        int xIndexOffset = x - scrollX;

        // The window rendering is clamped, if we go out of bound of the tilemap we stop rendering
        if (xIndexOffset >= Tilemap::WIDTH * SingleTile::TILE_WIDTH)
        {
            break;
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
        int yOffsetTile = _windowLineCounter % SingleTile::TILE_HEIGHT;

        /*
         * We retrieve the pixel color by getting the original sprite color,
         * converting using the palette and converting it to a grayscale value.
         */
        int colorValue = getTileById(tilemap.getTileIdForIndex(tileIndex), backgroundAndWindowTileDataAreaIndex())
                             .getColorData(xOffsetTile, yOffsetTile);
        // int colorValue = tilemap[tileIndex].getColorData(xOffsetTile, yOffsetTile);
        int convertedPaletteColor = _paletteBackground.convertColorId(colorValue);
        _temporaryFrame.setPixel(x, scanline, Palette::convertColorToGrayscale(convertedPaletteColor));
    }

    // We increment the window line counter only when a scanline window is rendered
    _windowLineCounter++;
}

void PPU::renderScanlineSprite(int scanline)
{
    int nbrSpritesInScanline = 0;

    std::vector<Sprite*> spritesToRender = {};
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
             * If the sprite is outside the screen bound then it's not going to be effectively rendered
             * but it still counts as if it was rendered and should increment the number of sprites rendered.
             */
            if ((sprite->getXPositionOnScreen() + SingleTile::TILE_WIDTH) < 0 ||
                sprite->getXPositionOnScreen() >= PPU::SCREEN_WIDTH)
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

    /*
     * Sort sprite by priority, from lowest to highest.
     * We will render first the lowest priority sprite so that they can be overridden by a sprite with higher priority.
     */
    std::sort(spritesToRender.begin(), spritesToRender.end(),
              [](Sprite* l, Sprite* r) { return !l->isPriorityBiggerThanOtherSprite(*r); });

    for (auto* sprite : spritesToRender)
    {
        int tileId = sprite->getTileId();
        bool isStackedTile = spriteSize() > SingleTile::TILE_HEIGHT;
        if (isStackedTile)
        {
            /*
             * If the sprite is made of two vertically stacked tiles,
             * the id of the tile should be masked to retrieve the id of the top tile.
             */
            tileId &= 0xFE;
        }

        Tile tile = getTileById(tileId, 0, isStackedTile);

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

            Palette& palette = sprite->getPaletteId() ? _paletteObj1 : _paletteObj0;
            int colorId = tile.getColorData(xCoordinateInTile, yCoordinateInTile);
            int convertedPaletteColor = palette.convertColorId(colorId);

            // We are copying the pixel if it's not white, white is treated as transparent
            bool isPixelOpaque = colorId != 0;

            /*
             * The pixel should only be rendered if it's over background/window or if
             * the background/window didn't render to this pixel.
             */
            bool pixelShouldBeRendered = sprite->isRenderedOverBackgroundAndWindow() ||
                                         _temporaryFrame.isPixelWhite(xCoordinateOnScreen, scanline);

            if (isPixelOpaque && pixelShouldBeRendered)
            {
                _temporaryFrame.setPixel(xCoordinateOnScreen, scanline,
                                         Palette::convertColorToGrayscale(convertedPaletteColor));
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
    : _mmu(mmu_), _interruptManager(interruptManager), _paletteBackground(_mmu, ADDR_PALETTE_BG),
      _paletteObj0(_mmu, ADDR_PALETTE_OBJ0), _paletteObj1(_mmu, ADDR_PALETTE_OBJ1),
      _lcdStatusRegister(std::make_unique<LCDStatusRegister>(_mmu))
{
    reset();

    for (int i = 0; i < _sprites.size(); ++i)
    {
        _sprites[i] = std::make_unique<Sprite>(_mmu, i);
    }
}

Tile PPU::getTileById(byte tileId, int8_t tileSetId, bool isStacked)
{
    int tileSetOffset = ADDR_TILE_SET_0;
    int tileIdCorrected = tileId;
    if (tileSetId == 1)
    {
        tileSetOffset = ADDR_TILE_SET_1;
        tileIdCorrected = static_cast<sbyte>(tileId);
    }

    word tileBaseAddr = tileSetOffset + SingleTile::BYTES_PER_TILE * tileIdCorrected;

    int tileBytesPerTile = isStacked ? StackedTile::BYTES_PER_TILE : SingleTile::BYTES_PER_TILE;
    Tile::TileDataArray dataArray = {};
    dataArray.resize(tileBytesPerTile);

    for (int i = 0; i < tileBytesPerTile; ++i)
    {
        dataArray[i] = _mmu.read(tileBaseAddr + i);
    }

    if (isStacked)
    {
        return StackedTile(dataArray);
    }
    else
    {
        return SingleTile(dataArray);
    }
}

Tilemap PPU::getTileMap(int index)
{
    int tileMapAddr = (index == 0) ? ADDR_MAP_0 : ADDR_MAP_1;
    return Tilemap(&_mmu, tileMapAddr);
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

int PPU::backgroundAndWindowTileDataAreaIndex() const
{
    return !utils::isNthBitSet(_mmu.read(ADDR_LCD_PPU_CONTROL), 4);
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
