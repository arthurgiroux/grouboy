#ifndef GROUBOY_PIXEL_HPP
#define GROUBOY_PIXEL_HPP

#include "common/types.hpp"

class Pixel
{
  public:
    Pixel(byte color, byte palette, byte spritePriority, byte backgroundPriority);
    ~Pixel() = default;
    byte getColor() const;
    byte getPalette() const;
    byte getSpritePriority() const;
    byte getBackgroundPriority() const;
    bool operator==(const Pixel& rhs) const;
    bool operator!=(const Pixel& rhs) const;

  private:
    byte _color;
    byte _palette;
    byte _spritePriority;
    byte _backgroundPriority;
};

#endif // GROUBOY_PIXEL_HPP
