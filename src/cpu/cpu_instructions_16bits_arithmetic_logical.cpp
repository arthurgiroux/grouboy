#include "cpu.hpp"

using namespace utils;

void CPU::addTwo8BitsRegistersToTwo8BitsRegisters(byte& resultRegMsb, byte& resultRegLsb, byte valueRegMsb,
                                                  byte valueRegLsb)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    uint16_t origValue = (resultRegMsb << 8) | resultRegLsb;
    uint32_t result = origValue;
    uint16_t addValue = (valueRegMsb << 8) | valueRegLsb;
    result += addValue;
    setCarryFlag(result > 0xFFFF);
    setHalfCarryFlag(((origValue & 0xFFF) + (addValue & 0xFFF)) > 0xFFF);
    resultRegMsb = getMsbFromWord(result);
    resultRegLsb = getLsbFromWord(result);
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

void CPU::addImmediateValueToStackPointer()
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    unsetFlag(CpuFlags::ZERO);
    uint16_t initialValue = sp;
    sbyte addValue = static_cast<sbyte>(mmu.read(pc));
    sp += addValue;
    pc++;
    setCarryFlag((((initialValue & 0xFF) + (addValue & 0xFF)) & 0x100) == 0x100);
    setHalfCarryFlag((((initialValue & 0xF) + (addValue & 0xF)) & 0x10) == 0x10);
    lastInstructionTicks = 4;
}