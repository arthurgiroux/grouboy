#include "tile.hpp"
#include <bitset>
#include <string>
#include <exception>


byte Tile::paletteValueToGrayscale(byte value) const {
    if (value == 0) {
        return 255;
    }
    else if (value == 1) {
        return 192;
    }
    else if (value == 2) {
        return 96;
    }
    else {
        return 0;
    }
}

void Tile::convertToPixels() {
    for (int i = 0; i < BYTES_PER_TILE; i += BYTES_PER_TILE_VALUE)  {
        std::bitset<8> msb = data[i * BYTES_PER_TILE_VALUE + 1];
        std::bitset<8> lsb = data[i * BYTES_PER_TILE_VALUE];
        for (int j = 0; j < 8; j++) {
            byte rawValue = (msb[j] << 1) || lsb[j];
            byte grayscaleValue = paletteValueToGrayscale(rawValue);
            int x = i / 2;
            pixels[x * 8 * 3 + j * 3]
        }
    }
}

Tile::Tile(const Tile::TileDataArray &data_) : data(data_) {
        convertToPixels();
}
