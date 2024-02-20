#include "graphics/generic_palette.hpp"
#include "graphics/pixel.hpp"
#include <gtest/gtest.h>

TEST(PixelTest, PixelColorCanBeSetAndGetCorrectly)
{
    auto pixel = Pixel(0x0F, nullptr, 0, 0, Pixel::Source::SPRITE);
    ASSERT_EQ(pixel.getColorId(), 0x0F);
}

TEST(PixelTest, PixelPaletteCanBeSetAndGetCorrectly)
{
    GenericPalette palette({RGBColor::BLACK, RGBColor::BLACK, RGBColor::BLACK, RGBColor::BLACK});
    auto pixel = Pixel(0, &palette, 0, 0, Pixel::Source::SPRITE);
    ASSERT_EQ(pixel.getPalette(), &palette);
}

TEST(PixelTest, PixelSpritePriorityCanBeSetAndGetCorrectly)
{
    auto pixel = Pixel(0, nullptr, 0x0F, 0, Pixel::Source::SPRITE);
    ASSERT_EQ(pixel.getSpritePriority(), 0x0F);
}

TEST(PixelTest, PixelBackgroundPriorityCanBeSetAndGetCorrectly)
{
    auto pixel = Pixel(0, nullptr, 0, 0x0F, Pixel::Source::SPRITE);
    ASSERT_EQ(pixel.getBackgroundPriority(), 0x0F);
}

TEST(PixelTest, PixelSourceCanBeSetAndGetCorrectly)
{
    auto pixel = Pixel(0, nullptr, 0, 0x0F, Pixel::Source::BG_WINDOW);
    ASSERT_EQ(pixel.getSource(), Pixel::Source::BG_WINDOW);
}