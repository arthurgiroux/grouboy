#include "utils.hpp"
#include <gtest/gtest.h>

using namespace utils;

void assertIncrement16BitsValueStoredIn8BitsValues(uint16_t value, uint16_t expectedValue) {
	byte msb = getMsbFromWord(value);
	byte lsb = getLsbFromWord(value);
    increment16BitsValueStoredIn8BitsValues(msb, lsb);
	ASSERT_EQ(msb, getMsbFromWord(expectedValue));
    ASSERT_EQ(lsb, getLsbFromWord(expectedValue));
}

void assertDecrement16BitsValueStoredIn8BitsValues(uint16_t value, uint16_t expectedValue) {
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
	uint16_t value = 0x1234;
	ASSERT_EQ(getMsbFromWord(value), 0x12);
}

TEST(UtilsTest, GetLsbFromWordShouldGiveLsbValue)
{
    uint16_t value = 0x1234;
    ASSERT_EQ(getLsbFromWord(value), 0x34);
}

TEST(UtilsTest, CreateAddrFromHighAndLowBytesShouldPutBytesInCorrectPlace)
{
	byte msb = 0x12;
	byte lsb = 0x34;
    uint16_t value = 0x1234;
    ASSERT_EQ(createAddrFromHighAndLowBytes(msb, lsb), value);
}