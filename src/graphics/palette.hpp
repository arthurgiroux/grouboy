#ifndef GBEMULATOR_PALETTE_HPP
#define GBEMULATOR_PALETTE_HPP

#include "memory/mmu.hpp"

/**
 * A Palette is a mapping from one color id to another.
 * The mapping is read from an address in memory.
 * Palette are used to extend the reusability of sprites
 * by being able to switch colors before rendering without having
 * to store another sprite data.
 */
class Palette
{
  public:
    /**
     * Create a new palette from a memory slot.
     *
     * @param mmu   The MMU to use to access the memory.
     * @param paletteAddr   The address of the palette mapping.
     */
    Palette(MMU& mmu, word paletteAddr);

    /**
     * Convert a specific color id with the palette.
     *
     * @param colorId the color id to convert [0, 3]
     * @return the converted color [0, 3]
     */
    byte convertColorId(byte colorId);

    /**
     * Convert a given color to its grayscale representation.
     *
     * @param value the color id to convert [0, 3]
     * @return a grayscale value [0, 255]
     */
    static byte convertColorToGrayscale(byte value);

    /**
     * The grayscale value [0, 255] for the color black
     */
    static const int COLOR_BLACK;

    /**
     * The grayscale value [0, 255] for the color white
     */
    static const int COLOR_WHITE;

    /**
     * The grayscale value [0, 255] for the color dark gray
     */
    static const int COLOR_DARK_GRAY;

    /**
     * The grayscale value [0, 255] for the light gray
     */
    static const int COLOR_LIGHT_GRAY;

  private:
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
