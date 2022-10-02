#include "cpu.hpp"

using namespace utils;

void CPU::incrementRegisterValue(uint16_t& reg)
{
    reg++;
    lastInstructionTicks = 2;
}

void CPU::incrementValueInMemoryAtAddr(byte addrMsb, byte addrLsb)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    uint16_t addr = createAddrFromHighAndLowBytes(addrMsb, addrLsb);
    byte oldvalue = mmu.read(addr);
    byte value = oldvalue + 1;
    mmu.write(addr, value);
    changeZeroValueFlag(value);
    setHalfCarryFlag((value & 0x0F) == 0x00);

    lastInstructionTicks = 3;
}

void CPU::decrementValueInMemoryAtAddr(byte addrMsb, byte addrLsb)
{
    setFlag(CpuFlags::SUBSTRACTION);
    uint16_t addr = createAddrFromHighAndLowBytes(addrMsb, addrLsb);
    byte oldvalue = mmu.read(addr);
    byte value = oldvalue - 1;
    mmu.write(addr, value);
    changeZeroValueFlag(value);
    setHalfCarryFlag((value & 0x0F) == 0x0F);
    lastInstructionTicks = 3;
}

void CPU::incrementRegisterValue(byte& reg)
{
    unsetFlag(CpuFlags::SUBSTRACTION);

    reg++;
    setHalfCarryFlag((reg & 0x0F) == 0x00);
    changeZeroValueFlag(reg);
    lastInstructionTicks = 1;
}

void CPU::decrementRegisterValue(byte& reg)
{
    setFlag(CpuFlags::SUBSTRACTION);

    reg--;
    setHalfCarryFlag((reg & 0x0F) == 0x0F);
    changeZeroValueFlag(reg);
    lastInstructionTicks = 1;
}

void CPU::decrementRegisterValue(uint16_t& reg)
{
    reg--;
    lastInstructionTicks = 2;
}

void CPU::decimalAdjustAccumulator()
{
    byte valueToAdd = 0;
    if (((a & 0x0F) > 9) || isFlagSet(CpuFlags::HALF_CARRY))
    {
        valueToAdd += 0x06;
    }

    if (((a >> 4) > 9) || isFlagSet(CpuFlags::CARRY))
    {
        valueToAdd += 0x60;
        setFlag(CpuFlags::CARRY);
    }
    else
    {
        unsetFlag(CpuFlags::CARRY);
    }

    a += valueToAdd;

    setFlagIfTrue(a == 0, CpuFlags::ZERO);
    unsetFlag(CpuFlags::HALF_CARRY);
    lastInstructionTicks = 1;
}

void CPU::setCarryFlagInstruction()
{
    unsetFlag(CpuFlags::HALF_CARRY);
    unsetFlag(CpuFlags::SUBSTRACTION);
    setFlag(CpuFlags::CARRY);
    lastInstructionTicks = 1;
}

void CPU::invertsCarryFlag()
{
    unsetFlag(CpuFlags::HALF_CARRY);
    unsetFlag(CpuFlags::SUBSTRACTION);
    setCarryFlag(!isFlagSet(CpuFlags::CARRY));
    lastInstructionTicks = 1;
}

void CPU::complementRegister(byte& reg)
{
    reg = ~reg;
    setFlag(CpuFlags::SUBSTRACTION);
    setFlag(CpuFlags::HALF_CARRY);
    lastInstructionTicks = 1;
}

void CPU::addImmediateValueTo8BitsRegister(byte& reg)
{
    add8BitsValueTo8BitsRegister(reg, mmu.read(pc++));
    lastInstructionTicks = 2;
}

void CPU::add8BitsValueAndCarryTo8BitsRegister(byte& reg, byte value)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    uint16_t result = reg + value;
	int offset = isFlagSet(CpuFlags::CARRY) ? 1 : 0;
	result += offset;
    setCarryFlag(result > 0xFF);
    setHalfCarryFlag((((reg & 0xF) + (value & 0xF) + offset) & 0x10) == 0x10);
    reg = static_cast<byte>(result);
    changeZeroValueFlag(reg);
    lastInstructionTicks = 1;
}

void CPU::addImmediateValueAndCarryTo8BitsRegister(byte& reg)
{
    add8BitsValueAndCarryTo8BitsRegister(reg, mmu.read(pc++));
    lastInstructionTicks = 2;
}

void CPU::addValueFromMemoryTo8BitsRegister(byte& reg, byte addrMsb, byte addrLsb)
{
    add8BitsValueTo8BitsRegister(reg, mmu.read(createAddrFromHighAndLowBytes(addrMsb, addrLsb)));
    lastInstructionTicks = 2;
}

void CPU::addValueFromMemoryAndCarryTo8BitsRegister(byte& reg, byte addrMsb, byte addrLsb)
{
    add8BitsValueAndCarryTo8BitsRegister(reg, mmu.read(createAddrFromHighAndLowBytes(addrMsb, addrLsb)));
    lastInstructionTicks = 2;
}

void CPU::substract8BitsValueFrom8BitsRegister(byte& reg, byte value)
{
    setFlag(CpuFlags::SUBSTRACTION);
    uint16_t result = reg - value;
    setCarryFlag(result > 0xFF);
    setHalfCarryFlag(((reg & 0xF) - (value & 0xF)) < 0);
    reg = static_cast<byte>(result);
    changeZeroValueFlag(reg);
    lastInstructionTicks = 1;
}

