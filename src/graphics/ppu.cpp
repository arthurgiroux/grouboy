
#include "ppu.hpp"
#include "cpu/interrupt_manager.hpp"
#include "spdlog/spdlog.h"
#include <cassert>
#include <set>

void PPU::step(int nbrTicks)
{
    _ticksSpentInCurrentMode += nbrTicks;

    LCDStatusRegister lcdStatusRegister(_mmu);

    bool areLYCAndLYEqual = lcdStatusRegister.areLYCAndLYEqual();
    lcdStatusRegister.setLYCompareFlag(areLYCAndLYEqual);

    if (areLYCAndLYEqual && lcdStatusRegister.isLYCompareStatInterruptEnabled() && !_LYCInterruptRaisedDuringScanline)
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
        if (lcdStatusRegister.isHBLANKStatInterruptEnabled())
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
            if (lcdStatusRegister.isVBLANKStatInterruptEnabled())
            {
                _interruptManager->raiseInterrupt(InterruptType::LCD_STAT);
            }

            _interruptManager->raiseInterrupt(InterruptType::VBLANK);
            setMode(VBLANK);
            swapFrameBuffers();
        }
        else
        {
            if (lcdStatusRegister.isOAMStatInterruptEnabled())
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
            if (lcdStatusRegister.isOAMStatInterruptEnabled())
            {
                _interruptManager->raiseInterrupt(InterruptType::LCD_STAT);
            }

            setMode(OAM_ACCESS);
            _currentScanline = 0;
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
    TileMap background = getTileMap(backgroundTileMapIndex(), backgroundAndWindowTileDataAreaIndex());

    /*
     * The tilemap is a 32x32 map of tiles of 8x8 pixels.
     * Since we are rendering only a line here, we can already compute which
     * line of the tilemap we are going to render.
     * We can compute the line by seeing how many tiles we span vertically.
     */
    int lineInTileMap = (scanline + scrollY) / Tile::TILE_HEIGHT;
    lineInTileMap %= TILEMAP_HEIGHT;
    assert(lineInTileMap < TILEMAP_HEIGHT);

    int offsetInTileMap = lineInTileMap * TILEMAP_WIDTH;

    /*
     * For each pixel in the line, we are going to retrieve the corresponding tile and copy the
     * pixels that corresponds in the current frame buffer.
     */
    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
        int xIndexOffset = scrollX + x;

        // The background map is not clamped, if we go "too far right",
        // it should display tiles that are on the left.
        if (xIndexOffset > TILEMAP_WIDTH * Tile::TILE_WIDTH)
        {
            xIndexOffset %= TILEMAP_WIDTH * Tile::TILE_WIDTH;
        }
        assert(xIndexOffset < TILEMAP_WIDTH * Tile::TILE_WIDTH);

        // We see how many tiles we span horizontally and add it to our offset to find the tile index
        int tileIndex = offsetInTileMap + (xIndexOffset / Tile::TILE_WIDTH);
        const RGBImage& tileImage = background[tileIndex].getImage();

        /*
         * Now that we retrieve the tile corresponding to the pixel, we need to copy the pixel
         * that corresponds to the one we are rendering.
         * Since scrollX have a 1-pixel resolution it means that we can sometimes render only part of a tile for a line.
         */
        int xOffsetTile = xIndexOffset % Tile::TILE_WIDTH;
        int yOffsetTile = (scanline + scrollY) % Tile::TILE_HEIGHT;

        // Finally we copy the correct pixel in our temporary frame.
        _temporaryFrame.copyPixel(x, scanline, tileImage, xOffsetTile, yOffsetTile);
    }
}

