#include "tile.hpp"
#include <bitset>

void Tile::convertToPixels()
{
    for (int line = 0; line < _height; line++)
    {
        /*
         * The color of a pixel for a tile is encoded on 2 bits of two adjacents bytes.
         * Example:
         *      [0x803E] [ 0 | 1 | 0 | 0 | 1 | 1 | 1 | 0 ]
         *      [0x803F] [ 1 | 0 | 0 | 0 | 1 | 0 | 1 | 1 ]
         *      data  =    2   1   0   0   3   1   3   2
         *
         *  This data  will then be converted to a color through a palette giving a grayscale value.
         *
         */
        std::bitset<8> msb(_data[line * BYTES_PER_TILE_VALUE + 1]);
        std::bitset<8> lsb(_data[line * BYTES_PER_TILE_VALUE]);
        for (int x = 0; x < _width; x++)
        {
            // The pixels are ordered from left to right, the highest bit is the leftmost pixel.
            byte colorData = (msb[7 - x] << 1) | static_cast<int>(lsb[7 - x]);
            _colorData[line * _width + x] = colorData;
        }
    }
}

Tile::Tile(const Tile::TileDataArray& data, int height, int width) : _data(data), _height(height), _width(width)
{
    _colorData.resize(height * width);
    convertToPixels();
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
    return _colorData[y * _width + x];
}

const std::vector<byte>& Tile::getColorData() const
{
    return _colorData;
}

SingleTile::SingleTile(const Tile::TileDataArray& data) : Tile(data, TILE_HEIGHT, TILE_WIDTH)
{
}

StackedTile::StackedTile(const Tile::TileDataArray& data) : Tile(data, TILE_HEIGHT, TILE_WIDTH)
{
}