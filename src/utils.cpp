#include "utils.hpp"

namespace utils
{

uint16_t createAddrFromHighAndLowBytes(byte msb, byte lsb)
{
	return (msb << 8u) | lsb;
}

byte getMsbFromWord(uint16_t value)
{
	return value >> 8;
}

byte getLsbFromWord(uint16_t value)
{
	return static_cast<byte>(value);
}

void increment16BitsValueStoredIn8BitsValues(byte& msb, byte& lsb)
{
	uint16_t value = (msb << 8) | lsb;
	value++;
	msb = (value >> 8);
	lsb = value & 0xFF;
}

void decrement16BitsValueStoredIn8BitsValues(byte& msb, byte& lsb)
{
	uint16_t value = (msb << 8) | lsb;
	value--;
	msb = (value >> 8);
	lsb = value & 0xFF;
}

bool isNthBitSet(int value, int bitPosition)
{
    return (value & (1 << bitPosition)) > 0;
}

void setNthBit(int& value, int bitPosition, bool state)
{
	if (state) {
        value |= (1 << bitPosition);
	}
	else {
        value &= ~(1 << bitPosition);
	}
}
} // namespace utils
