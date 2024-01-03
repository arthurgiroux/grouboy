#ifndef COLORPALETTEMEMORYMAPPER_H
#define COLORPALETTEMEMORYMAPPER_H

#include "graphics/generic_palette.hpp"
#include "rgb555_palette.hpp"
#include <array>

class ColorPaletteMemoryMapper
{
  public:
    void setAddress(byte addr);
    byte getAddress() const;
    void enableAddressAutoIncrement(bool value);
    bool isAddressAutoIncrementEnabled() const;
    void writeColor(byte value);
    byte readColor();

    Palette& getColorPalette(unsigned int index);

  private:
    int getPaletteIndexFromAddr() const;
    int getColorIdFromAddr() const;
    byte address = 0;
    bool isAddrIncrementEnabled = false;
    std::array<RGB555Palette, 8> colorPalettes;
};

#endif // COLORPALETTEMEMORYMAPPER_H
