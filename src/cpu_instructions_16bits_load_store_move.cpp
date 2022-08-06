#include "cpu.hpp"

using namespace utils;

void CPU::load16BitsValueInRegisters(byte& msbRegister, byte& lsbRegister)
{
	lsbRegister = mmu.read(pc);
	msbRegister = mmu.read(pc + 1);
	pc += 2;
	lastInstructionTicks = 3;
}

void CPU::load16BitsImmediateValueIntoRegister(uint16_t& reg)
{
	reg = mmu.readWord(pc);
	pc += 2;
	lastInstructionTicks = 3;
}

void CPU::load16BitsRegisterAtImmediateAddress(uint16_t reg)
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
	mmu.write(sp, regMsb);
	mmu.write(sp + 1, regLsb);
	lastInstructionTicks = 4;
}