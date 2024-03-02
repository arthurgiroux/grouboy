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
        color = utils::createWordFromBytes(value, utils::getLsbFromWord(color));
    }
    else
    {
        color = utils::createWordFromBytes(utils::getMsbFromWord(color), value);
    }

    colorPalettes[getPaletteIndexFromAddr()].setColorForId(color, getColorIdFromAddr());

    if (isAddrIncrementEnabled)
    {
        address = (address + 1) % (NBR_PALETTE * COLOR_PER_PALETTE * BYTES_PER_COLOR);
    }
}

byte ColorPaletteMemoryMapper::readColor()
{
    int color = colorPalettes[getPaletteIndexFromAddr()].getRGB555ColorForId(getColorIdFromAddr());
    if (address % 2 == 1)
    {
        return utils::getMsbFromWord(color);
    }

    return utils::getLsbFromWord(color);
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
    return address / (BYTES_PER_COLOR * COLOR_PER_PALETTE);
}

int ColorPaletteMemoryMapper::getColorIdFromAddr() const
{
    return (address / BYTES_PER_COLOR) % COLOR_PER_PALETTE;
}