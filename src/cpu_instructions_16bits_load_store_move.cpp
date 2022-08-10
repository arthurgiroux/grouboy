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

void CPU::load16BitsRegisterAndImmediateOffsetIn16BitsRegister(byte& msbRegister, byte& lsbRegister, uint16_t otherReg)
{
	unsetFlag(CpuFlags::ZERO);
	unsetFlag(CpuFlags::SUBSTRACTION);
	int8_t offset = static_cast<int8_t>(mmu.read(pc));
	uint16_t newValue = otherReg + offset;
	pc++;
	msbRegister = getMsbFromWord(newValue);
	lsbRegister = getLsbFromWord(newValue);
	// TODO: Check if carry and half carry should apply to + operation or generic load operation
	setCarryFlag((offset < 0 && newValue > otherReg) || (offset > 0 && newValue < otherReg));
	lastInstructionTicks = 3;
}

void CPU::loadTwo8BitsRegisterIn16BitsRegister(uint16_t& reg, byte msbValue, byte lsbValue)
{
	reg = (msbValue << 8) | lsbValue;
	lastInstructionTicks = 2;
}