#include "palette.hpp"

const int Palette::COLOR_BLACK = 0;
const int Palette::COLOR_WHITE = 255;
const int Palette::COLOR_DARK_GRAY = 192;
const int Palette::COLOR_LIGHT_GRAY = 96;

byte Palette::convertColorToGrayscale(byte value)
{
    if (value == 0)
    {
        return COLOR_WHITE;
    }
    else if (value == 1)
    {
        return COLOR_LIGHT_GRAY;
    }
    else if (value == 2)
    {
        return COLOR_DARK_GRAY;
    }
    else
    {
        return COLOR_BLACK;
    }
}

Palette::Palette(MMU& mmu, word paletteAddr) : _mmu(mmu), _paletteAddr(paletteAddr)
{
}

byte Palette::convertColorId(byte colorId)
{
    int paletteData = _mmu.read(_paletteAddr);

    /*
     * The mapping with color is the following:
     *  Bit 7-6 - Color for index 3
     *  Bit 5-4 - Color for index 2
     *  Bit 3-2 - Color for index 1
     *  Bit 1-0 - Color for index 0
     */
    int bitsPerMapping = 2;
    int bitMask = 0x03;
    
    return (paletteData >> (colorId * bitsPerMapping)) & bitMask;
}
