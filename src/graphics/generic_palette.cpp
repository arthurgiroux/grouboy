#include "generic_palette.hpp"

GenericPalette::GenericPalette()
{
}

GenericPalette::GenericPalette(std::array<RGBColor, 4> colors) : colorMapping(colors)
{
}

RGBColor GenericPalette::getColorForId(unsigned int index) const
{
    return colorMapping[index];
}

void GenericPalette::setColorForId(RGBColor color, unsigned int index)
{
    colorMapping[index] = color;
}
