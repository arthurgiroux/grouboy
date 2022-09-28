#include "cpu.hpp"

using namespace utils;

void CPU::loadValueToMemoryAtAddr(byte addrMsb, byte addrLsb, byte value)
{
	mmu.write(createAddrFromHighAndLowBytes(addrMsb, addrLsb), value);
	lastInstructionTicks = 2;
}

void CPU::load8BitsImmediateValueAtMemoryAddress(byte addrMsb, byte addrLsb)
{
	byte value = mmu.read(pc);
	pc++;
	mmu.write(createAddrFromHighAndLowBytes(addrMsb, addrLsb), value);
	lastInstructionTicks = 3;
}

void CPU::loadImmediateValueInRegister(byte& reg)
{
	reg = mmu.read(pc);
	pc++;
	lastInstructionTicks = 2;
}

void CPU::loadValueToMemoryAndIncrementAddr(byte& addrMsb, byte& addrLsb, byte value)
{
	mmu.write(createAddrFromHighAndLowBytes(addrMsb, addrLsb), value);
	increment16BitsValueStoredIn8BitsValues(addrMsb, addrLsb);
	lastInstructionTicks = 2;
}

void CPU::loadValueFromMemoryAndIncrementAddr(byte& reg, byte& addrMsb, byte& addrLsb)
{
	reg = mmu.read(createAddrFromHighAndLowBytes(addrMsb, addrLsb));
	increment16BitsValueStoredIn8BitsValues(addrMsb, addrLsb);
	lastInstructionTicks = 2;
}

void CPU::loadValueFromMemoryAndDecreaseAddr(byte& reg, byte& addrMsb, byte& addrLsb)
{
	reg = mmu.read(createAddrFromHighAndLowBytes(addrMsb, addrLsb));
	decrement16BitsValueStoredIn8BitsValues(addrMsb, addrLsb);
	lastInstructionTicks = 2;
}

void CPU::loadValueToMemoryAndDecreaseAddr(byte& addrMsb, byte& addrLsb, byte value)
{
	mmu.write(createAddrFromHighAndLowBytes(addrMsb, addrLsb), value);
	decrement16BitsValueStoredIn8BitsValues(addrMsb, addrLsb);
	lastInstructionTicks = 2;
}

void CPU::loadValueFromMemoryInto8BitsRegister(byte& reg, byte addrMsb, byte addrLsb)
{
	reg = mmu.read(createAddrFromHighAndLowBytes(addrMsb, addrLsb));
	lastInstructionTicks = 2;
}

void CPU::load8BitsValueInRegister(byte& reg, byte value)
{
	reg = value;
	lastInstructionTicks = 1;
}

void CPU::load8BitsRegisterAtImmediateAddress(byte reg)
{
	mmu.write(mmu.readWord(pc), reg);
	pc += 2;
	lastInstructionTicks = 4;
}

void CPU::add8BitsValueTo8BitsRegister(byte& reg, byte value)
{
	unsetFlag(CpuFlags::SUBSTRACTION);
	uint16_t result = reg + value;
	setCarryFlag(result > 0xFF);
    setHalfCarryFlag((((reg & 0xF) + (value & 0xF)) & 0x10) == 0x10);
    reg = static_cast<byte>(result);
	changeZeroValueFlag(reg);
	lastInstructionTicks = 1;
}

void CPU::loadAccumulatorInHighMemoryValue()
{
	mmu.write(0xFF00 | mmu.read(pc), a);
	pc++;
	lastInstructionTicks = 3;
}

void CPU::loadHighMemoryValueInAccumulator()
{
	a = mmu.read(0xFF00 | mmu.read(pc));
	pc++;
	lastInstructionTicks = 3;
}

void CPU::loadAccumulatorInHighMemoryValue(byte reg)
{
	mmu.write(0xFF00 | reg, a);
	lastInstructionTicks = 2;
}

void CPU::loadHighMemoryValueInAccumulator(byte reg)
{
	a = mmu.read(0xFF00 | reg);
	lastInstructionTicks = 2;
}

void CPU::loadImmediate16BitsValueIn8BitsRegister(byte& reg)
{
    reg = mmu.read(mmu.readWord(pc));
    pc += 2;
    lastInstructionTicks = 4;
}