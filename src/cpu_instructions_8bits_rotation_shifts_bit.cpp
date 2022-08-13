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
	// TODO: Confirm that the Zero flag should also be set in the standard instruction set
	changeZeroValueFlag(reg);
	lastInstructionTicks = 1;
}

void CPU::rotateValueInMemoryLeftCircular(byte addrMsb, byte addrLsb)
{
	uint16_t addr = createAddrFromHighAndLowBytes(addrMsb, addrLsb);
	byte value = mmu.read(addr);
	rotateRegisterLeftCircular(value);
	mmu.write(addr, value);
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
	// TODO: Confirm that the Zero flag should also be set in the standard instruction set
	changeZeroValueFlag(reg);
	lastInstructionTicks = 1;
}

void CPU::rotateValueInMemoryRightCircular(byte addrMsb, byte addrLsb)
{
	uint16_t addr = createAddrFromHighAndLowBytes(addrMsb, addrLsb);
	byte value = mmu.read(addr);
	rotateRegisterRightCircular(value);
	mmu.write(addr, value);
	lastInstructionTicks = 4;
}

void CPU::rotateRegisterLeftExtended(byte& reg)
{
	unsetFlag(CpuFlags::SUBSTRACTION);
	unsetFlag(CpuFlags::ZERO);
	unsetFlag(CpuFlags::HALF_CARRY);

	byte currentCarry = isFlagSet(CpuFlags::CARRY);
	/* Set the carry flag to the highest bit of reg */
	setCarryFlag((reg & 0x80) > 0);
	/* Rotate the accumulator and set the last bit to the original carry flag */
	reg = (reg << 1) | currentCarry;
	// TODO: Confirm that the Zero flag should also be set in the standard instruction set
	changeZeroValueFlag(reg);
	lastInstructionTicks = 2;
}

void CPU::rotateValueInMemoryLeft(byte addrMsb, byte addrLsb)
{
	uint16_t addr = createAddrFromHighAndLowBytes(addrMsb, addrLsb);
	byte value = mmu.read(addr);
	rotateRegisterLeftExtended(value);
	mmu.write(addr, value);
	lastInstructionTicks = 4;
}

void CPU::rotateRegisterRightExtended(byte& reg)
{
	unsetFlag(CpuFlags::SUBSTRACTION);
	unsetFlag(CpuFlags::ZERO);
	unsetFlag(CpuFlags::HALF_CARRY);

	byte currentCarry = isFlagSet(CpuFlags::CARRY);
	/* Set the carry flag to the lowest bit of X */
	setCarryFlag((reg & 0x01) > 0);
	/* Rotate the accumulator and set the highest bit to the original carry flag */
	reg = (currentCarry << 7) | (reg >> 1);
	// TODO: Confirm that the Zero flag should also be set in the standard instruction set
	changeZeroValueFlag(reg);
	lastInstructionTicks = 2;
}

void CPU::rotateValueInMemoryRight(byte addrMsb, byte addrLsb)
{
	uint16_t addr = createAddrFromHighAndLowBytes(addrMsb, addrLsb);
	byte value = mmu.read(addr);
	rotateRegisterRightExtended(value);
	mmu.write(addr, value);
	lastInstructionTicks = 4;
}

void CPU::rotateRegisterLeftCircularExtended(byte& reg)
{
    rotateRegisterLeftCircular(reg);
    lastInstructionTicks = 2;
}

void CPU::rotateRegisterRightCircularExtended(byte& reg)
{
    rotateRegisterRightCircular(reg);
    lastInstructionTicks = 2;
}

void CPU::shiftRightArithmeticRegister(byte& reg)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    unsetFlag(CpuFlags::HALF_CARRY);
    setCarryFlag((reg & 0x01) > 0);
    reg = (reg >> 1);
    changeZeroValueFlag(reg);
    lastInstructionTicks = 2;
}

void CPU::shiftRightArithmeticMemory(uint16_t memoryAddr)
{
    byte value = mmu.read(memoryAddr);
    shiftRightArithmeticRegister(value);
    mmu.write(memoryAddr, value);
    lastInstructionTicks = 4;
}