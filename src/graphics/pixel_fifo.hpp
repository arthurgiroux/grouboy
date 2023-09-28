#ifndef GROUBOY_PIXEL_FIFO_HPP
#define GROUBOY_PIXEL_FIFO_HPP

#include "pixel.hpp"
#include <queue>

class PixelFIFO
{
  public:
    PixelFIFO() = default;
    ~PixelFIFO() = default;
    void push(Pixel pixel);
    Pixel pop();
    bool isEmpty() const;
    bool isFull() const;

  private:
    static const int MAX_SIZE = 16;
    std::queue<Pixel> _fifo;
};

#endif // GROUBOY_PIXEL_FIFO_HPP
