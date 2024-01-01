#ifndef GBEMULATOR_PALETTE_HPP
#define GBEMULATOR_PALETTE_HPP

#include "generic_palette.hpp"
#include "graphics/rgb_color.hpp"
#include "memory/mmu.hpp"

/**
 * A Grayscale palette is a mapping from one color id to another.
 * The mapping is read from an address in memory.
 * GrayscalePalette are used to extend the reusability of sprites
 * by being able to switch colors before rendering without having
 * to store another sprite data.
 */
class GrayscalePalette : public GenericPalette
{
  public:
    /**
     * Create a new palette from a memory slot.
     *
     * @param mmu   The MMU to use to access the memory.
     * @param paletteAddr   The address of the palette mapping.
     */
    GrayscalePalette(MMU& mmu, word paletteAddr);

    /**
     * Convert a given color to its grayscale representation.
     *
     * @param colorId the color id to convert [0, 3]
     * @return a grayscale colorId [0, 255]
     */
    RGBColor convertToColor(byte colorId);

  private:
    /**
     * Convert a specific color id with the palette.
     *
     * @param colorId the color id to convert [0, 3]
     * @return the converted color [0, 3]
     */
    byte convertColorId(byte colorId);

    /**
     * The MMU to use to retrieve information from memory.
     */
    MMU& _mmu;

    /**
     * The address in memory where the color mapping is stored.
     */
    word _paletteAddr;
};

#endif // GBEMULATOR_PALETTE_HPP
