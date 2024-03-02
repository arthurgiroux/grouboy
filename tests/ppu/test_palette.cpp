#include "graphics/palette/grayscale_palette.hpp"
#include <gtest/gtest.h>

class PaletteTest : public ::testing::Test
{
  protected:
    MMU mmu;
};

TEST_F(PaletteTest, GetColorForIdShoulReturnCorrectColor)
{
    GenericPalette palette({RGBColor::BLACK, RGBColor::LIGHT_GRAY, RGBColor::DARK_GRAY, RGBColor::WHITE});

    ASSERT_EQ(palette.getColorForId(0), RGBColor::BLACK);
    ASSERT_EQ(palette.getColorForId(1), RGBColor::LIGHT_GRAY);
    ASSERT_EQ(palette.getColorForId(2), RGBColor::DARK_GRAY);
    ASSERT_EQ(palette.getColorForId(3), RGBColor::WHITE);
}

TEST_F(PaletteTest, SetColorForIdShoulSetColor)
{
    GenericPalette palette({RGBColor::BLACK, RGBColor::WHITE, RGBColor::BLACK, RGBColor::BLACK});

    palette.setColorForId(RGBColor::WHITE, 0);
    ASSERT_EQ(palette.getColorForId(0), RGBColor::WHITE);
    palette.setColorForId(RGBColor::BLACK, 1);
    ASSERT_EQ(palette.getColorForId(1), RGBColor::BLACK);
    palette.setColorForId(RGBColor::LIGHT_GRAY, 2);
    ASSERT_EQ(palette.getColorForId(2), RGBColor::LIGHT_GRAY);
    palette.setColorForId(RGBColor::DARK_GRAY, 3);
    ASSERT_EQ(palette.getColorForId(3), RGBColor::DARK_GRAY);
}