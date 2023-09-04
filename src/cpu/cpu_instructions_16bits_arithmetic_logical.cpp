#include "cpu.hpp"

using namespace utils;

void CPU::addTwo8BitsRegistersToTwo8BitsRegisters(byte& resultRegMsb, byte& resultRegLsb, byte valueRegMsb,
                                                  byte valueRegLsb)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    word origValue = createWordFromBytes(resultRegMsb, resultRegLsb);
    uint32_t result = origValue;
    word addValue = createWordFromBytes(valueRegMsb, valueRegLsb);
    result += addValue;
    setCarryFlag(result > 0xFFFF);
    setHalfCarryFlag(((origValue & 0xFFF) + (addValue & 0xFFF)) > 0xFFF);
    resultRegMsb = getMsbFromWord(static_cast<word>(result));
    resultRegLsb = getLsbFromWord(static_cast<word>(result));
    lastInstructionTicks = 2;
}

void CPU::decrementRegistersValue(byte& msgRegister, byte& lsbRegister)
{
    word xy = createWordFromBytes(msgRegister, lsbRegister);
    xy--;
    msgRegister = getMsbFromWord(xy);
    lsbRegister = getLsbFromWord(xy);
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
    word initialValue = sp;
    sbyte addValue = static_cast<sbyte>(mmu.read(pc));
    sp += addValue;
    pc++;
    setCarryFlag((((initialValue & 0xFF) + (addValue & 0xFF)) & 0x100) == 0x100);
    setHalfCarryFlag((((initialValue & 0xF) + (addValue & 0xF)) & 0x10) == 0x10);
    lastInstructionTicks = 4;
}