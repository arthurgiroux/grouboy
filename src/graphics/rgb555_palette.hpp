#ifndef GROUBOY_RGB555_PALETTE_HPP
#define GROUBOY_RGB555_PALETTE_HPP

#include "graphics/palette.hpp"

class RGB555Palette : public Palette
{
  public:
    RGB555Palette() = default;
    void setColorForId(int color, unsigned int index);
    RGBColor getColorForId(unsigned int index) const override;
    int getRGB555ColorForId(unsigned int index) const;

  private:
    std::array<int, 4> colorMapping = {};
};

#endif // GROUBOY_RGB555_PALETTE_HPP
