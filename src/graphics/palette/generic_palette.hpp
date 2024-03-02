#ifndef GROUBOY_GENERIC_PALETTE_HPP
#define GROUBOY_GENERIC_PALETTE_HPP

#include "graphics/rgb_color.hpp"
#include "palette.hpp"
#include <array>

/**
 * A generic palette that maps the color ids to
 */
class GenericPalette : public Palette
{
  public:
    /**
     * Create a new palette that maps color id to simple predefined RGB color
     * @param colors
     */
    explicit GenericPalette(std::array<RGBColor, 4> colors);

    /**
     * Set the RGB color that will be map for a specific id
     * @param color the color that will be mapped to the index
     * @param index the index of the color to map
     */
    void setColorForId(RGBColor color, unsigned int index);
    RGBColor getColorForId(unsigned int index) const override;

  protected:
    std::array<RGBColor, 4> colorMapping = {RGBColor::BLACK, RGBColor::BLACK, RGBColor::BLACK, RGBColor::BLACK};
};

#endif // GROUBOY_GENERIC_PALETTE_HPP
