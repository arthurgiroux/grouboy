#include "graphics/grayscale_palette.hpp"
#include <gtest/gtest.h>

class PaletteTest : public ::testing::Test
{
  protected:
    MMU mmu;
};

TEST_F(PaletteTest, PaletteConvertColorIdShouldAccordingToValueInMemory)
{
    int paletteAddr = 0x1234;
    GrayscalePalette palette(mmu, paletteAddr);

    // We remap all colors to the opposite one
    int colorMapping = 0b00011011;
    mmu.write(paletteAddr, colorMapping);
    ASSERT_EQ(palette.convertToColor(0), RGBColor::BLACK);
    ASSERT_EQ(palette.convertToColor(1), RGBColor::DARK_GRAY);
    ASSERT_EQ(palette.convertToColor(2), RGBColor::LIGHT_GRAY);
    ASSERT_EQ(palette.convertToColor(3), RGBColor::WHITE);
}

TEST_F(PaletteTest, MultiplePaletteShouldWorkIndependently)
{
    int paletteAddr1 = 0x1234;
    GrayscalePalette palette1(mmu, paletteAddr1);
    int colorMapping1 = 0b00011011;
    mmu.write(paletteAddr1, colorMapping1);

    int paletteAddr2 = 0x1235;
    GrayscalePalette palette2(mmu, paletteAddr2);
    int colorMapping2 = 0b11100100;
    mmu.write(paletteAddr2, colorMapping2);

    ASSERT_EQ(palette1.convertToColor(0), RGBColor::BLACK);
    ASSERT_EQ(palette1.convertToColor(1), RGBColor::DARK_GRAY);
    ASSERT_EQ(palette1.convertToColor(2), RGBColor::LIGHT_GRAY);
    ASSERT_EQ(palette1.convertToColor(3), RGBColor::WHITE);

    ASSERT_EQ(palette2.convertToColor(0), RGBColor::WHITE);
    ASSERT_EQ(palette2.convertToColor(1), RGBColor::LIGHT_GRAY);
    ASSERT_EQ(palette2.convertToColor(2), RGBColor::DARK_GRAY);
    ASSERT_EQ(palette2.convertToColor(3), RGBColor::BLACK);
}