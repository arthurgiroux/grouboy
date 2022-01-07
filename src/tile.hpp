#ifndef GBEMULATOR_TILE_H
#define GBEMULATOR_TILE_H

#include <array>
#include "types.hpp"

class Tile {
public:
    static const int BYTES_PER_TILE = 16;
    static const int BYTES_PER_TILE_VALUE = 2;
    static const int TILE_WIDTH = 8;
    static const int TILE_HEIGHT = 8;
    static const int TILE_SIZE = TILE_WIDTH * TILE_HEIGHT;
    static const int BYTES_PER_PIXEL = 3;

    using TileRGBArray = std::array<byte, TILE_SIZE * BYTES_PER_PIXEL>;
    using TileDataArray = std::array<byte, BYTES_PER_TILE>;

    Tile(const TileDataArray& data);

    const TileRGBArray& toRGB();


#ifndef UNIT_TESTING
private:
#endif
    byte paletteValueToGrayscale(byte value) const;
    void convertToPixels();

    TileDataArray data = {};
    TileRGBArray pixels = {};
};

#endif //GBEMULATOR_TILE_H
