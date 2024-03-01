#ifndef GROUBOY_PALETTE_HPP
#define GROUBOY_PALETTE_HPP

#include "graphics/rgb_color.hpp"
#include <array>

/**
 * A palette is a mapping between a color index and a RGB color.
 * Palettes are used to extend the reusability of sprites
 * by being able to switch colors before rendering without having
 * to store another sprite data.
 */
class Palette
{
  public:
    Palette() = default;
    virtual ~Palette() = default;

    /**
     * Get the color mapping for a specific color id
     * @param index The color index
     * @return The color that is mapped to this id
     */
    virtual RGBColor getColorForId(unsigned int index) const = 0;
};

#endif // GROUBOY_PALETTE_HPP
