#include "tile.hpp"
#include <bitset>

Tile::Tile(Tile::TileDataArray&& data, int height, int width) : _data(std::move(data)), _height(height), _width(width)
{
}

int Tile::getHeight() const
{
    return _height;
}

int Tile::getWidth() const
{
    return _width;
}

byte Tile::getColorData(int x, int y) const
{
    /*
     * The color of a pixel for a tile is encoded on 2 bits of two adjacent bytes.
     * Example:
     *      [0x803E] [ 0 | 1 | 0 | 0 | 1 | 1 | 1 | 0 ]
     *      [0x803F] [ 1 | 0 | 0 | 0 | 1 | 0 | 1 | 1 ]
     *      data  =    2   1   0   0   3   1   3   2
     *
     *  This data  will then be converted to a color through a palette giving a grayscale value.
     *
     */
    std::bitset<8> msb(_data[y * BYTES_PER_TILE_VALUE + 1]);
    std::bitset<8> lsb(_data[y * BYTES_PER_TILE_VALUE]);
    // The pixels are ordered from left to right, the highest bit is the leftmost pixel.
    return (msb[7 - x] << 1) | static_cast<int>(lsb[7 - x]);
}

SingleTile::SingleTile(Tile::TileDataArray&& data) : Tile(std::move(data), TILE_HEIGHT, TILE_WIDTH)
{
}

StackedTile::StackedTile(Tile::TileDataArray&& data) : Tile(std::move(data), TILE_HEIGHT, TILE_WIDTH)
{
}