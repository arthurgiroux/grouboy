#ifndef COLORPALETTEMEMORYMAPPER_H
#define COLORPALETTEMEMORYMAPPER_H

#include "generic_palette.hpp"
#include "rgb555_palette.hpp"
#include <array>

/**
 * The color palette memory mapper gives access to 8 RGB555 palettes through a memory interface.
 * Since the underlying color format values are stored on 15bits, each color in the palette will map
 * to 2 bytes, allowing to set/get the full color spectrum through read/write.
 */
class ColorPaletteMemoryMapper
{
  public:
    /**
     * Set the address on which any read/write action will be performed.
     * The color values of each palette are mapped on two bytes.
     * Each palette has 4 colors, and there's 8 palettes.
     * So for example:
     * address 0x00-0x01 is the color 0 of palette 0
     * address 0x02-0x03 is the color 1 of palette 0
     * address 0x08-0x09 is the color 0 of palette 1
     *
     * @param addr The address that will be used for read/write
     */
    void setAddress(byte addr);

    /**
     * Get the address that will be affected by read/write actions.
     * @return The address
     * @see setAddress for address mapping explanation
     */
    byte getAddress() const;

    /**
     * Enable or disable the auto increment of the address.
     * When auto-increment is enabled, after any write to the current address it will be incremented.
     * This is usually used if you want to sequentially update a range of color.
     * Note that the address is not incremented after a read.
     *
     * @param value true if address should be incremented, false otherwise
     */
    void enableAddressAutoIncrement(bool value);

    /**
     * Is the address incremented after a write operation.
     *
     * @return true if address should be incremented, false otherwise
     * @see enableAddressAutoIncrement for more information
     */
    bool isAddressAutoIncrementEnabled() const;

    /**
     * Write the color value to the current address
     * @param value msb or lsb of the RGB555 color value
     */
    void writeColor(byte value);

    /**
     * Read the color value of the current address
     * @return msb or lsb of the RGB555 color value
     */
    byte readColor();

    /**
     * Get a specific palette
     * @param index an integer between 0 and 7
     * @return a reference to the color palette
     */
    Palette& getColorPalette(unsigned int index);

  private:
    /**
     * Helper to get palette index from an address
     * @return a palette index, between 0 and 7
     */
    int getPaletteIndexFromAddr() const;

    /**
     * Helper to get the color id from an address
     * @return a color index between 0 and 3
     */
    int getColorIdFromAddr() const;

    /**
     * The number of palettes mapped
     */
    static const int NBR_PALETTE = 8;

    /**
     * The number of colors per palette
     */
    static const int COLOR_PER_PALETTE = 4;

    /**
     * The number of bytes for each color of the palette
     */
    static const int BYTES_PER_COLOR = 2;

    /**
     * The current address that will be used for read/write
     */
    byte address = 0;

    /**
     * Whether or not the address should auto-increment after a write operation
     */
    bool isAddrIncrementEnabled = false;

    /**
     * The array of palettes that is mapped
     */
    std::array<RGB555Palette, NBR_PALETTE> colorPalettes;
};

#endif // COLORPALETTEMEMORYMAPPER_H
