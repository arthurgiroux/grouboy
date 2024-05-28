#include "background_window_pixel_fetcher.hpp"
#include "ppu.hpp"
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
         */
        int startLine = _mode == Mode::WINDOW ? _y : (_ppu->getCurrentScanline() + _ppu->getScrollY()) & 255;
        int lineInTileMap = (startLine / SingleTile::TILE_HEIGHT) % Tilemap::HEIGHT;
        assert(lineInTileMap < Tilemap::HEIGHT);

        int offsetInTileMap = lineInTileMap * Tilemap::WIDTH;
    }
    else
    {
        _ticksInCurrentStep++;
    }
}

BackgroundWindowPixelFetcher::BackgroundWindowPixelFetcher(VRAM* vram, PPU* ppu, PixelFIFO& pixelFifo)
    : _ppu(ppu), _vram(vram), _pixelFifo(pixelFifo)
{
}

void BackgroundWindowPixelFetcher::step(int cycles)
{
    int cyclesLeft = cycles;
    while (cyclesLeft > 0)
    {
        if (_currentStep == Step::GetTile)
        {
            stepGetTile();
        }

        cyclesLeft--;
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
        _dataLow = _vram->readFromBank(static_cast<word>(_tileAddr + _tileLine * 2), _bankId);
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
        _dataHigh = _vram->readFromBank(static_cast<word>(_tileAddr + _tileLine * 2 + 1), _bankId);
        goToStep(Step::Push);
    }
    else
    {
        _ticksInCurrentStep++;
    }
}

void BackgroundWindowPixelFetcher::pushToFifo()
{
    if (_pixelFifo.isEmpty())
    {
        for (int x = 0; x < 8; ++x)
        {
            // The pixels are ordered from left to right, the highest bit is the leftmost pixel.
            int bitPosition = (7 - x);

            byte colorId = ((_dataHigh >> bitPosition) & 0x01) << 1 | ((_dataLow >> bitPosition) & 0x01);
            _pixelFifo.push({colorId, nullptr, Pixel::Source::BG_WINDOW, 0});
        }
        goToStep(Step::Sleep);
    }
}

void BackgroundWindowPixelFetcher::goToStep(BackgroundWindowPixelFetcher::Step step)
{
    _currentStep = step;
    _ticksInCurrentStep = 0;
}
