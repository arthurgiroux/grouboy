#ifndef GROUBOY_GENERIC_PALETTE_HPP
#define GROUBOY_GENERIC_PALETTE_HPP

#include "palette.hpp"
#include "rgb_color.hpp"
#include <array>

class GenericPalette : public Palette
{
  public:
    explicit GenericPalette(std::array<RGBColor, 4> colors);
    void setColorForId(RGBColor color, unsigned int index);
    RGBColor getColorForId(unsigned int index) const override;

  protected:
    std::array<RGBColor, 4> colorMapping = {RGBColor::BLACK, RGBColor::BLACK, RGBColor::BLACK, RGBColor::BLACK};
};

#endif // GROUBOY_GENERIC_PALETTE_HPP
