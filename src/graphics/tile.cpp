#include "tile.hpp"
#include "palette.hpp"
#include <bitset>

void Tile::convertToPixels()
{
    for (int line = 0; line < TILE_HEIGHT; line++)
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
        std::bitset<8> msb(data[line * BYTES_PER_TILE_VALUE + 1]);
        std::bitset<8> lsb(data[line * BYTES_PER_TILE_VALUE]);
        for (int x = 0; x < TILE_WIDTH; x++)
        {
            // The pixels are ordered from left to right, the highest bit is the leftmost pixel.
            byte colorData = (msb[7 - x] << 1) | static_cast<int>(lsb[7 - x]);
            _colorData[line * TILE_WIDTH + x] = colorData;
            byte grayscaleValue = Palette::convertColorToGrayscale(colorData);

            image.setPixel(x, line, grayscaleValue);
        }
    }
}

Tile::Tile(const Tile::TileDataArray& data_) : data(data_)
{
    convertToPixels();
}

const RGBImage& Tile::getImage() const
{
    return image;
}

byte Tile::getColorData(int x, int y) const
{
    return _colorData[y * TILE_WIDTH + x];
}
