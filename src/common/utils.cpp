#include "utils.hpp"

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
} // namespace utils
