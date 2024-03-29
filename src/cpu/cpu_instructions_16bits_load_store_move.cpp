#include "cpu.hpp"

using namespace utils;

void CPU::load16BitsValueInRegisters(byte& msbRegister, byte& lsbRegister)
{
    lsbRegister = mmu.read(pc);
    msbRegister = mmu.read(pc + 1);
    pc += 2;
    lastInstructionTicks = 3;
}

void CPU::load16BitsImmediateValueIntoRegister(word& reg)
{
    reg = mmu.readWord(pc);
    pc += 2;
    lastInstructionTicks = 3;
}

void CPU::load16BitsRegisterAtImmediateAddress(word reg)
{
    mmu.writeWord(mmu.readWord(pc), reg);
    pc += 2;
    lastInstructionTicks = 5;
}

void CPU::popMemoryIntoRegisterPair(byte& regMsb, byte& regLsb)
{
    regLsb = mmu.read(sp);
    regMsb = mmu.read(sp + 1);
    sp += 2;
    lastInstructionTicks = 3;
}

void CPU::push16BitsOntoStackPointer(byte regMsb, byte regLsb)
{
    sp -= 2;
    lastInstructionTicks = 4;
    mmu.write(sp, regLsb);
    mmu.write(sp + 1, regMsb);
}

void CPU::load16BitsRegisterAndImmediateOffsetIn16BitsRegister(byte& msbRegister, byte& lsbRegister, word otherReg)
{
    unsetFlag(CpuFlags::ZERO);
    unsetFlag(CpuFlags::SUBSTRACTION);
    sbyte offset = static_cast<sbyte>(mmu.read(pc));
    word newValue = otherReg + offset;
    pc++;
    msbRegister = getMsbFromWord(newValue);
    lsbRegister = getLsbFromWord(newValue);
    setHalfCarryFlag((((otherReg & 0xF) + (offset & 0xF)) & 0x10) == 0x10);
    setCarryFlag((((otherReg & 0xFF) + (offset & 0xFF)) & 0x100) == 0x100);
    lastInstructionTicks = 3;
}

void CPU::loadTwo8BitsRegisterIn16BitsRegister(word& reg, byte msbValue, byte lsbValue)
{
    reg = createWordFromBytes(msbValue, lsbValue);
    lastInstructionTicks = 2;
}