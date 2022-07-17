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
	for (int i = 0; i < BYTES_PER_TILE; i += BYTES_PER_TILE_VALUE)
	{
		std::bitset<8> msb(data[i + 1]);
		std::bitset<8> lsb(data[i]);
		for (int j = 0; j < 8; j++)
		{
			byte rawValue = (msb[j] << 1) | lsb[j];
			byte grayscaleValue = paletteValueToGrayscale(rawValue);
			int x = i / 2;
			pixels[x * 8 * 3 + j * 3] = grayscaleValue;
			pixels[x * 8 * 3 + j * 3 + 1] = grayscaleValue;
			pixels[x * 8 * 3 + j * 3 + 2] = grayscaleValue;
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
