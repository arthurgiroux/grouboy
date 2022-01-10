#include <gtest/gtest.h>
#include "tile.hpp"

TEST(TileTest, AllZeroTileShouldBeAllWhite) {
    Tile::TileDataArray data = {};
    Tile tile(data);
    const Tile::TileRGBArray& pixels = tile.toRGB();

    for (int i = 0; i < pixels.size(); i++) {
        ASSERT_EQ(pixels[i], Tile::COLOR_WHITE);
    }
}

TEST(TileTest, FilledTileShouldBeAllBlack) {
    Tile::TileDataArray data = {};
    data.fill(0xFF);
    Tile tile(data);
    const Tile::TileRGBArray& pixels = tile.toRGB();

    for (int i = 0; i < pixels.size(); i++) {
        ASSERT_EQ(pixels[i], Tile::COLOR_BLACK);
    }
}

TEST(TileTest, LSBTileShouldBeAllLightGray) {
    Tile::TileDataArray data = {};
    for (int i = 0; i < data.size(); ++i) {
        if (i % 2 == 0) {
            data[i] = 0xFF;
        }
        else {
            data[i] = 0;
        }
    }
    Tile tile(data);
    const Tile::TileRGBArray& pixels = tile.toRGB();

    for (int i = 0; i < pixels.size(); i++) {
        ASSERT_EQ(pixels[i], Tile::COLOR_LIGHT_GRAY);
    }
}


TEST(TileTest, MSBTileShouldBeAllDarkGray) {
    Tile::TileDataArray data = {};
    for (int i = 0; i < data.size(); ++i) {
        if (i % 2 == 1) {
            data[i] = 0xFF;
        } else {
            data[i] = 0;
        }
    }
    Tile tile(data);
    const Tile::TileRGBArray &pixels = tile.toRGB();

    for (int i = 0; i < pixels.size(); i++) {
        ASSERT_EQ(pixels[i], Tile::COLOR_DARK_GRAY);
    }
}