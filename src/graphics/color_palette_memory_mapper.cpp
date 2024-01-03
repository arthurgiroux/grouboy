#include "color_palette_memory_mapper.hpp"
#include "palette.hpp"

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

void ColorPaletteMemoryMapper::writeColor(byte value)
{
    int color = colorPalettes[getPaletteIndexFromAddr()].getRGB555ColorForId(getColorIdFromAddr());

    if (address % 2 == 1)
    {
        color = (value << 8) | (color & 0xFF);
    }
    else
    {
        color = (color & 0xFF00) | value;
    }

    colorPalettes[getPaletteIndexFromAddr()].setColorForId(color, getColorIdFromAddr());

    if (isAddrIncrementEnabled)
    {
        address = (address + 1) % 64;
    }
}

byte ColorPaletteMemoryMapper::readColor()
{
    int color = colorPalettes[getPaletteIndexFromAddr()].getRGB555ColorForId(getColorIdFromAddr());
    if (address % 2 == 1)
    {
        color >>= 8;
    }

    return color & 0xFF;
}

Palette& ColorPaletteMemoryMapper::getColorPalette(unsigned int index)
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