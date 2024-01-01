#include "grayscale_palette.hpp"
#include "rgb_color.hpp"

const RGBColor GrayscalePalette::COLOR_WHITE = RGBColor(255);
const RGBColor GrayscalePalette::COLOR_LIGHT_GRAY = RGBColor(170);
const RGBColor GrayscalePalette::COLOR_DARK_GRAY = RGBColor(85);
const RGBColor GrayscalePalette::COLOR_BLACK = RGBColor(0);

RGBColor GrayscalePalette::convertToColor(byte colorId)
{
    byte paletteId = convertColorId(colorId);

    if (paletteId == 0)
    {
        return COLOR_WHITE;
    }
    else if (paletteId == 1)
    {
        return COLOR_LIGHT_GRAY;
    }
    else if (paletteId == 2)
    {
        return COLOR_DARK_GRAY;
    }
    else
    {
        return COLOR_BLACK;
    }
}

GrayscalePalette::GrayscalePalette(MMU& mmu, word paletteAddr) : _mmu(mmu), _paletteAddr(paletteAddr)
{
}

byte GrayscalePalette::convertColorId(byte colorId)
{
    byte paletteData = _mmu.read(_paletteAddr);

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
