#include "graphics/color_palette_memory_mapper.hpp"
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
    byte color = 0xAB;
    colorPaletteMemoryMapper.setAddress(paletteAddr);
    colorPaletteMemoryMapper.writeColor(color);
    ASSERT_EQ(colorPaletteMemoryMapper.readColor(), color);

    EXPECT_EQ(colorPaletteMemoryMapper.getColorPalette(0).getColorForId(0).;
}