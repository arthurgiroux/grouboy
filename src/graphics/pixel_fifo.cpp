#include "pixel_fifo.hpp"

void PixelFIFO::push(Pixel pixel)
{
    _fifo.push(pixel);
}

Pixel PixelFIFO::pop()
{
    auto pixel = _fifo.front();
    _fifo.pop();
    return pixel;
}

bool PixelFIFO::isEmpty() const
{
    return _fifo.empty();
}

bool PixelFIFO::isFull() const
{
    return _fifo.size() == MAX_SIZE;
}
