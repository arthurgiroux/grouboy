#include "grayscale_palette.hpp"
#include "graphics/rgb_color.hpp"

GrayscalePalette::GrayscalePalette(MMU& mmu, word paletteAddr)
    : GenericPalette({RGBColor::WHITE, RGBColor::LIGHT_GRAY, RGBColor::DARK_GRAY, RGBColor::BLACK}), _mmu(mmu),
      _paletteAddr(paletteAddr)
{
}

byte GrayscalePalette::convertColorId(byte colorId) const
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

RGBColor GrayscalePalette::getColorForId(unsigned int index) const
{
    return colorMapping[convertColorId(static_cast<byte>(index))];
}
