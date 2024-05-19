#ifndef GBEMULATOR_TILE_H
#define GBEMULATOR_TILE_H

#include "common/types.hpp"
#include "rgb_image.hpp"
#include <map>
#include <vector>

class VRAM;

/**
 * A tile are graphical element that can be displayed as part of the background/window maps,
 * and/or as part of sprites.
 * Tiles pixel are binary encoded in memory and each pixel can have 4 values.
 */
class Tile
{
  public:
    /**
     * Create a new tile of a specified size from raw data
     *
     * @param height    The height in pixels
     * @param width     The width in pixels
     * @param vram      The VRAM to use to retrieve the tile data
     * @param bankId    The Bank ID of the VRAM where the tile data is stored
     * @param startAddr The address in the VRAM where the tile data starts
     */
    Tile(int height, int width, VRAM* vram, int bankId, word startAddr);
    virtual ~Tile() = default;

    /**
     * Get the color data of a given pixel
     *
     * @param x the x coordinate in pixel
     * @param y the y coordinate in pixel
     * @return  the color data, values [0, 4]
     */
    byte getColorData(int x, int y);

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
    void lazyLoadTileData(int line);

    /**
     * The height of the tile in pixels
     */
    int _height;

    /**
     * The width of the image in pixels
     */
    int _width;

    /**
     * The memory from where we load
     */
    VRAM* _vram;

    /**
     * The bank id where the tile information are retrieved from
     */
    int _bankId;

    /**
     * The address where the tile data starts in the VRAM
     */
    word _startAddr;

    /**
     * A map of the tile data that is lazy loaded.
     * The index is the line number.
     */
    std::map<int, word> _data = {};
};

/**
 * A single 8x8 tile.
 */
class SingleTile : public Tile
{
  public:
    /**
     * Create a new single 8x8 tile
     * @param vram      The VRAM to use to retrieve the tile data
     * @param bankId    The Bank ID of the VRAM where the tile data is stored
     * @param startAddr The address in the VRAM where the tile data starts
     */
    SingleTile(VRAM* vram, int bankId, word startAddr);

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
     * @param vram      The VRAM to use to retrieve the tile data
     * @param bankId    The Bank ID of the VRAM where the tile data is stored
     * @param startAddr The address in the VRAM where the tile data starts
     */
    StackedTile(VRAM* vram, int bankId, word startAddr);

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
