#include "graphics/tile.hpp"
#include "memory/vram.hpp"
#include <gtest/gtest.h>

TEST(SingleTileTest, HeightShouldBe8)
{
    std::vector<byte> data(16);
    SingleTile tile(nullptr, 0, 0);
    ASSERT_EQ(tile.getHeight(), 8);
}

TEST(SingleTileTest, WidthShouldBe8)
{
    std::vector<byte> data(16);
    SingleTile tile(nullptr, 0, 0);
    ASSERT_EQ(tile.getWidth(), 8);
}

TEST(SingleTileTest, ColorIdShouldBeReadFromAdjacentsBytes)
{
    VRAM vram;
    /*
     * [ 0 | 1 | 0 | 0 | 1 | 1 | 1 | 0 ]
     * [ 1 | 0 | 0 | 0 | 1 | 0 | 1 | 1 ]
     *   2   1   0   0   3   1   3   2
     */
    vram.write(0x16, 0b01001110);
    vram.write(0x17, 0b10001011);
    SingleTile tile(&vram, 0, 0x16);
    ASSERT_EQ(tile.getColorData(0, 0), 2);
    ASSERT_EQ(tile.getColorData(1, 0), 1);
    ASSERT_EQ(tile.getColorData(2, 0), 0);
    ASSERT_EQ(tile.getColorData(3, 0), 0);
    ASSERT_EQ(tile.getColorData(4, 0), 3);
    ASSERT_EQ(tile.getColorData(5, 0), 1);
    ASSERT_EQ(tile.getColorData(6, 0), 3);
    ASSERT_EQ(tile.getColorData(7, 0), 2);
}

TEST(StackedTileTest, HeightShouldBe16)
{
    std::vector<byte> data(16);
    StackedTile tile(nullptr, 0, 0);
    ASSERT_EQ(tile.getHeight(), 16);
}

TEST(StackedTileTest, WidthShouldBe8)
{
    std::vector<byte> data(16);
    StackedTile tile(nullptr, 0, 0);
    ASSERT_EQ(tile.getWidth(), 8);
}