#include "graphics/color_palette_memory_mapper.hpp"
#include "graphics/palette.hpp"
#include <gtest/gtest.h>

class ColorPaletteMemoryMapperTest : public ::testing::Test
{
  protected:
    ColorPaletteMemoryMapper colorPaletteMemoryMapper;
};

TEST_F(ColorPaletteMemoryMapperTest, SetAddressShouldSetTheAddress)
{
    byte addr = 0x34;
    colorPaletteMemoryMapper.setAddress(addr);
    EXPECT_EQ(colorPaletteMemoryMapper.getAddress(), addr);
}

TEST_F(ColorPaletteMemoryMapperTest, SetAddressAutoIncrementShouldSetAutoIncrementFlag)
{
    colorPaletteMemoryMapper.enableAddressAutoIncrement(true);
    EXPECT_TRUE(colorPaletteMemoryMapper.isAddressAutoIncrementEnabled());

    colorPaletteMemoryMapper.enableAddressAutoIncrement(false);
    EXPECT_FALSE(colorPaletteMemoryMapper.isAddressAutoIncrementEnabled());
}

TEST_F(ColorPaletteMemoryMapperTest, WriteColorShouldWriteToCorrectPaletteAndColor)
{
    // We write to the first byte color of the first palette
    byte paletteAddr = 0x00;
    byte color = 0xD3;
    colorPaletteMemoryMapper.setAddress(paletteAddr);
    colorPaletteMemoryMapper.writeColor(color);
    ASSERT_EQ(colorPaletteMemoryMapper.readColor(), color);

    int expectedRed = 156;
    EXPECT_EQ(colorPaletteMemoryMapper.getColorPalette(0).getColorForId(0).getRed(), expectedRed);
    int expectedGreen = 49;
    EXPECT_EQ(colorPaletteMemoryMapper.getColorPalette(0).getColorForId(0).getGreen(), expectedGreen);
    int expectedBlue = 0;
    EXPECT_EQ(colorPaletteMemoryMapper.getColorPalette(0).getColorForId(0).getBlue(), expectedBlue);

    // Write to second byte
    paletteAddr = 0x01;
    color = 0xA9;
    colorPaletteMemoryMapper.setAddress(paletteAddr);
    colorPaletteMemoryMapper.writeColor(color);
    ASSERT_EQ(colorPaletteMemoryMapper.readColor(), color);

    expectedRed = 156;
    EXPECT_EQ(colorPaletteMemoryMapper.getColorPalette(0).getColorForId(0).getRed(), expectedRed);
    expectedGreen = 115;
    EXPECT_EQ(colorPaletteMemoryMapper.getColorPalette(0).getColorForId(0).getGreen(), expectedGreen);
    expectedBlue = 82;
    EXPECT_EQ(colorPaletteMemoryMapper.getColorPalette(0).getColorForId(0).getBlue(), expectedBlue);
}

TEST_F(ColorPaletteMemoryMapperTest, AutoAddressIncrementShouldIncreaseAfterWrite)
{
    // We write to the first byte color of the first palette
    byte paletteAddr = 0x00;
    byte color = 0xD3;
    colorPaletteMemoryMapper.enableAddressAutoIncrement(true);
    colorPaletteMemoryMapper.setAddress(paletteAddr);
    colorPaletteMemoryMapper.readColor();
    colorPaletteMemoryMapper.writeColor(color);
    ASSERT_EQ(colorPaletteMemoryMapper.getAddress(), paletteAddr + 1);
}

TEST_F(ColorPaletteMemoryMapperTest, AutoAddressIncrementShouldNotIncreaseAfterRead)
{
    // We write to the first byte color of the first palette
    byte paletteAddr = 0x00;
    colorPaletteMemoryMapper.setAddress(paletteAddr);
    ASSERT_EQ(colorPaletteMemoryMapper.readColor(), 0);
    ASSERT_EQ(colorPaletteMemoryMapper.getAddress(), paletteAddr);
}