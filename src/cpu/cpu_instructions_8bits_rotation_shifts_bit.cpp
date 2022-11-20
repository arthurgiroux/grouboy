#include "cpu.hpp"

using namespace utils;

void CPU::rotateRegisterLeftCircular(byte& reg)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    unsetFlag(CpuFlags::ZERO);
    unsetFlag(CpuFlags::HALF_CARRY);

    /* Set the carry flag to the highest bit of X */
    setCarryFlag((reg & 0x80) > 0);
    /* Rotate the value circularly */
    reg = (reg << 1) | (reg >> 7);
    lastInstructionTicks = 1;
}

void CPU::rotateValueInMemoryLeftCircular(byte addrMsb, byte addrLsb)
{
    word addr = createWordFromBytes(addrMsb, addrLsb);
    byte value = mmu.read(addr);
    rotateRegisterLeftCircular(value);
    mmu.write(addr, value);
    changeZeroValueFlag(value);
    lastInstructionTicks = 4;
}

void CPU::rotateRegisterRightCircular(byte& reg)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    unsetFlag(CpuFlags::ZERO);
    unsetFlag(CpuFlags::HALF_CARRY);

    /* Set the carry flag to the lowest bit of X */
    setCarryFlag((reg & 0x01) > 0);
    /* Rotate the value circularly */
    reg = (reg << 7) | (reg >> 1);
    lastInstructionTicks = 1;
}

void CPU::rotateValueInMemoryRightCircular(byte addrMsb, byte addrLsb)
{
    word addr = createWordFromBytes(addrMsb, addrLsb);
    byte value = mmu.read(addr);
    rotateRegisterRightCircular(value);
    mmu.write(addr, value);
    changeZeroValueFlag(value);
    lastInstructionTicks = 4;
}

void CPU::rotateRegisterLeft(byte& reg)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    unsetFlag(CpuFlags::ZERO);
    unsetFlag(CpuFlags::HALF_CARRY);

    byte currentCarry = isFlagSet(CpuFlags::CARRY);
    /* Set the carry flag to the highest bit of reg */
    setCarryFlag((reg & 0x80) > 0);
    /* Rotate the accumulator and set the last bit to the original carry flag */
    reg = (reg << 1) | currentCarry;
    lastInstructionTicks = 1;
}

void CPU::rotateRegisterLeftExtended(byte& reg)
{
    rotateRegisterLeft(reg);
    changeZeroValueFlag(reg);
    lastInstructionTicks = 2;
}

void CPU::rotateValueInMemoryLeft(byte addrMsb, byte addrLsb)
{
    word addr = createWordFromBytes(addrMsb, addrLsb);
    byte value = mmu.read(addr);
    rotateRegisterLeftExtended(value);
    mmu.write(addr, value);
    lastInstructionTicks = 4;
}

void CPU::rotateRegisterRight(byte& reg)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    unsetFlag(CpuFlags::ZERO);
    unsetFlag(CpuFlags::HALF_CARRY);

    byte currentCarry = isFlagSet(CpuFlags::CARRY);
    /* Set the carry flag to the lowest bit of X */
    setCarryFlag((reg & 0x01) > 0);
    /* Rotate the accumulator and set the highest bit to the original carry flag */
    reg = (currentCarry << 7) | (reg >> 1);
    lastInstructionTicks = 1;
}

void CPU::rotateRegisterRightExtended(byte& reg)
{
    rotateRegisterRight(reg);
    changeZeroValueFlag(reg);
    lastInstructionTicks = 2;
}

void CPU::rotateValueInMemoryRight(byte addrMsb, byte addrLsb)
{
    word addr = createWordFromBytes(addrMsb, addrLsb);
    byte value = mmu.read(addr);
    rotateRegisterRightExtended(value);
    mmu.write(addr, value);
    lastInstructionTicks = 4;
}

void CPU::rotateRegisterLeftCircularExtended(byte& reg)
{
    rotateRegisterLeftCircular(reg);
    changeZeroValueFlag(reg);
    lastInstructionTicks = 2;
}

