#ifndef GROUBOY_PIXEL_HPP
#define GROUBOY_PIXEL_HPP

#include "common/types.hpp"
#include "palette.hpp"

class Pixel
{
  public:
    Pixel() = default;
    Pixel(byte colorId, Palette* palette, byte spritePriority, byte backgroundPriority);
    ~Pixel() = default;
    byte getColorId() const;
    Palette* getPalette() const;
    byte getSpritePriority() const;
    byte getBackgroundPriority() const;
    bool operator==(const Pixel& rhs) const;
    bool operator!=(const Pixel& rhs) const;

  private:
    byte _colorId;
    Palette* _palette;
    byte _spritePriority;
    byte _backgroundPriority;
};

#endif // GROUBOY_PIXEL_HPP
