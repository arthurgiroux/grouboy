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
    ASSERT_EQ(palette.convertToColor(0), GrayscalePalette::COLOR_WHITE);
    ASSERT_EQ(palette.convertToColor(1), GrayscalePalette::COLOR_LIGHT_GRAY);
    ASSERT_EQ(palette.convertToColor(2), GrayscalePalette::COLOR_DARK_GRAY);
    ASSERT_EQ(palette.convertToColor(3), GrayscalePalette::COLOR_BLACK);
}

TEST_F(GrayscalePaletteTest, PaletteMappingShouldBeReadFromMemory)
{
    mmu.write(0x0000, 0b00011011);
    ASSERT_EQ(palette.convertToColor(3), GrayscalePalette::COLOR_WHITE);
    ASSERT_EQ(palette.convertToColor(2), GrayscalePalette::COLOR_LIGHT_GRAY);
    ASSERT_EQ(palette.convertToColor(1), GrayscalePalette::COLOR_DARK_GRAY);
    ASSERT_EQ(palette.convertToColor(0), GrayscalePalette::COLOR_BLACK);
}