void CPU::rotateRegisterRightCircularExtended(byte& reg)
{
    rotateRegisterRightCircular(reg);
    changeZeroValueFlag(reg);
    lastInstructionTicks = 2;
}

void CPU::shiftLeftArithmeticRegister(byte& reg)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    unsetFlag(CpuFlags::ZERO);
    unsetFlag(CpuFlags::HALF_CARRY);
    setCarryFlag((reg & 0x80) > 0);
    reg = (reg << 1);
    changeZeroValueFlag(reg);
    lastInstructionTicks = 2;
}

void CPU::shiftLeftArithmeticMemory(word memoryAddr)
{
    byte value = mmu.read(memoryAddr);
    shiftLeftArithmeticRegister(value);
    mmu.write(memoryAddr, value);
    lastInstructionTicks = 4;
}

void CPU::shiftRightArithmeticRegister(byte& reg)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    unsetFlag(CpuFlags::HALF_CARRY);
    setCarryFlag((reg & 0x01) > 0);
    reg = ((reg & 0x80) | (reg >> 1));
    changeZeroValueFlag(reg);
    lastInstructionTicks = 2;
}

void CPU::shiftRightArithmeticMemory(word memoryAddr)
{
    byte value = mmu.read(memoryAddr);
    shiftRightArithmeticRegister(value);
    mmu.write(memoryAddr, value);
    lastInstructionTicks = 4;
}

void CPU::shiftRightLogicalRegister(byte& reg)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    unsetFlag(CpuFlags::HALF_CARRY);
    setCarryFlag((reg & 0x01) > 0);
    reg = (reg >> 1);
    changeZeroValueFlag(reg);
    lastInstructionTicks = 2;
}

void CPU::shiftRightLogicalMemory(word memoryAddr)
{
    byte value = mmu.read(memoryAddr);
    shiftRightLogicalRegister(value);
    mmu.write(memoryAddr, value);
    lastInstructionTicks = 4;
}

void CPU::swapNibblesInRegister(byte& reg)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    unsetFlag(CpuFlags::HALF_CARRY);
    unsetFlag(CpuFlags::CARRY);

    // swap the two nibbles
    reg = ((reg >> 4) | (reg << 4));
    changeZeroValueFlag(reg);
    lastInstructionTicks = 2;
}

void CPU::swapNibblesInMemory(word memoryAddr)
{
    byte value = mmu.read(memoryAddr);
    swapNibblesInRegister(value);
    mmu.write(memoryAddr, value);
    lastInstructionTicks = 4;
}

void CPU::isBitSetForValue(byte value, byte bitPosition)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    setFlag(CpuFlags::HALF_CARRY);
    // If the value of the bit is not set then we set the flag zero
    setFlagIfTrue(!utils::isNthBitSet(value, bitPosition), CpuFlags::ZERO);
    lastInstructionTicks = 2;
}

void CPU::isBitSetInMemory(word memoryAddr, byte bitPosition)
{
    isBitSetForValue(mmu.read(memoryAddr), bitPosition);
    lastInstructionTicks = 4;
}

void CPU::resetBitForValue(byte& value, byte bitPosition)
{
    value &= ~(1 << bitPosition);
    lastInstructionTicks = 2;
}

void CPU::resetBitInMemory(word memoryAddr, byte bitPosition)
{
    byte value = mmu.read(memoryAddr);
    resetBitForValue(value, bitPosition);
    mmu.write(memoryAddr, value);
    lastInstructionTicks = 4;
}

void CPU::setBitForValue(byte& value, byte bitPosition)
{
    value |= (1 << bitPosition);
    lastInstructionTicks = 2;
}

void CPU::setBitInMemory(word memoryAddr, byte bitPosition)
{
    byte value = mmu.read(memoryAddr);
    setBitForValue(value, bitPosition);
    mmu.write(memoryAddr, value);
    lastInstructionTicks = 4;
}