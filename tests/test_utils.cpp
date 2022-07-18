#include "utils.hpp"
#include <gtest/gtest.h>

using namespace utils;

void testIncrement16BitsValueStoredIn8BitsValues(uint16_t value) {
    uint16_t expectedValue = value + 1;
	byte msb = value >> 8;
	byte lsb = value & 0xFF;
    increment16BitsValueStoredIn8BitsValues(msb, lsb);
	ASSERT_EQ(msb, expectedValue >> 8);
    ASSERT_EQ(lsb, expectedValue & 0xFF);
}

void testDecrement16BitsValueStoredIn8BitsValues(uint16_t value) {
    uint16_t expectedValue = value - 1;
    byte msb = value >> 8;
    byte lsb = value & 0xFF;
    decrement16BitsValueStoredIn8BitsValues(msb, lsb);
    ASSERT_EQ(msb, expectedValue >> 8);
    ASSERT_EQ(lsb, expectedValue & 0xFF);
}

TEST(UtilsTest, Increment16BitsValueStoredIn8BitsValues)
{
    testIncrement16BitsValueStoredIn8BitsValues(0);
    testIncrement16BitsValueStoredIn8BitsValues(200);
    testIncrement16BitsValueStoredIn8BitsValues(UINT16_MAX);
}

TEST(UtilsTest, Decrement16BitsValueStoredIn8BitsValues)
{
    testDecrement16BitsValueStoredIn8BitsValues(0);
    testDecrement16BitsValueStoredIn8BitsValues(200);
    testDecrement16BitsValueStoredIn8BitsValues(UINT16_MAX);
}
