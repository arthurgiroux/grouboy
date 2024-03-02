#include "generic_palette.hpp"

GenericPalette::GenericPalette(std::array<RGBColor, 4> colors) : colorMapping(colors)
{
}

void GenericPalette::setColorForId(RGBColor color, unsigned int index)
{
    colorMapping[index] = color;
}

RGBColor GenericPalette::getColorForId(unsigned int index) const
{
    return colorMapping[index];
}
