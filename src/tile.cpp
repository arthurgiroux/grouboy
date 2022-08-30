#include "tile.hpp"
#include <bitset>

const int Tile::COLOR_BLACK = 0;
const int Tile::COLOR_WHITE = 255;
const int Tile::COLOR_DARK_GRAY = 192;
const int Tile::COLOR_LIGHT_GRAY = 96;

byte Tile::paletteValueToGrayscale(byte value) const
{
	if (value == 0)
	{
		return COLOR_WHITE;
	}
	else if (value == 1)
	{
		return COLOR_LIGHT_GRAY;
	}
	else if (value == 2)
	{
		return COLOR_DARK_GRAY;
	}
	else
	{
		return COLOR_BLACK;
	}
}

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
			byte tileData = (msb[7 - x] << 1) | static_cast<int>(lsb[7 - x]);
			byte grayscaleValue = paletteValueToGrayscale(tileData);

			pixels[(line * TILE_WIDTH + x) * BYTES_PER_PIXEL] = grayscaleValue;
			pixels[(line * TILE_WIDTH + x) * BYTES_PER_PIXEL + 1] = grayscaleValue;
			pixels[(line * TILE_WIDTH + x) * BYTES_PER_PIXEL + 2] = grayscaleValue;
		}
	}
}

Tile::Tile(const Tile::TileDataArray& data_) : data(data_)
{
	convertToPixels();
}

const Tile::TileRGBArray& Tile::toRGB()
{
	return pixels;
}
