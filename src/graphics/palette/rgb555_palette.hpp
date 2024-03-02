#ifndef GROUBOY_RGB555_PALETTE_HPP
#define GROUBOY_RGB555_PALETTE_HPP

#include "graphics/palette/palette.hpp"

/**
 * A palette that maps a color id to an RGB value.
 * The RGB value is stored as an RGB555.
 */
class RGB555Palette : public Palette
{
  public:
    RGB555Palette() = default;
    RGBColor getColorForId(unsigned int index) const override;

    /**
     * Set the RGB color for a specific color id.
     * @param color The RGB color as a RGB555 value.
     * @param index The color id to map to this color
     */
    void setColorForId(int color, unsigned int index);

    /**
     * Get the RGB color for a color id.
     *
     * @param index The color id
     * @return an RGB555 color
     */
    int getRGB555ColorForId(unsigned int index) const;

  private:
    /**
     * The mapping between RGB555 value and the color id
     */
    std::array<int, 4> colorMapping = {};
};

#endif // GROUBOY_RGB555_PALETTE_HPP
