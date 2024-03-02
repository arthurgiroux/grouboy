#ifndef GBEMULATOR_PALETTE_HPP
#define GBEMULATOR_PALETTE_HPP

#include "generic_palette.hpp"
#include "graphics/rgb_color.hpp"
#include "memory/mmu.hpp"

/**
 * A Grayscale palette maps the color id to shades of gray.
 * The mapping between color id and shade of gray can be set from the MMU.
 */
class GrayscalePalette : public GenericPalette
{
  public:
    /**
     * Create a new palette from a memory address.
     *
     * @param mmu   The MMU to use to access the memory.
     * @param paletteAddr   The address of the palette mapping.
     */
    GrayscalePalette(MMU& mmu, word paletteAddr);

    RGBColor getColorForId(unsigned int index) const override;

  private:
    /**
     * Convert a specific color id with the palette.
     *
     * @param colorId the color id to convert [0, 3]
     * @return the converted color [0, 3]
     */
    byte convertColorId(byte colorId) const;

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
