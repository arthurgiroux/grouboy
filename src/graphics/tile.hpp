#ifndef GBEMULATOR_TILE_H
#define GBEMULATOR_TILE_H

#include "common/types.hpp"
#include "rgb_image.hpp"
#include <vector>

/**
 * A tile are graphical element that can be displayed as part of the background/window maps,
 * and/or as part of sprites.
 * Tiles pixel are binary encoded in memory and each pixel can have 4 values.
 */
class Tile
{
  public:
    /**
     * Type for an array holding the raw tile data
     */
    using TileDataArray = std::vector<byte>;

    /**
     * Type for an array holding the color representation of the tile
     */
    using ColorDataArray = std::vector<byte>;

    /**
     * Create a new tile of a specified size from raw data
     *
     * @param data      The raw data to read the colors from
     * @param height    The height in pixels
     * @param width     The width in pixels
     */
    Tile(TileDataArray&& data, int height, int width);
    virtual ~Tile() = default;

    /**
     * Get the color data of a given pixel
     *
     * @param x the x coordinate in pixel
     * @param y the y coordinate in pixel
     * @return  the color data, values [0, 4]
     */
    byte getColorData(int x, int y) const;

    /**
     * Get the full color data array
     * @return The color data for the tile
     */
    const std::vector<byte>& getColorData() const;

    /**
     * Get the tile's height
     *
     * @return the height in pixels
     */
    int getHeight() const;

    /**
     * Get the tile's width
     *
     * @return the width in pixels
     */
    int getWidth() const;

    /**
     * Number of bytes per value for the tile
     */
    static const int BYTES_PER_TILE_VALUE = 2;

  protected:
    /**
     * Convert the tile data to colored pixels
     */
    void convertToPixels();

    /**
     * The raw data of the tile
     */
    TileDataArray _data = {};

    /**
     * The tile converted to color data
     */
    ColorDataArray _colorData = {};

    /**
     * The height of the tile in pixels
     */
    int _height;

    /**
     * The width of the image in pixels
     */
    int _width;
};

/**
 * A single 8x8 tile.
 */
class SingleTile : public Tile
{
  public:
    /**
     * Create a new single 8x8 tile
     *
     * @param data      The raw data to read the colors from
     */
    SingleTile(TileDataArray&& data);

    /**
     * Number of bytes per value for the tile
     */
    static const int BYTES_PER_TILE = 16;

    /**
     * Tile width in pixels
     */
    static const int TILE_WIDTH = 8;

    /**
     * Tile height in pixels
     */
    static const int TILE_HEIGHT = 8;
};

/**
 * An abstraction for two vertically stacked tile, giving an 8x16 tile.
 */
class StackedTile : public Tile
{
  public:
    /**
     * Create a 8x16 stacked tile
     *
     * @param data      The raw data to read the colors from
     */
    StackedTile(TileDataArray&& data);

    /**
     * Number of bytes per value for the tile
     */
    static const int BYTES_PER_TILE = 32;

    /**
     * Tile width in pixels
     */
    static const int TILE_WIDTH = 8;

    /**
     * Tile height in pixels
     */
    static const int TILE_HEIGHT = 16;
};

#endif // GBEMULATOR_TILE_H
