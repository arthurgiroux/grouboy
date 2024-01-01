#ifndef GROUBOY_RGB_COLOR_HPP
#define GROUBOY_RGB_COLOR_HPP

#include "common/utils.hpp"

/**
 * A class for handling RGB color.
 * Resolution of each color is 1-byte
 */
class RGBColor
{
  public:
    /**
     * Create a new color from the 3 rgb components
     * @param r amount of red (0, 255)
     * @param g amount of green (0, 255)
     * @param b amount of blue (0, 255)
     */
    RGBColor(byte r, byte g, byte b);

    /**
     * Create a new color from a grayscale value
     * @param grayscale a grayscale value (0, 255)
     */
    explicit RGBColor(byte grayscale);

    /**
     * Get the amount of red in the color
     * @return a value between 0 and 255
     */
    byte getRed() const;

    /**
     * Get the amount of green in the color
     * @return a value between 0 and 255
     */
    byte getGreen() const;

    /**
     * Get the amount of blue in the color
     * @return a value between 0 and 255
     */
    byte getBlue() const;

    bool operator==(const RGBColor& rhs) const;
    bool operator!=(const RGBColor& rhs) const;

    /**
     * The color black
     */
    static const RGBColor BLACK;

    /**
     * The color white
     */
    static const RGBColor WHITE;

    /**
     * The color dark gray
     */
    static const RGBColor DARK_GRAY;

    /**
     * The color for light gray
     */
    static const RGBColor LIGHT_GRAY;

  private:
    byte red = 0;
    byte green = 0;
    byte blue = 0;
};

#endif // GROUBOY_RGB_COLOR_HPP
