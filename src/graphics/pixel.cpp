#include "pixel.hpp"
#include "palette.hpp"

Pixel::Pixel(byte colorId, Palette* palette, int spritePriority, int backgroundPriority, Source source)
    : _colorId(colorId), _palette(palette), _spritePriority(spritePriority), _backgroundPriority(backgroundPriority),
      _source(source)
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

int Pixel::getSpritePriority() const
{
    return _spritePriority;
}

int Pixel::getBackgroundPriority() const
{
    return _backgroundPriority;
}

Pixel::Source Pixel::getSource() const
{
    return _source;
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
