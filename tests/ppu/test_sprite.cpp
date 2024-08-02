#include "graphics/sprite.hpp"
#include <gtest/gtest.h>

class SpriteTest : public ::testing::Test
{
  protected:
    void assertThatSettingYValueInMemoryGivesCorrectYScreenCoordinate(int spriteId, int yValue, int expectedResult)
    {
        Sprite sprite(oam, spriteId);
        int startAddr = BYTE_PER_TILE * spriteId;
        int yCoordinateMemoryOffset = 0;
        oam.write(startAddr + yCoordinateMemoryOffset, yValue);
        ASSERT_EQ(sprite.getYPositionOnScreen(), expectedResult);
    }

    void assertThatSettingXValueInMemoryGivesCorrectYScreenCoordinate(int spriteId, int xValue, int expectedResult)
    {
        Sprite sprite(oam, spriteId);
        int startAddr = BYTE_PER_TILE * spriteId;
        int xCoordinateMemoryOffset = 1;
        oam.write(startAddr + xCoordinateMemoryOffset, xValue);
        ASSERT_EQ(sprite.getXPositionOnScreen(), expectedResult);
    }

    void setDataFlagInMemory(int spriteId, int dataFlagValue)
    {
        int startAddr = BYTE_PER_TILE * spriteId;
        int dataFlagMemoryOffset = 3;
        oam.write(startAddr + dataFlagMemoryOffset, dataFlagValue);
    }

    OAM oam;
    static const int BYTE_PER_TILE = 4;
};

TEST_F(SpriteTest, ConstructorSetsInformationCorrectly)
{
    int expectedId = 20;
    Sprite sprite(oam, expectedId);
    ASSERT_EQ(sprite.getId(), expectedId);
}

TEST_F(SpriteTest, getYPositionOnScreenReadsValueFromMemory)
{
    assertThatSettingYValueInMemoryGivesCorrectYScreenCoordinate(20, 60, 44);
}

TEST_F(SpriteTest, getYPositionOnScreenReadsValueFromMemoryAndCanBeNegative)
{
    assertThatSettingYValueInMemoryGivesCorrectYScreenCoordinate(20, 0, -16);
}

TEST_F(SpriteTest, getXPositionOnScreenReadsValueFromMemory)
{
    assertThatSettingXValueInMemoryGivesCorrectYScreenCoordinate(20, 60, 52);
}

TEST_F(SpriteTest, getXPositionOnScreenReadsValueFromMemoryAndCanBeNegative)
{
    assertThatSettingXValueInMemoryGivesCorrectYScreenCoordinate(20, 0, -8);
}

TEST_F(SpriteTest, GetTileIdReadsIdFromMemory)
{
    int spriteId = 20;
    int tileId = 42;
    Sprite sprite(oam, spriteId);
    int startAddr = BYTE_PER_TILE * spriteId;
    int tileIdCoordinateMemoryOffset = 2;
    oam.write(startAddr + tileIdCoordinateMemoryOffset, tileId);
    ASSERT_EQ(sprite.getTileId(), tileId);
}

TEST_F(SpriteTest, isFlippedVerticallyShouldReturnFalseIfFlagIsUnsetInMemory)
{
    int spriteId = 20;
    Sprite sprite(oam, spriteId);
    setDataFlagInMemory(spriteId, 0x00);
    ASSERT_FALSE(sprite.isFlippedVertically());
}

TEST_F(SpriteTest, isFlippedVerticallyShouldReturnTrueIfFlagIsSetInMemory)
{
    int spriteId = 20;
    Sprite sprite(oam, spriteId);
    setDataFlagInMemory(spriteId, 0b01000000);
    ASSERT_TRUE(sprite.isFlippedVertically());
}

TEST_F(SpriteTest, isFlippedHorizontallyShouldReturnFalseIfFlagIsUnsetInMemory)
{
    int spriteId = 20;
    Sprite sprite(oam, spriteId);
    setDataFlagInMemory(spriteId, 0x00);
    ASSERT_FALSE(sprite.isFlippedHorizontally());
}

TEST_F(SpriteTest, isFlippedHorizontallyShouldReturnTrueIfFlagIsSetInMemory)
{
    int spriteId = 20;
    Sprite sprite(oam, spriteId);
    setDataFlagInMemory(spriteId, 0b00100000);
    ASSERT_TRUE(sprite.isFlippedHorizontally());
}

TEST_F(SpriteTest, getPaletteIdShouldReturnThePaletteIdSetInMemory)
{
    int spriteId = 20;
    Sprite sprite(oam, spriteId);
    setDataFlagInMemory(spriteId, 0b00010000);
    ASSERT_EQ(sprite.getGrayscalePaletteId(), 1);
}

TEST_F(SpriteTest, getBankIdShouldReturnTheBankIdSetInMemory)
{
    int spriteId = 20;
    Sprite sprite(oam, spriteId);
    setDataFlagInMemory(spriteId, 0b00001000);
    ASSERT_EQ(sprite.getBankId(), 1);
}