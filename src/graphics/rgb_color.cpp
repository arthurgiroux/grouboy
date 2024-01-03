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
    byte red = utils::convertFrom5BitsTo8Bits(static_cast<byte>(value));
    byte green = utils::convertFrom5BitsTo8Bits(static_cast<byte>(value >> 5));
    byte blue = utils::convertFrom5BitsTo8Bits(static_cast<byte>(value >> 10));
    return {red, green, blue};
}

int RGBColor::toRGB555() const
{
    byte r5 = utils::convertFrom8BitsTo5Bits(red);
    byte g5 = utils::convertFrom8BitsTo5Bits(green);
    byte b5 = utils::convertFrom8BitsTo5Bits(blue);

    return (b5 << 10) | (g5 << 5) | r5;
}
