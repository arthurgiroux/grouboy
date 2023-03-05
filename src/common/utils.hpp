#ifndef GBEMULATOR_UTILS_HPP
#define GBEMULATOR_UTILS_HPP

#include "types.hpp"
#include <string>

constexpr std::size_t operator""_KiB(unsigned long long int x)
{
    return 1024ULL * x;
}

constexpr std::size_t operator""_MiB(unsigned long long int x)
{
    return 1024_KiB * x;
}

namespace utils
{
/**
 * Create a 16 bits word from an MSB and an LSB.
 *
 * @param msb   the high byte
 * @param lsb   the low byte
 * @return      a 16 bits unsigned value
 */
word createWordFromBytes(byte msb, byte lsb);

/**
 * Get the Most significant Bit of a word (16 bits value).
 *
 * @param value     The word to split
 * @return      The msb of the word
 */
byte getMsbFromWord(word value);

/**
 * Get the Least significant Bit of a word (16 bits value).
 *
 * @param value     The word to split
 * @return      The lsb of the word
 */
byte getLsbFromWord(word value);

/**
 * Format a string using a "printf"-style format.
 * @param fmt   The string to format
 * @param args  The value used for formatting
 * @return      The formatting string
 */
template <typename... Args>
std::string string_format(const char* fmt, Args... args)
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
void increment16BitsValueStoredIn8BitsValues(byte& msb, byte& lsb);

/**
 * Decrement a 16 bits value that is stored in two 8 bits value
 *
 * @param msb   the most significant byte of the 16 bits value
 * @param lsb   the least significant byte of the 16 bits value
 */
void decrement16BitsValueStoredIn8BitsValues(byte& msb, byte& lsb);

/**
 * Returns if the nth bit is set in the given number.
 * @param value         the number to test
 * @param bitPosition   the 0-based position of the bit to test
 * @return true if the bit is set, false otherwise
 */
bool isNthBitSet(int value, int bitPosition);

void setNthBit(int& value, int bitPosition, bool state);

/**
 * Read the binary content of a file.
 *
 * @param filepath  The binary file to read
 * @param out       The data that was read in case of success
 * @return  True if the file was read correctly, false otherwise
 */
bool readBinaryDataFromFile(const std::string& filepath, std::vector<byte>& out);

/**
 * Represents a memory range between two addresses.
 */
class AddressRange
{
  public:
    /**
     * Create a new range of address
     * @param start The start address (inclusive)
     * @param end   The end address (inclusive)
     */
    AddressRange(word start, word end);

    /**
     * Checks if a given address is in the range
     *
     * @param address The address to check
     * @return True is the address in is range, false otherwise
     */
    bool contains(word address) const;

    /**
     * Return the start address
     * @return The start address in range
     */
    word start() const;

    /**
     * Return the end address
     * @return The end address in range
     */
    word end() const;

    /**
     * Convert the given absolute address to a relative address in the range
     *
     * @param address the absolute address to convert
     * @return The relative address in the range
     */
    word relative(word address) const;

  private:
    /**
     * The start address in the range (inclusive)
     */
    word _startAddr;

    /**
     * The end address in the range (inclusive)
     */
    word _endAddr;
};
} // namespace utils

#endif // GBEMULATOR_UTILS_HPP
