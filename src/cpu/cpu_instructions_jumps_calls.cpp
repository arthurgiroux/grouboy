#include "cpu.hpp"

using namespace utils;

void CPU::jumpConditional(bool condition)
{
    word addr = mmu.readWord(pc);
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
    word addr = mmu.readWord(pc);
    pc = addr;
    lastInstructionTicks = 4;
}

void CPU::jumpToAddrIn16BitsRegister(byte addrMsb, byte addrLsb)
{
    pc = createWordFromBytes(addrMsb, addrLsb);
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
    word addr = mmu.readWord(sp);
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
    interruptsEnabled = true;
}

void CPU::callImmediateSubroutineIfConditionSatisfied(bool condition)
{
    if (condition)
    {
        callImmediateSubroutine();
    }
    else
    {
        pc += 2;
        lastInstructionTicks = 3;
    }
}

void CPU::callImmediateSubroutine()
{
    sp -= 2;
    mmu.writeWord(sp, pc + 2);
    pc = mmu.readWord(pc);
    lastInstructionTicks = 6;
}

void CPU::callRestartRoutine(byte memoryAddr)
{
    sp -= 2;
    mmu.writeWord(sp, pc);
    pc = memoryAddr;
    lastInstructionTicks = 4;
}