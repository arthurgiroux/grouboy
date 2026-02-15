#ifndef GROUBOY_PIXEL_FIFO_HPP
#define GROUBOY_PIXEL_FIFO_HPP

#include "pixel.hpp"
#include <deque>

class PixelFIFO
{
  public:
    PixelFIFO() = default;
    ~PixelFIFO() = default;
    void push(Pixel pixel);
    Pixel pop();
    bool isEmpty() const;
    bool isFull() const;
    size_t size() const;
    void clear();

    /**
     * Get a reference to the pixel at the given index.
     * Used for sprite mixing where we need to modify pixels in place.
     * @param index The index in the FIFO (0 = front)
     * @return Reference to the pixel at that index
     */
    Pixel& at(size_t index);

  private:
    static const int MAX_SIZE = 16;
    std::deque<Pixel> _fifo;
};

#endif // GROUBOY_PIXEL_FIFO_HPP
