#include "background_window_pixel_fetcher.hpp"

void BackgroundWindowPixelFetcher::getTile()
{
}

BackgroundWindowPixelFetcher::BackgroundWindowPixelFetcher(VRAM* vram, PPU* ppu, PixelFIFO& pixelFifo)
    : _vram(vram), _ppu(ppu), _pixelFifo(pixelFifo)
{
}

void BackgroundWindowPixelFetcher::step(int cycles)
{
}

void BackgroundWindowPixelFetcher::setMode(BackgroundWindowPixelFetcher::Mode mode)
{
    _mode = mode;
}
