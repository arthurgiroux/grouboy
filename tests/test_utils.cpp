#include "common/utils.hpp"
#include <gtest/gtest.h>

using namespace utils;

void assertIncrement16BitsValueStoredIn8BitsValues(word value, word expectedValue)
{
    byte msb = getMsbFromWord(value);
    byte lsb = getLsbFromWord(value);
    increment16BitsValueStoredIn8BitsValues(msb, lsb);
    ASSERT_EQ(msb, getMsbFromWord(expectedValue));
    ASSERT_EQ(lsb, getLsbFromWord(expectedValue));
}

void assertDecrement16BitsValueStoredIn8BitsValues(word value, word expectedValue)
{
    byte msb = getMsbFromWord(value);
    byte lsb = getLsbFromWord(value);
    decrement16BitsValueStoredIn8BitsValues(msb, lsb);
    ASSERT_EQ(msb, getMsbFromWord(expectedValue));
    ASSERT_EQ(lsb, getLsbFromWord(expectedValue));
}

TEST(UtilsTest, Increment16BitsValueStoredIn8BitsValuesForValue0ShouldGive1)
{
    assertIncrement16BitsValueStoredIn8BitsValues(0, 1);
}

TEST(UtilsTest, Increment16BitsValueStoredIn8BitsValuesForValue200ShouldGive201)
{
    assertIncrement16BitsValueStoredIn8BitsValues(200, 201);
}

TEST(UtilsTest, Increment16BitsValueStoredIn8BitsValuesForValueMaxShouldGive0)
{
    assertIncrement16BitsValueStoredIn8BitsValues(UINT16_MAX, 0);
}

TEST(UtilsTest, Decrement16BitsValueStoredIn8BitsValuesForValue0ShouldGiveMax)
{
    assertDecrement16BitsValueStoredIn8BitsValues(0, UINT16_MAX);
}

TEST(UtilsTest, Decrement16BitsValueStoredIn8BitsValuesForValue200ShouldGive199)
{
    assertDecrement16BitsValueStoredIn8BitsValues(200, 199);
}

TEST(UtilsTest, Decrement16BitsValueStoredIn8BitsValuesForValueMaxShouldGiveMaxMinus1)
{
    assertDecrement16BitsValueStoredIn8BitsValues(UINT16_MAX, UINT16_MAX - 1);
}

TEST(UtilsTest, GetMsbFromWordShouldGiveMsbValue)
{
    word value = 0x1234;
    ASSERT_EQ(getMsbFromWord(value), 0x12);
}

TEST(UtilsTest, GetLsbFromWordShouldGiveLsbValue)
{
    word value = 0x1234;
    ASSERT_EQ(getLsbFromWord(value), 0x34);
}

TEST(UtilsTest, CreateAddrFromHighAndLowBytesShouldPutBytesInCorrectPlace)
{
    byte msb = 0x12;
    byte lsb = 0x34;
    word value = 0x1234;
    ASSERT_EQ(createWordFromBytes(msb, lsb), value);
}

TEST(UtilsTest, TestBitForValue0ShouldReturnFalseForAllBits)
{
    for (int i = 0; i < 16; ++i)
    {
        ASSERT_FALSE(utils::isNthBitSet(0, i));
    }
}

TEST(UtilsTest, TestBitForPowerOfTwoShouldReturnTrue)
{
    int value = 1;
    for (int i = 0; i < 16; ++i)
    {
        ASSERT_TRUE(utils::isNthBitSet(value, i));
        // Setting the bit by going to the next power of 2.
        value *= 2;
    }
}

TEST(UtilsTest, TestBitForGivenValueShouldGivesExpectedResult)
{
    int value = 0b1011010001011010;
    ASSERT_EQ(utils::isNthBitSet(value, 0), false);
    ASSERT_EQ(utils::isNthBitSet(value, 1), true);
    ASSERT_EQ(utils::isNthBitSet(value, 2), false);
    ASSERT_EQ(utils::isNthBitSet(value, 3), true);
    ASSERT_EQ(utils::isNthBitSet(value, 4), true);
    ASSERT_EQ(utils::isNthBitSet(value, 5), false);
    ASSERT_EQ(utils::isNthBitSet(value, 6), true);
    ASSERT_EQ(utils::isNthBitSet(value, 7), false);
    ASSERT_EQ(utils::isNthBitSet(value, 8), false);
    ASSERT_EQ(utils::isNthBitSet(value, 9), false);
    ASSERT_EQ(utils::isNthBitSet(value, 10), true);
    ASSERT_EQ(utils::isNthBitSet(value, 11), false);
    ASSERT_EQ(utils::isNthBitSet(value, 12), true);
    ASSERT_EQ(utils::isNthBitSet(value, 13), true);
    ASSERT_EQ(utils::isNthBitSet(value, 14), false);
    ASSERT_EQ(utils::isNthBitSet(value, 15), true);
}

TEST(UtilsTest, AddressRangeShouldSetStartAndEndAddrCorrectly)
{
    word startAddr = 0x1234;
    word endAddr = 0x4321;
    auto range = AddressRange(startAddr, endAddr);
    ASSERT_EQ(range.start(), startAddr);
    ASSERT_EQ(range.end(), endAddr);
}

TEST(UtilsTest, AddressRangeContainsShouldReturnTrueIfAddressBetweenStartAndEnd)
{
    word startAddr = 0x1234;
    word endAddr = 0x1236;
    auto range = AddressRange(startAddr, endAddr);
    ASSERT_TRUE(range.contains(startAddr));
    ASSERT_TRUE(range.contains(endAddr));
    ASSERT_TRUE(range.contains(0x1235));
}

TEST(UtilsTest, AddressRangeContainsShouldReturnFalseIfAddressOutOfRange)
{
    word startAddr = 0x1234;
    word endAddr = 0x1236;
    auto range = AddressRange(startAddr, endAddr);
    ASSERT_FALSE(range.contains(startAddr - 1));
    ASSERT_FALSE(range.contains(endAddr + 1));
}

TEST(UtilsTest, AddressRangeRelativeShouldReturnTheAddressRelativeToTheStartAddress)
{
    word startAddr = 0x1234;
    word endAddr = 0x1236;
    auto range = AddressRange(startAddr, endAddr);
    ASSERT_EQ(range.relative(startAddr), 0);
    ASSERT_EQ(range.relative(0x1235), 1);
    ASSERT_EQ(range.relative(endAddr), 2);
}