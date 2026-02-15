#include "pixel_fifo.hpp"

void PixelFIFO::push(Pixel pixel)
{
    _fifo.push_back(pixel);
}

Pixel PixelFIFO::pop()
{
    auto pixel = _fifo.front();
    _fifo.pop_front();
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

size_t PixelFIFO::size() const
{
    return _fifo.size();
}

void PixelFIFO::clear()
{
    _fifo.clear();
}

Pixel& PixelFIFO::at(size_t index)
{
    return _fifo.at(index);
}
