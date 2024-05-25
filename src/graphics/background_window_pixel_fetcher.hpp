#ifndef GROUBOY_BACKGROUND_WINDOW_PIXEL_FETCHER_HPP
#define GROUBOY_BACKGROUND_WINDOW_PIXEL_FETCHER_HPP

#include "pixel_fifo.hpp"

// Forward declaration
class VRAM;
class PPU;

class BackgroundWindowPixelFetcher
{
  public:
    enum class Mode
    {
        BACKGROUND,
        WINDOW,
    };

    BackgroundWindowPixelFetcher(VRAM* vram, PPU* ppu, PixelFIFO& pixelFifo);
    void step(int cycles);
    void setMode(Mode mode);

  private:
    enum class Step
    {
        GetTile,
        GetTileDataLow,
        GetTileDataHigh,
        Sleep,
        Push
    };

    void getTile();

    PPU* _ppu;
    VRAM* _vram;
    PixelFIFO& _pixelFifo;
    Step _currentStep = Step::GetTile;
    Mode _mode = Mode::BACKGROUND;
};

#endif // GROUBOY_BACKGROUND_WINDOW_PIXEL_FETCHER_HPP
