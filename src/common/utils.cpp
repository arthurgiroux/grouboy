#include "utils.hpp"
#include <fstream>
#include <vector>

namespace utils
{

word createWordFromBytes(byte msb, byte lsb)
{
    return (msb << 8u) | lsb;
}

byte getMsbFromWord(word value)
{
    return value >> 8;
}

byte getLsbFromWord(word value)
{
    return static_cast<byte>(value);
}

void increment16BitsValueStoredIn8BitsValues(byte& msb, byte& lsb)
{
    word value = createWordFromBytes(msb, lsb);
    value++;
    msb = getMsbFromWord(value);
    lsb = getLsbFromWord(value);
}

void decrement16BitsValueStoredIn8BitsValues(byte& msb, byte& lsb)
{
    word value = createWordFromBytes(msb, lsb);
    value--;
    msb = getMsbFromWord(value);
    lsb = getLsbFromWord(value);
}

bool isNthBitSet(int value, int bitPosition)
{
    return (value & (1 << bitPosition)) > 0;
}

void setNthBit(int& value, int bitPosition, bool state)
{
    if (state)
    {
        value |= (1 << bitPosition);
    }
    else
    {
        value &= ~(1 << bitPosition);
    }
}

bool readBinaryDataFromFile(const std::string& filepath, std::vector<byte>& out)
{
    std::ifstream input(filepath, std::ios::binary);
    if (!input.good())
    {
        return false;
    }

    out = std::vector<byte>((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    input.close();

    return true;
}

byte convertFrom5BitsTo8Bits(byte value)
{
    int MAX_VALUE_5BIT = 31;
    float MAX_VALUE_8BIT = 255.f;
    return static_cast<byte>(round(((value & 0x1F) * MAX_VALUE_8BIT) / MAX_VALUE_5BIT));
}

byte convertFrom8BitsTo5Bits(byte value)
{
    int MAX_VALUE_5BIT = 31;
    float MAX_VALUE_8BIT = 255.f;
    return static_cast<byte>(round((value * MAX_VALUE_5BIT) / MAX_VALUE_8BIT));
}

AddressRange::AddressRange(word start, word end) : _startAddr(start), _endAddr(end)
{
}

bool AddressRange::contains(word address) const
{
    return address >= _startAddr && address <= _endAddr;
}

word AddressRange::start() const
{
    return _startAddr;
}

word AddressRange::end() const
{
    return _endAddr;
}

word AddressRange::relative(word address) const
{
    return address - _startAddr;
}
} // namespace utils
