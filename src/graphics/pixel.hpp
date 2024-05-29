#ifndef GROUBOY_PIXEL_HPP
#define GROUBOY_PIXEL_HPP

#include "common/types.hpp"
#include "graphics/palette/palette.hpp"

/**
 * Represents a pixel that can potentially be displayed on the screen.
 * A pixel is a combination of different parameters until it's rendered,
 * it has a color, a palette, some priorities that will helped determined if
 * it should be rendered or not and a source (either background/window or sprite).
 */
class Pixel
{
  public:
    /**
     * From where the pixel originated
     */
    enum class Source
    {
        BG_WINDOW, // Background or window
        SPRITE     // Sprite
    };

    Pixel() = default;

    /**
     * Create a new pixel
     *
     * @param colorId The id of the color, before palette adjustment
     * @param palette The palette to use to convert the color id to the final color
     * @param source The source from where the pixel was created
     * @param priority The priority of the pixel, will be used to check if it should be overridden
     */
    Pixel(byte colorId, Palette* palette, Source source, int priority = 0);
    Pixel(byte colorId, int paletteId, Source source, int priority = 0);
    ~Pixel() = default;

    /**
     * Get the color id of the pixel
     * @return an id between 0 and 3
     */
    byte getColorId() const;

    /**
     * Get the palette used to convert the color
     * @return A pointer to the palette
     */
    Palette* getPalette() const;

    /**
     * Get the priority of the pixel.
     * A pixel with a higher priority can override a pixel with lower priority
     * in certain conditions.
     * @return a signed integer
     */
    int getPriority() const;

    /**
     * Get from where the pixel originated.
     * @return The source of the pixel
     */
    Source getSource() const;

    int getPaletteId() const;
    bool operator==(const Pixel& rhs) const;
    bool operator!=(const Pixel& rhs) const;

  private:
    /**
     * The color id of the pixel.
     */
    byte _colorId;

    /**
     * A pointer to the palette that will be used for converting the color.
     */
    Palette* _palette;

    /**
     * The source from where the pixel was created.
     */
    Source _source;

    /**
     * The priority of the pixel.
     */
    int _priority;

    int _paletteId;
};

#endif // GROUBOY_PIXEL_HPP
