#ifndef GROUBOY_SPRITE_PIXEL_FETCHER_HPP
#define GROUBOY_SPRITE_PIXEL_FETCHER_HPP

#include "common/types.hpp"
#include "pixel_fifo.hpp"
#include "sprite.hpp"

class VRAM;
class PPU;

/**
 * Fetches sprite pixel data from VRAM and pushes them to the OAM FIFO.
 * The sprite fetcher has a 5-step state machine per Pan Docs:
 * GetTileId (2 dots) -> GetTileDataLow (2 dots) -> GetTileDataHigh (2 dots) -> Sleep (2 dots) -> Push
 */
class SpritePixelFetcher
{
  public:
    SpritePixelFetcher(VRAM* vram, PPU* ppu, PixelFIFO& oamFifo);
    ~SpritePixelFetcher() = default;

    /**
     * Start fetching pixels for a sprite.
     * @param sprite The sprite to fetch
     * @param scanline The current scanline being rendered
     */
    void startFetching(Sprite* sprite, int scanline);

    /**
     * Perform one step of the sprite fetcher state machine.
     * @return true when the fetch is complete and pixels have been pushed
     */
    bool step();

    /**
     * Check if the fetcher is currently active (fetching a sprite).
     * @return true if active
     */
    bool isActive() const;

    /**
     * Reset the fetcher to its initial state.
     */
    void reset();

  private:
    enum class Step
    {
        GetTileId,
        GetTileDataLow,
        GetTileDataHigh,
        Sleep,
        Push
    };

    void stepGetTileId();
    void stepGetTileDataLow();
    void stepGetTileDataHigh();
    void stepSleep();
    void pushToFifo();
    void goToStep(Step step);

    PPU* _ppu;
    VRAM* _vram;
    PixelFIFO& _oamFifo;

    Step _currentStep = Step::GetTileId;
    bool _active = false;
    int _ticksInCurrentStep = 0;

    // Current sprite being fetched
    Sprite* _sprite = nullptr;
    int _scanline = 0;

    // Tile fetching state
    byte _tileId = 0;
    int _tileLine = 0;
    word _tileAddr = 0;
    int _bankId = 0;
    byte _dataLow = 0;
    byte _dataHigh = 0;
};

#endif // GROUBOY_SPRITE_PIXEL_FETCHER_HPP
