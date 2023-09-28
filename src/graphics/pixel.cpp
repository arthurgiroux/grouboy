#include "pixel.hpp"

Pixel::Pixel(byte color, byte palette, byte spritePriority, byte backgroundPriority)
    : _color(color), _palette(palette), _spritePriority(spritePriority), _backgroundPriority(backgroundPriority)
{
}

byte Pixel::getColor() const
{
    return _color;
}

byte Pixel::getPalette() const
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
    return _color == rhs._color && _palette == rhs._palette && _spritePriority == rhs._spritePriority &&
           _backgroundPriority == rhs._backgroundPriority;
}

bool Pixel::operator!=(const Pixel& rhs) const
{
    return !(rhs == *this);
}
