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
