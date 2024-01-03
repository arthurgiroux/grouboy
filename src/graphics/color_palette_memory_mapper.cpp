#include "color_palette_memory_mapper.hpp"

#include <stdexcept>

void ColorPaletteMemoryMapper::setAddress(byte addr)
{
    address = addr;
}

byte ColorPaletteMemoryMapper::getAddress() const
{
    return address;
}

void ColorPaletteMemoryMapper::enableAddressAutoIncrement(bool value)
{
    isAddrIncrementEnabled = value;
}

bool ColorPaletteMemoryMapper::isAddressAutoIncrementEnabled() const
{
    return isAddrIncrementEnabled;
}

void ColorPaletteMemoryMapper::writeColor(int value)
{
    int color = colorPalettes[getPaletteIndexFromAddr()].getColorForId(getColorIdFromAddr()).toRGB555();

    if (address % 2 == 1)
    {
        value <<= 8;
    }

    color &= value;

    colorPalettes[getPaletteIndexFromAddr()].setColorForId(RGBColor::fromRGB555(color), getColorIdFromAddr());

    if (isAddrIncrementEnabled)
    {
        address = (address + 1) % 64;
    }
}

int ColorPaletteMemoryMapper::readColor()
{
    int color = colorPalettes[getPaletteIndexFromAddr()].getColorForId(getColorIdFromAddr()).toRGB555();
    if (address % 2 == 1)
    {
        color >>= 8;
    }

    return color & 0xFF;
}

GenericPalette& ColorPaletteMemoryMapper::getColorPalette(unsigned int index)
{
    if (index >= colorPalettes.size())
    {
        throw std::runtime_error("Invalid color palette id");
    }

    return colorPalettes[index];
}

int ColorPaletteMemoryMapper::getPaletteIndexFromAddr() const
{
    return address / (2 * 4);
}

int ColorPaletteMemoryMapper::getColorIdFromAddr() const
{
    return (address / 2) % 4;
}