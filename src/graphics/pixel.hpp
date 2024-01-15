#ifndef GROUBOY_PIXEL_HPP
#define GROUBOY_PIXEL_HPP

#include "common/types.hpp"
#include "palette.hpp"

class Pixel
{
  public:
    Pixel() = default;
    Pixel(byte colorId, Palette* palette, int spritePriority, int backgroundPriority);
    ~Pixel() = default;
    byte getColorId() const;
    Palette* getPalette() const;
    int getSpritePriority() const;
    int getBackgroundPriority() const;
    bool operator==(const Pixel& rhs) const;
    bool operator!=(const Pixel& rhs) const;

  private:
    byte _colorId;
    Palette* _palette;
    int _spritePriority;
    int _backgroundPriority;
};

#endif // GROUBOY_PIXEL_HPP
