#ifndef GBEMULATOR_TILE_H
#define GBEMULATOR_TILE_H

#include "rgb_image.hpp"
#include "types.hpp"
#include <array>

class Tile
{
  public:
	static const int BYTES_PER_TILE = 16;
	static const int BYTES_PER_TILE_VALUE = 2;
	static const int TILE_WIDTH = 8;
	static const int TILE_HEIGHT = 8;

	using TileDataArray = std::array<byte, BYTES_PER_TILE>;

	Tile(const TileDataArray& data);

	const RGBImage& getImage() const;

	static const int COLOR_BLACK;
	static const int COLOR_WHITE;
	static const int COLOR_DARK_GRAY;
	static const int COLOR_LIGHT_GRAY;
	
  private:
	byte paletteValueToGrayscale(byte value) const;
	void convertToPixels();

	TileDataArray data = {};
	RGBImage image = RGBImage(TILE_HEIGHT, TILE_WIDTH);
};

#endif // GBEMULATOR_TILE_H
