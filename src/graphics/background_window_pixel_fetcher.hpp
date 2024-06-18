#ifndef GROUBOY_BACKGROUND_WINDOW_PIXEL_FETCHER_HPP
#define GROUBOY_BACKGROUND_WINDOW_PIXEL_FETCHER_HPP

#include "pixel_fifo.hpp"
#include "tilemap.hpp"

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
    void step();
    void setMode(Mode mode);
    void reset();

  private:
    enum class Step
    {
        GetTile,
        GetTileDataLow,
        GetTileDataHigh,
        Sleep,
        Push
    };

    void stepGetTile();
    void stepGetTileDataLow();
    void stepGetTileDataHigh();
    void pushToFifo();
    void goToStep(Step step);

    PPU* _ppu;
    VRAM* _vram;
    PixelFIFO& _pixelFifo;
    Step _currentStep = Step::GetTile;
    Mode _mode = Mode::BACKGROUND;
    int _ticksInCurrentStep = 0;
    int _bankId = 0;
    word _tileAddr = 0;
    int _tileIndex = 0;
    int _tileLine = 0;
    int _x = 0;
    byte _dataHigh = 0;
    byte _dataLow = 0;

    std::vector<Tilemap> _tilemaps{};

    /**
     * The address of the tile map with index 0.
     */
    static constexpr word ADDR_MAP_0 = 0x9800;

    /**
     * The address of the tile map with index 1.
     */
    static constexpr word ADDR_MAP_1 = 0x9C00;

    bool _flippedHorizontally = false;
    bool _flippedVertically = false;
    int _priority = 0;
    int _paletteId = 0;
};

#endif // GROUBOY_BACKGROUND_WINDOW_PIXEL_FETCHER_HPP
