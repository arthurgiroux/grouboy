#include "cpu.hpp"

using namespace utils;

void CPU::addTwo8BitsRegistersToTwo8BitsRegisters(byte& resultRegMsb, byte& resultRegLsb, byte valueRegMsb,
                                                  byte valueRegLsb)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    uint16_t origValue = (resultRegMsb << 8) | resultRegLsb;
    uint32_t result = origValue;
    result += (valueRegMsb << 8) | valueRegLsb;
    setCarryFlag(result > 0xFFFF);
    setHalfCarryFlag(origValue <= 0xFFF && result > 0xFFF);
    resultRegMsb = ((result & 0xFF00) >> 8);
    resultRegLsb = (result & 0xFF);
    lastInstructionTicks = 2;
}

void CPU::decrementRegistersValue(byte& msgRegister, byte& lsbRegister)
{
    uint16_t xy = (msgRegister << 8) | lsbRegister;
    xy--;
    msgRegister = (xy >> 8);
    lsbRegister = (xy & 0x00FF);
    lastInstructionTicks = 2;
}

void CPU::incrementRegistersValue(byte& msbRegister, byte& lsbRegister)
{
    lsbRegister++;
    // If we overflowed the LSB then we increment the MSB
    if (lsbRegister == 0)
    {
        msbRegister++;
    }
    lastInstructionTicks = 2;
}