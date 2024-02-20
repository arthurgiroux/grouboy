#ifndef GROUBOY_PALETTE_HPP
#define GROUBOY_PALETTE_HPP

#include "graphics/rgb_color.hpp"
#include <array>

class Palette
{
  public:
    Palette() = default;
    virtual ~Palette() = default;
    virtual RGBColor getColorForId(unsigned int index) const = 0;
};

#endif // GROUBOY_PALETTE_HPP
