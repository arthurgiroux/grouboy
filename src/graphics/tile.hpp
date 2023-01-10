#ifndef GBEMULATOR_TILE_H
#define GBEMULATOR_TILE_H

#include "common/types.hpp"
#include "rgb_image.hpp"
#include <array>

class Tile
{
  public:
    static const int BYTES_PER_TILE = 16;
    static const int BYTES_PER_TILE_VALUE = 2;
    static const int TILE_WIDTH = 8;
    static const int TILE_HEIGHT = 8;

    using TileDataArray = std::array<byte, BYTES_PER_TILE>;
    using ColorDataArray = std::array<byte, TILE_HEIGHT * TILE_WIDTH>;

    Tile(const TileDataArray& data);

    const RGBImage& getImage() const;
    byte getColorData(int x, int y) const;

  private:
    void convertToPixels();

    TileDataArray data = {};
    ColorDataArray _colorData = {};
    RGBImage image = RGBImage(TILE_HEIGHT, TILE_WIDTH);
};

#endif // GBEMULATOR_TILE_H
