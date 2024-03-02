#include "graphics/palette/generic_palette.hpp"
#include "graphics/pixel.hpp"
#include <gtest/gtest.h>

TEST(PixelTest, PixelColorCanBeSetAndGetCorrectly)
{
    auto pixel = Pixel(0x0F, nullptr, Pixel::Source::SPRITE);
    ASSERT_EQ(pixel.getColorId(), 0x0F);
}

TEST(PixelTest, PixelPaletteCanBeSetAndGetCorrectly)
{
    GenericPalette palette({RGBColor::BLACK, RGBColor::BLACK, RGBColor::BLACK, RGBColor::BLACK});
    auto pixel = Pixel(0, &palette, Pixel::Source::SPRITE);
    ASSERT_EQ(pixel.getPalette(), &palette);
}

TEST(PixelTest, PixelPriorityCanBeSetAndGetCorrectly)
{
    auto pixel = Pixel(0, nullptr, Pixel::Source::SPRITE, 0x0F);
    ASSERT_EQ(pixel.getPriority(), 0x0F);
}

TEST(PixelTest, PixelSourceCanBeSetAndGetCorrectly)
{
    auto pixel = Pixel(0, nullptr, Pixel::Source::BG_WINDOW);
    ASSERT_EQ(pixel.getSource(), Pixel::Source::BG_WINDOW);
}