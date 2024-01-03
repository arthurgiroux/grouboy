#include "rgb_color.hpp"

const RGBColor RGBColor::WHITE = RGBColor(255);
const RGBColor RGBColor::LIGHT_GRAY = RGBColor(170);
const RGBColor RGBColor::DARK_GRAY = RGBColor(85);
const RGBColor RGBColor::BLACK = RGBColor(0);

RGBColor::RGBColor(byte r, byte g, byte b) : red(r), green(g), blue(b)
{
}

RGBColor::RGBColor(byte grayscale) : RGBColor(grayscale, grayscale, grayscale)
{
}

byte RGBColor::getRed() const
{
    return red;
}

byte RGBColor::getGreen() const
{
    return green;
}

byte RGBColor::getBlue() const
{
    return blue;
}

bool RGBColor::operator==(const RGBColor& rhs) const
{
    return red == rhs.red && green == rhs.green && blue == rhs.blue;
}

bool RGBColor::operator!=(const RGBColor& rhs) const
{
    return !(rhs == *this);
}

RGBColor RGBColor::fromRGB555(int value)
{
    byte red = (((value) & 0x1F) * 255) / 31;
    byte green = (((value >> 5) & 0x1F) * 255) / 31;
    byte blue = ((value >> 10 & 0x1F) * 255) / 31;
    return {red, green, blue};
}

int RGBColor::toRGB555() const
{
    unsigned short r5 = (red * 31) / 255;
    unsigned short g5 = (green * 31) / 255;
    unsigned short b5 = (blue * 31) / 255;

    unsigned short rgb555 = (r5 << 10) | (g5 << 5) | b5;

    return rgb555;
}
