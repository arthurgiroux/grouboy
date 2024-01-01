#ifndef GROUBOY_GENERIC_PALETTE_HPP
#define GROUBOY_GENERIC_PALETTE_HPP

#include "rgb_color.hpp"
#include <array>

class GenericPalette
{
  public:
    GenericPalette();
    explicit GenericPalette(std::array<RGBColor, 4> colors);
    RGBColor getColorForId(unsigned int index) const;
    void setColorForId(RGBColor color, unsigned int index);

  private:
    std::array<RGBColor, 4> colorMapping = {RGBColor::BLACK, RGBColor::BLACK, RGBColor::BLACK, RGBColor::BLACK};
};

#endif // GROUBOY_GENERIC_PALETTE_HPP
