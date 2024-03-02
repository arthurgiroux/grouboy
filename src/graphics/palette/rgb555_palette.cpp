#include "rgb555_palette.hpp"

void RGB555Palette::setColorForId(int color, unsigned int index)
{
    colorMapping[index] = color;
}

RGBColor RGB555Palette::getColorForId(unsigned int index) const
{
    return RGBColor::fromRGB555(colorMapping[index]);
}

int RGB555Palette::getRGB555ColorForId(unsigned int index) const
{
    return colorMapping[index];
}
