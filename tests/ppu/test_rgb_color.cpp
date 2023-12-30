#include "graphics/rgb_color.hpp"
#include <gtest/gtest.h>

TEST(RGBColor, RGBColorCanBeSetFromRGBValues)
{
    byte r = 1;
    byte g = 2;
    byte b = 3;
    RGBColor color = RGBColor(r, g, b);
    ASSERT_EQ(color.getRed(), r);
    ASSERT_EQ(color.getGreen(), g);
    ASSERT_EQ(color.getBlue(), b);
}

TEST(RGBColor, RGBColorCanBeSetFromGrayscaleValue)
{
    byte grayscale = 128;
    RGBColor color = RGBColor(grayscale);
    ASSERT_EQ(color.getRed(), grayscale);
    ASSERT_EQ(color.getGreen(), grayscale);
    ASSERT_EQ(color.getBlue(), grayscale);
}