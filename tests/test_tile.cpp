#include "graphics/palette.hpp"
#include "graphics/tile.hpp"
#include <gtest/gtest.h>

TEST(TileTest, AllZeroTileShouldBeAllWhite)
{
    Tile::TileDataArray data = {};
    Tile tile(data);
    const std::vector<byte>& pixels = tile.getImage().getData();

    for (size_t i = 0; i < pixels.size(); i++)
    {
        ASSERT_EQ(pixels[i], Palette::COLOR_WHITE);
    }
}

TEST(TileTest, FilledTileShouldBeAllBlack)
{
    Tile::TileDataArray data = {};
    data.fill(0xFF);
    Tile tile(data);
    const std::vector<byte>& pixels = tile.getImage().getData();

    for (size_t i = 0; i < pixels.size(); i++)
    {
        ASSERT_EQ(pixels[i], Palette::COLOR_BLACK);
    }
}

TEST(TileTest, LSBTileShouldBeAllLightGray)
{
    Tile::TileDataArray data = {};
    for (size_t i = 0; i < data.size(); ++i)
    {
        if (i % 2 == 0)
        {
            data[i] = 0xFF;
        }
        else
        {
            data[i] = 0;
        }
    }
    Tile tile(data);
    const std::vector<byte>& pixels = tile.getImage().getData();

    for (size_t i = 0; i < pixels.size(); i++)
    {
        ASSERT_EQ(pixels[i], Palette::COLOR_LIGHT_GRAY);
    }
}

TEST(TileTest, MSBTileShouldBeAllDarkGray)
{
    Tile::TileDataArray data = {};
    for (size_t i = 0; i < data.size(); ++i)
    {
        if (i % 2 == 1)
        {
            data[i] = 0xFF;
        }
        else
        {
            data[i] = 0;
        }
    }
    Tile tile(data);
    const std::vector<byte>& pixels = tile.getImage().getData();

    for (size_t i = 0; i < pixels.size(); i++)
    {
        ASSERT_EQ(pixels[i], Palette::COLOR_DARK_GRAY);
    }
}