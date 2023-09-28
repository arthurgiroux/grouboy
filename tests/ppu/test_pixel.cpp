#include "graphics/pixel.hpp"
#include <gtest/gtest.h>

TEST(PixelTest, PixelColorCanBeSetAndGetCorrectly)
{
    auto pixel = Pixel(0x0F, 0, 0, 0);
    ASSERT_EQ(pixel.getColor(), 0x0F);
}

TEST(PixelTest, PixelPaletteCanBeSetAndGetCorrectly)
{
    auto pixel = Pixel(0, 0x0F, 0, 0);
    ASSERT_EQ(pixel.getPalette(), 0x0F);
}

TEST(PixelTest, PixelSpritePriorityCanBeSetAndGetCorrectly)
{
    auto pixel = Pixel(0, 0, 0x0F, 0);
    ASSERT_EQ(pixel.getSpritePriority(), 0x0F);
}

TEST(PixelTest, PixelBackgroundPriorityCanBeSetAndGetCorrectly)
{
    auto pixel = Pixel(0, 0, 0, 0x0F);
    ASSERT_EQ(pixel.getBackgroundPriority(), 0x0F);
}