void CPU::substractImmediateValueFrom8BitsRegister(byte& reg)
{
    substract8BitsValueFrom8BitsRegister(reg, mmu.read(pc++));
    lastInstructionTicks = 2;
}

void CPU::substractValueInMemoryFrom8BitsRegister(byte& reg, byte addrMsb, byte addrLsb)
{
    substract8BitsValueFrom8BitsRegister(reg, mmu.read(createAddrFromHighAndLowBytes(addrMsb, addrLsb)));
    lastInstructionTicks = 2;
}

void CPU::sub8BitsValueAndCarryTo8BitsRegister(byte& reg, byte value)
{
    setFlag(CpuFlags::SUBSTRACTION);
    uint16_t result = reg - value;
	int offset = isFlagSet(CpuFlags::CARRY) ? 1 : 0;
	result -= offset;
    setCarryFlag(result > 0xFF);
    setHalfCarryFlag(((reg & 0xF) - (value & 0xF) - offset) < 0);
    reg = static_cast<byte>(result);
    changeZeroValueFlag(reg);
    lastInstructionTicks = 1;
}

void CPU::subImmediateValueAndCarryTo8BitsRegister(byte& reg)
{
    sub8BitsValueAndCarryTo8BitsRegister(reg, mmu.read(pc++));
    lastInstructionTicks = 2;
}

void CPU::subValueFromMemoryAndCarryTo8BitsRegister(byte& reg, byte addrMsb, byte addrLsb)
{
    sub8BitsValueAndCarryTo8BitsRegister(reg, mmu.read(createAddrFromHighAndLowBytes(addrMsb, addrLsb)));
    lastInstructionTicks = 2;
}

void CPU::logicalAndBetweenAccumulatorAnd8BitsRegister(byte value)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    setFlag(CpuFlags::HALF_CARRY);
    unsetFlag(CpuFlags::CARRY);
    a &= value;
    changeZeroValueFlag(a);
    lastInstructionTicks = 1;
}

void CPU::logicalAndBetweenAccumulatorAndValueInMemory(byte addrMsb, byte addrLsb)
{
    logicalAndBetweenAccumulatorAnd8BitsRegister(mmu.read(createAddrFromHighAndLowBytes(addrMsb, addrLsb)));
    lastInstructionTicks = 2;
}

void CPU::logicalAndBetweenAccumulatorAndImmediateValue()
{
    logicalAndBetweenAccumulatorAnd8BitsRegister(mmu.read(pc++));
    lastInstructionTicks = 2;
}

void CPU::logicalXorBetweenAccumulatorAnd8BitsRegister(byte value)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    unsetFlag(CpuFlags::HALF_CARRY);
    unsetFlag(CpuFlags::CARRY);
    a ^= value;
    changeZeroValueFlag(a);
    lastInstructionTicks = 1;
}

void CPU::logicalXorBetweenAccumulatorAndValueInMemory(byte addrMsb, byte addrLsb)
{
    logicalXorBetweenAccumulatorAnd8BitsRegister(mmu.read(createAddrFromHighAndLowBytes(addrMsb, addrLsb)));
    lastInstructionTicks = 2;
}

void CPU::logicalXorBetweenAccumulatorAndImmediateValue()
{
    logicalXorBetweenAccumulatorAnd8BitsRegister(mmu.read(pc++));
    lastInstructionTicks = 2;
}

void CPU::logicalOrBetweenAccumulatorAnd8BitsRegister(byte value)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    unsetFlag(CpuFlags::HALF_CARRY);
    unsetFlag(CpuFlags::CARRY);
    a |= value;
    changeZeroValueFlag(a);
    lastInstructionTicks = 1;
}

void CPU::logicalOrBetweenAccumulatorAndValueInMemory(byte addrMsb, byte addrLsb)
{
    logicalOrBetweenAccumulatorAnd8BitsRegister(mmu.read(createAddrFromHighAndLowBytes(addrMsb, addrLsb)));
    lastInstructionTicks = 2;
}

void CPU::logicalOrBetweenAccumulatorAndImmediateValue()
{
    logicalOrBetweenAccumulatorAnd8BitsRegister(mmu.read(pc++));
    lastInstructionTicks = 2;
}

void CPU::compareAccumulatorAndRegister(byte value)
{
    setFlag(CpuFlags::SUBSTRACTION);
    setFlagIfTrue(a == value, CpuFlags::ZERO);
    setHalfCarryFlag(((a & 0xf) - (value & 0xf)) < 0);
    setCarryFlag(a < value);
    lastInstructionTicks = 1;
}

void CPU::compareAccumulatorAndValueInMemory(byte addrMsb, byte addrLsb)
{
    compareAccumulatorAndRegister(mmu.read(createAddrFromHighAndLowBytes(addrMsb, addrLsb)));
    lastInstructionTicks = 2;
}

void CPU::compareAccumulatorAndImmediateValue()
{
    compareAccumulatorAndRegister(mmu.read(pc++));
    lastInstructionTicks = 2;
}