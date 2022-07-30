#ifndef GBEMULATOR_UTILS_HPP
#define GBEMULATOR_UTILS_HPP

#include <string>
#include "types.hpp"

namespace utils
{

/**
 * Create a 16 bits address from an MSB and an LSB.
 *
 * @param msb   the high byte
 * @param lsb   the low byte
 * @return      a 16 bits unsigned address
 */
static uint16_t createAddrFromHighAndLowBytes(byte msb, byte lsb)
{
	return (msb << 8u) | lsb;
}

/**
 * Get the Most significant Bit of a word (16 bits value).
 *
 * @param value     The word to split
 * @return      The msb of the word
 */
static byte getMsbFromWord(uint16_t value)
{
    return value >> 8;
}

/**
 * Get the Least significant Bit of a word (16 bits value).
 *
 * @param value     The word to split
 * @return      The lsb of the word
 */
static byte getLsbFromWord(uint16_t value)
{
    return value;
}

/**
 * Format a string using a "printf"-style format.
 * @param fmt   The string to format
 * @param args  The value used for formatting
 * @return      The formatting string
 */
template <typename... Args>
static std::string string_format(const char* fmt, Args... args)
{
	size_t size = snprintf(nullptr, 0, fmt, args...);
	std::string buf;
	buf.reserve(size + 1);
	buf.resize(size);
	snprintf(&buf[0], size + 1, fmt, args...);
	return buf;
}

/**
 * Increment a 16 bits value that is stored in two 8 bits value
 *
 * @param msb   the most significant byte of the 16 bits value
 * @param lsb   the least significant byte of the 16 bits value
 */
static void increment16BitsValueStoredIn8BitsValues(byte& msb, byte& lsb)
{
	uint16_t value = (msb << 8) | lsb;
	value++;
	msb = (value >> 8);
	lsb = value & 0xFF;
}

/**
 * Decrement a 16 bits value that is stored in two 8 bits value
 *
 * @param msb   the most significant byte of the 16 bits value
 * @param lsb   the least significant byte of the 16 bits value
 */
static void decrement16BitsValueStoredIn8BitsValues(byte& msb, byte& lsb)
{
	uint16_t value = (msb << 8) | lsb;
	value--;
	msb = (value >> 8);
	lsb = value & 0xFF;
}
} // namespace utils

#endif // GBEMULATOR_UTILS_HPP