void PPU::renderScanlineWindow(int scanline)
{
    // TODO: render window
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
            if (sprite->getXPositionOnScreen() < 0 || sprite->getXPositionOnScreen() >= PPU::SCREEN_WIDTH)
            {
                continue;
            }

            spritesToRender.push_back(sprite.get());
        }
    }

    if (nbrSpritesInScanline == MAX_NBR_SPRITES_PER_SCANLINE)
    {
        spdlog::info("Rendering the maximum amount of {} sprites for scanline {}.", nbrSpritesInScanline, scanline);
    }

    // Sort sprite by priority
    std::sort(spritesToRender.begin(), spritesToRender.end(),
              [](Sprite* l, Sprite* r) { return l->isPriorityBiggerThanOtherSprite(*r); });

    // Keeping track of which pixel were rendered
    std::set<int> renderedPixels = {};
    for (auto* sprite : spritesToRender)
    {
        Tile tile = getTileById(sprite->getTileId(), 0);
        auto tileImage = tile.getImage();

        int yCoordinateInTile = scanline - sprite->getYPositionOnScreen();
        if (sprite->isFlippedVertically())
        {
            yCoordinateInTile = Tile::TILE_HEIGHT - 1 - yCoordinateInTile;
        }

        for (int xOffset = 0; xOffset < Tile::TILE_WIDTH; xOffset++)
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

            // If the pixel was already rendered by another sprite with higher priority, we are not rendering it again.
            if (renderedPixels.count(xCoordinateOnScreen))
            {
                continue;
            }

            int xCoordinateInTile = xOffset;
            if (sprite->isFlippedHorizontally())
            {
                xCoordinateInTile = Tile::TILE_WIDTH - 1 - xCoordinateInTile;
            }

            // We are copying the pixel if it's not white, white is treated as transparent
            bool isPixelOpaque = !tileImage.isPixelWhite(xCoordinateInTile, yCoordinateInTile);

            /*
             * The pixel should only be rendered if it's over background/window or if
             * the background/window didn't render to this pixel.
             */
            bool pixelShouldBeRendered = sprite->isRenderedOverBackgroundAndWindow() ||
                                         _temporaryFrame.isPixelWhite(xCoordinateOnScreen, scanline);

            if (isPixelOpaque && pixelShouldBeRendered)
            {
                _temporaryFrame.copyPixel(xCoordinateOnScreen, scanline, tileImage, xCoordinateInTile,
                                          yCoordinateInTile);
            }
        }
    }
}

void PPU::swapFrameBuffers()
{
    _lastRenderedFrame = _temporaryFrame;
    _frameId++;
}

PPU::PPU(MMU& mmu_, InterruptManager* interruptManager) : _mmu(mmu_), _interruptManager(interruptManager)
{
    reset();

    for (int i = 0; i < _sprites.size(); ++i)
    {
        _sprites[i] = std::make_unique<Sprite>(_mmu, i);
    }
}

Tile PPU::getTileById(byte tileId, int8_t tileSetId)
{
    int tileSetOffset = ADDR_TILE_SET_0;
    int tileIdCorrected = tileId;
    if (tileSetId == 1)
    {
        tileSetOffset = ADDR_TILE_SET_1;
        tileIdCorrected = static_cast<sbyte>(tileId);
    }

    word tileBaseAddr = tileSetOffset + Tile::BYTES_PER_TILE * tileIdCorrected;
    Tile::TileDataArray dataArray = {};
    for (int i = 0; i < Tile::BYTES_PER_TILE; ++i)
    {
        dataArray[i] = _mmu.read(tileBaseAddr + i);
    }

    return Tile(dataArray);
}

PPU::TileMap PPU::getTileMap(int index, int tileSetId)
{
    int tileMapAddr = (index == 0) ? ADDR_MAP_0 : ADDR_MAP_1;
    TileMap map = {};

    for (int i = 0; i < TILEMAP_WIDTH * TILEMAP_HEIGHT; ++i)
    {
        sbyte tileId = _mmu.read(tileMapAddr + i);
        map.push_back(getTileById(tileId, tileSetId));
    }

    return map;
}

bool PPU::isDisplayEnabled() const
{
    return utils::isNthBitSet(_mmu.read(ADDR_LCD_PPU_CONTROL), 7);
}

int PPU::tileMapIndexForWindow() const
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
    _LYCInterruptRaisedDuringScanline = false;
    _temporaryFrame = RGBImage(SCREEN_HEIGHT, SCREEN_WIDTH);
    _lastRenderedFrame = RGBImage(SCREEN_HEIGHT, SCREEN_WIDTH);
}