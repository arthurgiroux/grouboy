#include "background_window_pixel_fetcher.hpp"
#include "ppu.hpp"
#include "spdlog/spdlog.h"
#include "tile.hpp"
#include "tilemap.hpp"
#include <cassert>

void BackgroundWindowPixelFetcher::stepGetTile()
{
    if (_ticksInCurrentStep == 1)
    {
        /*
         * The tilemap is a 32x32 map of tiles of 8x8 pixels.
         * Since we are rendering only a line here, we can already compute which
         * line of the tilemap we are going to render.
         * We can compute the line by seeing how many tiles we span vertically.
         *
         * For the window, we use the window line counter instead of the scanline.
         * The window line counter only increments when the window is actually rendered,
         * not on every scanline. This allows the window to be shown/hidden mid-frame
         * and continue from the correct line.
         */
        int startLine;
        if (_mode == Mode::WINDOW)
        {
            // Window uses its own internal line counter
            startLine = _ppu->getWindowLineCounter();
        }
        else
        {
            // Background uses scanline + scroll Y, wrapped to 256
            startLine = (_ppu->getCurrentScanline() + _ppu->getScrollY()) & 255;
        }

        int lineInTileMap = (startLine / SingleTile::TILE_HEIGHT) % Tilemap::HEIGHT;
        assert(lineInTileMap < Tilemap::HEIGHT);

        int offsetInTileMap = lineInTileMap * Tilemap::WIDTH;

        int xIndexOffset = _mode == Mode::WINDOW ? _x : (_ppu->getScrollX() / SingleTile::TILE_WIDTH) + _x;
        // We see how many tiles we span horizontally and add it to our offset to find the tile index
        _tileIndex = offsetInTileMap + (xIndexOffset & 0x1F);

        int tilemapId = (_mode == Mode::WINDOW) ? _ppu->windowTileMapIndex() : _ppu->backgroundTileMapIndex();
        int tileId = _tilemaps[tilemapId].getTileIdForIndex(_tileIndex);

        _tileLine = (startLine % SingleTile::TILE_HEIGHT);
        _x++;

        _tileAddr = _vram->getTileAddrById(static_cast<byte>(tileId), _ppu->backgroundAndWindowTileDataAreaIndex());

        goToStep(Step::GetTileDataLow);
    }
    else
    {
        _ticksInCurrentStep++;
    }
}

BackgroundWindowPixelFetcher::BackgroundWindowPixelFetcher(VRAM* vram, PPU* ppu, PixelFIFO& pixelFifo)
    : _ppu(ppu), _vram(vram), _pixelFifo(pixelFifo)
{
    _tilemaps.reserve(2);
    _tilemaps.emplace_back(_vram, ADDR_MAP_0);
    _tilemaps.emplace_back(_vram, ADDR_MAP_1);
}

void BackgroundWindowPixelFetcher::step()
{
    if (_currentStep == Step::GetTile)
    {
        stepGetTile();
    }
    else if (_currentStep == Step::GetTileDataLow)
    {
        stepGetTileDataLow();
    }
    else if (_currentStep == Step::GetTileDataHigh)
    {
        stepGetTileDataHigh();
    }
    else if (_currentStep == Step::Push)
    {
        pushToFifo();
    }
}

void BackgroundWindowPixelFetcher::setMode(BackgroundWindowPixelFetcher::Mode mode)
{
    _mode = mode;
}

void BackgroundWindowPixelFetcher::stepGetTileDataLow()
{
    if (_ticksInCurrentStep == 1)
    {
        _bankId = 0;
        _paletteId = 0;
        _flippedHorizontally = false;
        _flippedVertically = false;
        _priority = 0;

        if (_ppu->getMMU().isColorModeSupported())
        {
            int tilemapId = (_mode == Mode::WINDOW) ? _ppu->windowTileMapIndex() : _ppu->backgroundTileMapIndex();
            Tilemap::TileInfo tileInfo = _tilemaps[tilemapId].getTileInfoForIndex(_tileIndex);
            _bankId = tileInfo.getVRAMBankId();
            _paletteId = tileInfo.getColorPaletteId();
            _flippedHorizontally = tileInfo.isFlippedHorizontally();
            _flippedVertically = tileInfo.isFlippedVertically();
            _priority = tileInfo.isRenderedAboveSprites();
        }

        // For vertical flip: line 0 becomes line 7, line 7 becomes line 0
        int line = _flippedVertically ? (SingleTile::TILE_HEIGHT - 1 - _tileLine) : _tileLine;
        _dataLow = _vram->readFromBank(static_cast<word>(_tileAddr + line * 2), _bankId);
        goToStep(Step::GetTileDataHigh);
    }
    else
    {
        _ticksInCurrentStep++;
    }
}

void BackgroundWindowPixelFetcher::stepGetTileDataHigh()
{
    if (_ticksInCurrentStep == 1)
    {
        // For vertical flip: line 0 becomes line 7, line 7 becomes line 0
        int line = _flippedVertically ? (SingleTile::TILE_HEIGHT - 1 - _tileLine) : _tileLine;
        _dataHigh = _vram->readFromBank(static_cast<word>(_tileAddr + line * 2 + 1), _bankId);
        goToStep(Step::Push);
    }
    else
    {
        _ticksInCurrentStep++;
    }
}

void BackgroundWindowPixelFetcher::pushToFifo()
{
    if (_pixelFifo.size() <= 8)
    {
        for (int x = 0; x < 8; ++x)
        {
            // The pixels are ordered from left to right, the highest bit is the leftmost pixel.
            // Normal: bit 7 is leftmost (x=0), bit 0 is rightmost (x=7)
            // Flipped: bit 0 is leftmost (x=0), bit 7 is rightmost (x=7)
            int bitPosition = _flippedHorizontally ? x : (7 - x);

            byte colorId = ((_dataHigh >> bitPosition) & 0x01) << 1 | ((_dataLow >> bitPosition) & 0x01);
            _pixelFifo.push({colorId, _paletteId, Pixel::Source::BG_WINDOW, _priority});
        }
        goToStep(Step::GetTile);
    }
}

void BackgroundWindowPixelFetcher::goToStep(BackgroundWindowPixelFetcher::Step step)
{
    _currentStep = step;
    _ticksInCurrentStep = 0;
}

void BackgroundWindowPixelFetcher::reset()
{
    _x = 0;
    _ticksInCurrentStep = 0;
    _currentStep = Step::GetTile;
}
