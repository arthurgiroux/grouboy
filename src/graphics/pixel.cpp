#include "pixel.hpp"
#include "palette.hpp"

Pixel::Pixel(byte colorId, Palette* palette, byte spritePriority, byte backgroundPriority)
    : _colorId(colorId), _palette(palette), _spritePriority(spritePriority), _backgroundPriority(backgroundPriority)
{
}

byte Pixel::getColorId() const
{
    return _colorId;
}

Palette* Pixel::getPalette() const
{
    return _palette;
}

byte Pixel::getSpritePriority() const
{
    return _spritePriority;
}

byte Pixel::getBackgroundPriority() const
{
    return _backgroundPriority;
}

bool Pixel::operator==(const Pixel& rhs) const
{
    return _colorId == rhs._colorId && _palette == rhs._palette && _spritePriority == rhs._spritePriority &&
           _backgroundPriority == rhs._backgroundPriority;
}

bool Pixel::operator!=(const Pixel& rhs) const
{
    return !(rhs == *this);
}
