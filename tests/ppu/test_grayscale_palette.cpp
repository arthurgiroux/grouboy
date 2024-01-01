#include "graphics/grayscale_palette.hpp"
#include "graphics/tile.hpp"
#include <gtest/gtest.h>

class GrayscalePaletteTest : public ::testing::Test
{
  protected:
    MMU mmu;
    GrayscalePalette palette = GrayscalePalette(mmu, 0x0000);
};

TEST_F(GrayscalePaletteTest, DefaultPaletteMappingShouldMapFromWhiteToBlack)
{
    mmu.write(0x0000, 0b11100100);
    ASSERT_EQ(palette.convertToColor(0), RGBColor::WHITE);
    ASSERT_EQ(palette.convertToColor(1), RGBColor::LIGHT_GRAY);
    ASSERT_EQ(palette.convertToColor(2), RGBColor::DARK_GRAY);
    ASSERT_EQ(palette.convertToColor(3), RGBColor::BLACK);
}

TEST_F(GrayscalePaletteTest, PaletteMappingShouldBeReadFromMemory)
{
    mmu.write(0x0000, 0b00011011);
    ASSERT_EQ(palette.convertToColor(3), RGBColor::WHITE);
    ASSERT_EQ(palette.convertToColor(2), RGBColor::LIGHT_GRAY);
    ASSERT_EQ(palette.convertToColor(1), RGBColor::DARK_GRAY);
    ASSERT_EQ(palette.convertToColor(0), RGBColor::BLACK);
}