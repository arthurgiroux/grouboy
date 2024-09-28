#ifndef GROUBOY_PIXEL_FIFO_RENDERER_HPP
#define GROUBOY_PIXEL_FIFO_RENDERER_HPP

#include "background_window_pixel_fetcher.hpp"
#include "pixel_fifo.hpp"

class MMU;
class PPU;
class PixelFifoRenderer
{
  public:
    PixelFifoRenderer(MMU*, PPU* ppu);
    ~PixelFifoRenderer() = default;
    void step();
    void reset();
    int getX() const;

  private:
    MMU* _mmu;
    PPU* _ppu;
    BackgroundWindowPixelFetcher _bgWindowPixelFetcher;
    PixelFIFO _backgroundWindowFIFO;
    PixelFIFO _spritesFIFO;
    int _x = 0;
};

#endif // GROUBOY_PIXEL_FIFO_RENDERER_HPP
