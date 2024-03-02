#include "pixel.hpp"
#include "graphics/palette/palette.hpp"

Pixel::Pixel(byte colorId, Palette* palette, Source source, int priority)
    : _colorId(colorId), _palette(palette), _source(source), _priority(priority)
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

int Pixel::getPriority() const
{
    return _priority;
}

Pixel::Source Pixel::getSource() const
{
    return _source;
}

bool Pixel::operator==(const Pixel& rhs) const
{
    return _colorId == rhs._colorId && _palette == rhs._palette && _priority == rhs._priority && _source == rhs._source;
}

bool Pixel::operator!=(const Pixel& rhs) const
{
    return !(rhs == *this);
}
