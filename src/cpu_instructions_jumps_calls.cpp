#include "cpu.hpp"

using namespace utils;

void CPU::jumpConditional(bool condition)
{
    uint16_t addr = mmu.readWord(pc);
    pc += 2;
    if (condition)
    {
        pc = addr;
        lastInstructionTicks = 4;
    }
    else
    {
        lastInstructionTicks = 3;
    }
}

void CPU::jump()
{
    uint16_t addr = mmu.readWord(pc);
    pc = addr;
    lastInstructionTicks = 4;
}

void CPU::jumpToAddrIn16BitsRegister(byte addrMsb, byte addrLsb)
{
    pc = createAddrFromHighAndLowBytes(addrMsb, addrLsb);
    lastInstructionTicks = 1;
}

void CPU::jumpRelativeConditional(bool condition)
{
    auto offset = static_cast<int8_t>(mmu.read(pc));
    pc++;
    if (condition)
    {
        pc += offset;
        lastInstructionTicks = 3;
    }
    else
    {
        lastInstructionTicks = 2;
    }
}

void CPU::jumpRelative()
{
    auto offset = static_cast<int8_t>(mmu.read(pc));
    pc++;
    pc += offset;
    lastInstructionTicks = 3;
}


void CPU::returnInstruction()
{
    // We unpop the address of the caller
    // From the stack and set the pc to the value
    uint16_t addr = mmu.readWord(sp);
    sp += 2;
    pc = addr;
    lastInstructionTicks = 4;
}

void CPU::returnInstructionConditional(bool cond)
{
    if (cond)
    {
        returnInstruction();
        lastInstructionTicks = 5;
    }
    else
    {
        lastInstructionTicks = 2;
    }
}

void CPU::returnInstructionAfterInterrupt()
{
    returnInstruction();
    // TODO:
    // Revisit what to do after implementing interrups
    interrupts = false;
}