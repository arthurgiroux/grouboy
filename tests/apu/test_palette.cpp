#include "graphics/palette.hpp"
#include <gtest/gtest.h>

class PaletteTest : public ::testing::Test
{
  protected:
    MMU mmu;
};

TEST_F(PaletteTest, PaletteConvertColorIdShouldAccordingToValueInMemory)
{
    int paletteAddr = 0x1234;
    Palette palette(mmu, paletteAddr);

    // We remap all colors to the opposite one
    int colorMapping = 0b00011011;
    mmu.write(paletteAddr, colorMapping);
    ASSERT_EQ(palette.convertColorId(0), 3);
    ASSERT_EQ(palette.convertColorId(1), 2);
    ASSERT_EQ(palette.convertColorId(2), 1);
    ASSERT_EQ(palette.convertColorId(3), 0);
}

TEST_F(PaletteTest, MultiplePaletteShouldWorkIndependently)
{
    int paletteAddr1 = 0x1234;
    Palette palette1(mmu, paletteAddr1);
    int colorMapping1 = 0b00011011;
    mmu.write(paletteAddr1, colorMapping1);

    int paletteAddr2 = 0x1235;
    Palette palette2(mmu, paletteAddr2);
    int colorMapping2 = 0b11100100;
    mmu.write(paletteAddr2, colorMapping2);

    ASSERT_EQ(palette1.convertColorId(0), 3);
    ASSERT_EQ(palette1.convertColorId(1), 2);
    ASSERT_EQ(palette1.convertColorId(2), 1);
    ASSERT_EQ(palette1.convertColorId(3), 0);

    ASSERT_EQ(palette2.convertColorId(0), 0);
    ASSERT_EQ(palette2.convertColorId(1), 1);
    ASSERT_EQ(palette2.convertColorId(2), 2);
    ASSERT_EQ(palette2.convertColorId(3), 3);
}

TEST_F(PaletteTest, ConvertingColor0ToGrayscaleShouldGiveWhite)
{
    ASSERT_EQ(Palette::convertColorToGrayscale(0), Palette::COLOR_WHITE);
}

TEST_F(PaletteTest, ConvertingColor1ToGrayscaleShouldGiveLightGray)
{
    ASSERT_EQ(Palette::convertColorToGrayscale(1), Palette::COLOR_LIGHT_GRAY);
}

TEST_F(PaletteTest, ConvertingColor2ToGrayscaleShouldGiveDarkGray)
{
    ASSERT_EQ(Palette::convertColorToGrayscale(2), Palette::COLOR_DARK_GRAY);
}

TEST_F(PaletteTest, ConvertingColor2ToGrayscaleShouldGiveBlack)
{
    ASSERT_EQ(Palette::convertColorToGrayscale(3), Palette::COLOR_BLACK);
}