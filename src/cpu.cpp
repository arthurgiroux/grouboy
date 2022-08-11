#include "cpu.hpp"
#include <iostream>

using namespace utils;

CPU::CPU(MMU& mmu_) : mmu(mmu_)
{
	tick = 0;
	lastInstructionTicks = 0;
	pc = 0;
	sp = 0;
	halted = false;
	interrupts = false;
	ticksBeforeEnablingInterrupts = 0;
	ticksBeforeDisablingInterrupts = 0;
	a = 0;
	b = 0;
	c = 0;
	d = 0;
	e = 0;
	h = 0;
	l = 0;
	f = 0;
}

int CPU::fetchDecodeAndExecute()
{
	// TODO: Review interrupts logic
	if (ticksBeforeEnablingInterrupts > 0)
	{
		ticksBeforeEnablingInterrupts--;
	}
	else if (ticksBeforeDisablingInterrupts > 0)
	{
		ticksBeforeDisablingInterrupts--;
	}

	executeInstruction(mmu.read(pc++));
	tick += lastInstructionTicks;

	if (ticksBeforeEnablingInterrupts == 1)
	{
		interrupts = true;
		ticksBeforeEnablingInterrupts = 0;
	}
	else if (ticksBeforeDisablingInterrupts == 1)
	{
		interrupts = false;
		ticksBeforeDisablingInterrupts = 0;
	}

	return lastInstructionTicks;
}

void CPU::setFlag(CpuFlags flag)
{
	f |= flag;
}

void CPU::unsetFlag(CpuFlags flag)
{
	f &= static_cast<byte>(~flag);
}

void CPU::setFlagIfTrue(bool condition, CpuFlags flag)
{
	if (condition)
	{
		setFlag(flag);
	}
	else
	{
		unsetFlag(flag);
	}
}

void CPU::changeZeroValueFlag(byte value)
{
	setFlagIfTrue(value == 0, CpuFlags::ZERO);
}

CPU::~CPU() = default;

void CPU::SLA_X(byte& X)
{
	unsetFlag(CpuFlags::SUBSTRACTION);
	unsetFlag(CpuFlags::ZERO);
	unsetFlag(CpuFlags::HALF_CARRY);
	setCarryFlag((X & 0x80) > 0);
	X = (X << 1);
	changeZeroValueFlag(X);
	lastInstructionTicks = 2;
}

void CPU::SLA_XYm(byte X, byte Y)
{
	byte value = mmu.read((X << 8) | Y);
	SLA_X(value);
	mmu.write((X << 8) | Y, value);
	lastInstructionTicks = 4;
}

void CPU::SRA_X(byte X)
{
	unsetFlag(CpuFlags::SUBSTRACTION);
	unsetFlag(CpuFlags::ZERO);
	unsetFlag(CpuFlags::HALF_CARRY);
	unsetFlag(CpuFlags::CARRY);

	X = ((X & 0x80) | (X >> 1));
	changeZeroValueFlag(X);
	lastInstructionTicks = 2;
}

void CPU::SRA_XYm(byte X, byte Y)
{
	byte value = mmu.read((X << 8) | Y);
	SRA_X(value);
	mmu.write((X << 8) | Y, value);
	lastInstructionTicks = 4;
}

void CPU::SRL_X(byte& X)
{
	unsetFlag(CpuFlags::SUBSTRACTION);
	unsetFlag(CpuFlags::ZERO);
	unsetFlag(CpuFlags::HALF_CARRY);

	setCarryFlag((X & 0x01) > 0);
	X = (X >> 1);
	changeZeroValueFlag(X);
	lastInstructionTicks = 2;
}

void CPU::SRL_XYm(byte X, byte Y)
{
	byte value = mmu.read((X << 8) | Y);
	SRL_X(value);
	mmu.write((X << 8) | Y, value);
	lastInstructionTicks = 4;
}

void CPU::SWAP_X(byte& X)
{
	unsetFlag(CpuFlags::SUBSTRACTION);
	unsetFlag(CpuFlags::ZERO);
	unsetFlag(CpuFlags::HALF_CARRY);
	unsetFlag(CpuFlags::CARRY);

	// swap the two nibbles
	X = ((X >> 4) | (X << 4));
	changeZeroValueFlag(X);
	lastInstructionTicks = 2;
}

void CPU::SWAP_XYm(byte X, byte Y)
{
	byte value = mmu.read((X << 8) | Y);
	SWAP_X(value);
	mmu.write((X << 8) | Y, value);
	lastInstructionTicks = 4;
}

void CPU::BIT_X_Y(byte X, byte Y)
{
	unsetFlag(CpuFlags::SUBSTRACTION);
	setFlag(CpuFlags::HALF_CARRY);
	// If the value of the bit is set then we set the flag zero
	changeZeroValueFlag((Y >> X) & 0x01);
	lastInstructionTicks = 2;
}

void CPU::BIT_X_YZm(byte X, byte Y, byte Z)
{
	byte value = mmu.read((Y << 8) | Z);
	BIT_X_Y(X, value);
	lastInstructionTicks = 4;
}

void CPU::RES_X_Y(byte X, byte& Y)
{
	Y = (Y & ~(0x01 << X));
	lastInstructionTicks = 2;
}

void CPU::RES_X_YZm(byte X, byte Y, byte Z)
{
	byte value = mmu.read((Y << 8) | Z);
	RES_X_Y(X, value);
	mmu.write((Y << 8) | Z, value);
	lastInstructionTicks = 4;
}

void CPU::SET_X_Y(byte X, byte& Y)
{
	Y = (Y | (0x01 << X));
	lastInstructionTicks = 2;
}

void CPU::SET_X_YZm(byte X, byte Y, byte Z)
{
	byte value = mmu.read((Y << 8) | Z);
	SET_X_Y(X, value);
	mmu.write((Y << 8) | Z, value);
	lastInstructionTicks = 4;
}

void CPU::ADD_SP_X(sbyte X)
{
	unsetFlag(CpuFlags::SUBSTRACTION);
	unsetFlag(CpuFlags::ZERO);
	uint16_t value = sp;
	sp += X;
	setCarryFlag(value > sp);
	setHalfCarryFlag((value <= 0x00FF) && (sp > 0x00FF));
	lastInstructionTicks = 4;
}

void CPU::executeInstruction(const byte& opCode)
{
	using namespace standardInstructions;

	switch (opCode)
	{

		/******************************************************/
		/************************ 0x00 ************************/
		/******************************************************/

	case NOP:
		NoOperation();
		break;

	case LD_BC_nn:
		load16BitsValueInRegisters(b, c);
		break;

	case LD_BCm_A:
		loadValueToMemoryAtAddr(b, c, a);
		break;

	case INC_BC:
		incrementRegistersValue(b, c);
		break;

	case INC_B:
		incrementRegisterValue(b);
		break;

	case DEC_B:
		decrementRegisterValue(b);
		break;

	case LD_B_n:
		loadImmediateValueInRegister(b);
		break;

	case RLC_A:
		rotateRegisterLeftCircular(a);
		break;

	case LD_nnm_SP:
		load16BitsRegisterAtImmediateAddress(sp);
		break;

	case ADD_HL_BC:
		addTwo8BitsRegistersToTwo8BitsRegisters(h, l, b, c);
		break;

	case LD_A_BCm:
		loadValueFromMemoryInto8BitsRegister(a, b, c);
		break;

	case DEC_BC:
		decrementRegistersValue(b, c);
		break;

	case INC_C:
		incrementRegisterValue(c);
		break;

	case DEC_C:
		decrementRegisterValue(c);
		break;

	case LD_C_n:
		loadImmediateValueInRegister(c);
		break;

	case RRC_A:
		rotateRegisterRightCircular(a);
		break;

		/******************************************************/
		/************************ 0x10 ************************/
		/******************************************************/

	case STOP:
		// TODO:
		// Stop cpu and gpu and wait for button to be pressed
		lastInstructionTicks = 2;
		break;

	case LD_DE_nn:
		load16BitsValueInRegisters(d, e);
		break;

	case LD_DEm_A:
		loadValueToMemoryAtAddr(d, e, a);
		break;

	case INC_DE:
		incrementRegistersValue(d, e);
		break;

	case INC_D:
		incrementRegisterValue(d);
		break;

	case DEC_D:
		decrementRegisterValue(d);
		break;

	case LD_D_n:
		loadImmediateValueInRegister(d);
		break;

	case RL_A:
		rotateRegisterLeftExtended(a);
		break;

	case JR_n:
		jumpRelative();
		break;

	case ADD_HL_DE:
		addTwo8BitsRegistersToTwo8BitsRegisters(h, l, d, e);
		break;

	case LD_A_DEm:
		loadValueFromMemoryInto8BitsRegister(a, d, e);
		break;

	case DEC_DE:
		decrementRegistersValue(d, e);
		break;

	case INC_E:
		incrementRegisterValue(e);
		break;

	case DEC_E:
		decrementRegisterValue(e);
		break;

	case LD_E_n:
		loadImmediateValueInRegister(e);
		break;

	case RR_A:
		rotateRegisterRightExtended(a);
		break;

		/******************************************************/
		/************************ 0x20 ************************/
		/******************************************************/

	case JR_NZ_n:
		jumpRelativeConditional(!isFlagSet(CpuFlags::ZERO));
		break;

	case LD_HL_nn:
		load16BitsValueInRegisters(h, l);
		break;

	case LD_HLm_I_A:
		loadValueToMemoryAndIncrementAddr(h, l, a);
		break;

	case INC_HL:
		incrementRegistersValue(h, l);
		break;

	case INC_H:
		incrementRegisterValue(h);
		break;

	case DEC_H:
		decrementRegisterValue(h);
		break;

	case LD_H_n:
		loadImmediateValueInRegister(h);
		break;

	case DAA:
		decimalAdjustAccumulator();
		break;

	case JR_Z_n:
		jumpRelativeConditional(isFlagSet(CpuFlags::ZERO));
		break;

	case ADD_HL_HL:
		addTwo8BitsRegistersToTwo8BitsRegisters(h, l, h, l);
		break;

	case LD_A_HLm_I:
		loadValueFromMemoryAndIncrementAddr(a, h, l);
		break;

	case DEC_HL:
		decrementRegistersValue(h, l);
		break;

	case INC_L:
		incrementRegisterValue(l);
		break;

	case DEC_L:
		decrementRegisterValue(l);
		break;

	case LD_L_n:
		loadImmediateValueInRegister(l);
		break;

	case CPL:
		complementRegister(a);
		break;

		/******************************************************/
		/************************ 0x30 ************************/
		/******************************************************/

	case JR_NC_n:
		jumpRelativeConditional(!isFlagSet(CpuFlags::CARRY));
		break;

	case LD_SP_nn:
		load16BitsImmediateValueIntoRegister(sp);
		break;

	case LD_HLm_D_A:
		loadValueToMemoryAndDecreaseAddr(h, l, a);
		break;

	case INC_SP:
		incrementRegisterValue(sp);
		break;

	case INC_HLm:
		incrementValueInMemoryAtAddr(h, l);
		break;

	case DEC_HLm:
		decrementValueInMemoryAtAddr(h, l);
		break;

	case LD_HLm_n:
		load8BitsImmediateValueAtMemoryAddress(h, l);
		break;

	case SCF:
		setCarryFlagInstruction();
		break;

	case JR_C_n:
		jumpRelativeConditional(isFlagSet(CpuFlags::CARRY));
		break;

	case ADD_HL_SP:
		addTwo8BitsRegistersToTwo8BitsRegisters(h, l, (sp << 8), (sp & 0x00FF));
		break;

	case LD_A_HLm_D:
		loadValueFromMemoryAndDecreaseAddr(a, h, l);
		break;

	case DEC_SP:
		decrementRegisterValue(sp);
		break;

	case INC_A:
		incrementRegisterValue(a);
		break;

	case DEC_A:
		decrementRegisterValue(a);
		break;

	case LD_A_n:
		loadImmediateValueInRegister(a);
		break;

	case CCF:
		invertsCarryFlag();
		break;

		/******************************************************/
		/************************ 0x40 ************************/
		/******************************************************/

	case LD_B_B:
		load8BitsValueInRegister(b, b);
		break;

	case LD_B_C:
		load8BitsValueInRegister(b, c);
		break;

	case LD_B_D:
		load8BitsValueInRegister(b, d);
		break;

	case LD_B_E:
		load8BitsValueInRegister(b, e);
		break;

	case LD_B_H:
		load8BitsValueInRegister(b, h);
		break;

	case LD_B_L:
		load8BitsValueInRegister(b, l);
		break;

	case LD_B_HLm:
		loadValueFromMemoryInto8BitsRegister(b, h, l);
		break;

	case LD_B_A:
		load8BitsValueInRegister(b, a);
		break;

	case LD_C_B:
		load8BitsValueInRegister(c, b);
		break;

	case LD_C_C:
		load8BitsValueInRegister(c, c);
		break;

	case LD_C_D:
		load8BitsValueInRegister(c, d);
		break;

	case LD_C_E:
		load8BitsValueInRegister(c, e);
		break;

	case LD_C_H:
		load8BitsValueInRegister(c, h);
		break;

	case LD_C_L:
		load8BitsValueInRegister(c, l);
		break;

	case LD_C_HLm:
		loadValueFromMemoryInto8BitsRegister(c, h, l);
		break;

	case LD_C_A:
		load8BitsValueInRegister(c, a);
		break;

		/******************************************************/
		/************************ 0x50 ************************/
		/******************************************************/

	case LD_D_B:
		load8BitsValueInRegister(d, b);
		break;

	case LD_D_C:
		load8BitsValueInRegister(d, c);
		break;

	case LD_D_D:
		load8BitsValueInRegister(d, d);
		break;

	case LD_D_E:
		load8BitsValueInRegister(d, e);
		break;

	case LD_D_H:
		load8BitsValueInRegister(d, h);
		break;

	case LD_D_L:
		load8BitsValueInRegister(d, l);
		break;

	case LD_D_HLm:
		loadValueFromMemoryInto8BitsRegister(d, h, l);
		break;

	case LD_D_A:
		load8BitsValueInRegister(d, a);
		break;

	case LD_E_B:
		load8BitsValueInRegister(e, b);
		break;

	case LD_E_C:
		load8BitsValueInRegister(e, c);
		break;

	case LD_E_D:
		load8BitsValueInRegister(e, d);
		break;

	case LD_E_E:
		load8BitsValueInRegister(e, e);
		break;

	case LD_E_H:
		load8BitsValueInRegister(e, h);
		break;

	case LD_E_L:
		load8BitsValueInRegister(e, l);
		break;

	case LD_E_HLm:
		loadValueFromMemoryInto8BitsRegister(e, h, l);
		break;

	case LD_E_A:
		load8BitsValueInRegister(e, a);
		break;

		/******************************************************/
		/************************ 0x60 ************************/
		/******************************************************/

	case LD_H_B:
		load8BitsValueInRegister(h, b);
		break;

	case LD_H_C:
		load8BitsValueInRegister(h, c);
		break;

	case LD_H_D:
		load8BitsValueInRegister(h, d);
		break;

	case LD_H_E:
		load8BitsValueInRegister(h, e);
		break;

	case LD_H_H:
		load8BitsValueInRegister(h, h);
		break;

	case LD_H_L:
		load8BitsValueInRegister(h, l);
		break;

	case LD_H_HLm:
		loadValueFromMemoryInto8BitsRegister(h, h, l);
		break;

	case LD_H_A:
		load8BitsValueInRegister(h, a);
		break;

	case LD_L_B:
		load8BitsValueInRegister(l, b);
		break;

	case LD_L_C:
		load8BitsValueInRegister(l, c);
		break;

	case LD_L_D:
		load8BitsValueInRegister(l, d);
		break;

	case LD_L_E:
		load8BitsValueInRegister(l, e);
		break;

	case LD_L_H:
		load8BitsValueInRegister(l, h);
		break;

	case LD_L_L:
		load8BitsValueInRegister(l, l);
		break;

	case LD_L_HLm:
		loadValueFromMemoryInto8BitsRegister(l, h, l);
		break;

	case LD_L_A:
		load8BitsValueInRegister(l, a);
		break;

		/******************************************************/
		/************************ 0x70 ************************/
		/******************************************************/

	case LD_HLm_B:
		loadValueToMemoryAtAddr(h, l, b);
		break;

	case LD_HLm_C:
		loadValueToMemoryAtAddr(h, l, c);
		break;

	case LD_HLm_D:
		loadValueToMemoryAtAddr(h, l, d);
		break;

	case LD_HLm_E:
		loadValueToMemoryAtAddr(h, l, e);
		break;

	case LD_HLm_H:
		loadValueToMemoryAtAddr(h, l, h);
		break;

	case LD_HLm_L:
		loadValueToMemoryAtAddr(h, l, l);
		break;

	case HALT:
		halted = true;
		lastInstructionTicks = 1;
		break;

	case LD_HLm_A:
		loadValueToMemoryAtAddr(h, l, a);
		break;

	case LD_A_B:
		load8BitsValueInRegister(a, b);
		break;

	case LD_A_C:
		load8BitsValueInRegister(a, c);
		break;

	case LD_A_D:
		load8BitsValueInRegister(a, d);
		break;

	case LD_A_E:
		load8BitsValueInRegister(a, e);
		break;

	case LD_A_H:
		load8BitsValueInRegister(a, h);
		break;

	case LD_A_L:
		load8BitsValueInRegister(a, l);
		break;

	case LD_A_HLm:
		loadValueFromMemoryInto8BitsRegister(a, h, l);
		break;

	case LD_A_A:
		load8BitsValueInRegister(a, a);
		break;

		/******************************************************/
		/************************ 0x80 ************************/
		/******************************************************/

	case ADD_A_B:
		add8BitsValueTo8BitsRegister(a, b);
		break;

	case ADD_A_C:
		add8BitsValueTo8BitsRegister(a, c);
		break;

	case ADD_A_D:
		add8BitsValueTo8BitsRegister(a, d);
		break;

	case ADD_A_E:
		add8BitsValueTo8BitsRegister(a, e);
		break;

	case ADD_A_H:
		add8BitsValueTo8BitsRegister(a, h);
		break;

	case ADD_A_L:
		add8BitsValueTo8BitsRegister(a, l);
		break;

	case ADD_A_HLm:
		addValueFromMemoryTo8BitsRegister(a, h, l);
		break;

	case ADD_A_A:
		add8BitsValueTo8BitsRegister(a, a);
		break;

	case ADC_A_B:
		add8BitsValueAndCarryTo8BitsRegister(a, b);
		break;

	case ADC_A_C:
		add8BitsValueAndCarryTo8BitsRegister(a, c);
		break;

	case ADC_A_D:
		add8BitsValueAndCarryTo8BitsRegister(a, d);
		break;

	case ADC_A_E:
		add8BitsValueAndCarryTo8BitsRegister(a, e);
		break;

	case ADC_A_H:
		add8BitsValueAndCarryTo8BitsRegister(a, h);
		break;

	case ADC_A_L:
		add8BitsValueAndCarryTo8BitsRegister(a, l);
		break;

	case ADC_A_HLm:
		addValueFromMemoryAndCarryTo8BitsRegister(a, h, l);
		break;

	case ADC_A_A:
		add8BitsValueAndCarryTo8BitsRegister(a, a);
		break;

		/******************************************************/
		/************************ 0x90 ************************/
		/******************************************************/

	case SUB_A_B:
		substract8BitsValueFrom8BitsRegister(a, b);
		break;

	case SUB_A_C:
		substract8BitsValueFrom8BitsRegister(a, c);
		break;

	case SUB_A_D:
		substract8BitsValueFrom8BitsRegister(a, d);
		break;

	case SUB_A_E:
		substract8BitsValueFrom8BitsRegister(a, e);
		break;

	case SUB_A_H:
		substract8BitsValueFrom8BitsRegister(a, h);
		break;

	case SUB_A_L:
		substract8BitsValueFrom8BitsRegister(a, l);
		break;

	case SUB_A_HLm:
		substractValueInMemoryFrom8BitsRegister(a, h, l);
		break;

	case SUB_A_A:
		substract8BitsValueFrom8BitsRegister(a, a);
		break;

	case SBC_A_B:
		sub8BitsValueAndCarryTo8BitsRegister(a, b);
		break;

	case SBC_A_C:
		sub8BitsValueAndCarryTo8BitsRegister(a, c);
		break;

	case SBC_A_D:
		sub8BitsValueAndCarryTo8BitsRegister(a, d);
		break;

	case SBC_A_E:
		sub8BitsValueAndCarryTo8BitsRegister(a, e);
		break;

	case SBC_A_H:
		sub8BitsValueAndCarryTo8BitsRegister(a, h);
		break;

	case SBC_A_L:
		sub8BitsValueAndCarryTo8BitsRegister(a, l);
		break;

	case SBC_A_HLm:
		subValueFromMemoryAndCarryTo8BitsRegister(a, h, l);
		break;

	case SBC_A_A:
		sub8BitsValueAndCarryTo8BitsRegister(a, a);
		break;

		/******************************************************/
		/************************ 0xA0 ************************/
		/******************************************************/

	case AND_B:
		logicalAndBetweenAccumulatorAnd8BitsRegister(b);
		break;

	case AND_C:
		logicalAndBetweenAccumulatorAnd8BitsRegister(c);
		break;

	case AND_D:
		logicalAndBetweenAccumulatorAnd8BitsRegister(d);
		break;

	case AND_E:
		logicalAndBetweenAccumulatorAnd8BitsRegister(e);
		break;

	case AND_H:
		logicalAndBetweenAccumulatorAnd8BitsRegister(h);
		break;

	case AND_L:
		logicalAndBetweenAccumulatorAnd8BitsRegister(l);
		break;

	case AND_HLm:
		logicalAndBetweenAccumulatorAndValueInMemory(h, l);
		break;

	case AND_A:
		logicalAndBetweenAccumulatorAnd8BitsRegister(a);
		break;

	case XOR_B:
		logicalXorBetweenAccumulatorAnd8BitsRegister(b);
		break;

	case XOR_C:
		logicalXorBetweenAccumulatorAnd8BitsRegister(c);
		break;

	case XOR_D:
		logicalXorBetweenAccumulatorAnd8BitsRegister(d);
		break;

	case XOR_E:
		logicalXorBetweenAccumulatorAnd8BitsRegister(e);
		break;

	case XOR_H:
		logicalXorBetweenAccumulatorAnd8BitsRegister(h);
		break;

	case XOR_L:
		logicalXorBetweenAccumulatorAnd8BitsRegister(l);
		break;

	case XOR_HLm:
		logicalXorBetweenAccumulatorAndValueInMemory(h, l);
		break;

	case XOR_A:
		logicalXorBetweenAccumulatorAnd8BitsRegister(a);
		break;

		/******************************************************/
		/************************ 0xB0 ************************/
		/******************************************************/

	case OR_B:
		logicalOrBetweenAccumulatorAnd8BitsRegister(b);
		break;

	case OR_C:
		logicalOrBetweenAccumulatorAnd8BitsRegister(c);
		break;

	case OR_D:
		logicalOrBetweenAccumulatorAnd8BitsRegister(d);
		break;

	case OR_E:
		logicalOrBetweenAccumulatorAnd8BitsRegister(e);
		break;

	case OR_H:
		logicalOrBetweenAccumulatorAnd8BitsRegister(h);
		break;

	case OR_L:
		logicalOrBetweenAccumulatorAnd8BitsRegister(l);
		break;

	case OR_HLm:
		logicalOrBetweenAccumulatorAndValueInMemory(h, l);
		break;

	case OR_A:
		logicalOrBetweenAccumulatorAnd8BitsRegister(a);
		break;

	case CP_B:
		compareAccumulatorAndRegister(b);
		break;

	case CP_C:
		compareAccumulatorAndRegister(c);
		break;

	case CP_D:
		compareAccumulatorAndRegister(d);
		break;

	case CP_E:
		compareAccumulatorAndRegister(e);
		break;

	case CP_H:
		compareAccumulatorAndRegister(h);
		break;

	case CP_L:
		compareAccumulatorAndRegister(l);
		break;

	case CP_HLm:
		compareAccumulatorAndValueInMemory(h, l);
		break;

	case CP_A:
		compareAccumulatorAndRegister(a);
		break;

		/******************************************************/
		/************************ 0xC0 ************************/
		/******************************************************/

	case RET_NZ:
		returnInstructionConditional(!isFlagSet(CpuFlags::ZERO));
		break;

	case POP_BC:
		popMemoryIntoRegisterPair(b, c);
		break;

	case JP_NZ_nn:
		jumpConditional(!isFlagSet(CpuFlags::ZERO));
		break;

	case JP_nn:
		jump();
		break;

	case CALL_NZ_nn:
		callImmediateSubroutineIfConditionSatisfied(!isFlagSet(CpuFlags::ZERO));
		break;

	case PUSH_BC:
		push16BitsOntoStackPointer(b, c);
		break;

	case ADD_A_n:
		addImmediateValueTo8BitsRegister(a);
		break;

	case RST_0:
		callRestartRoutine(0x00);
		break;

	case RET_Z:
		returnInstructionConditional(isFlagSet(CpuFlags::ZERO));
		break;

	case RET:
		returnInstruction();
		break;

	case JP_Z_nn:
		jumpConditional(isFlagSet(CpuFlags::ZERO));
		break;

	case EXT_OPS:
		executeExtendedInstruction(mmu.read(pc++));
		break;

	case CALL_Z_nn:
		callImmediateSubroutineIfConditionSatisfied(isFlagSet(CpuFlags::ZERO));
		break;

	case CALL_nn:
		callImmediateSubroutine();
		break;

	case ADC_A_n:
		addImmediateValueAndCarryTo8BitsRegister(a);
		break;

	case RST_8:
		callRestartRoutine(0x08);
		break;

		/******************************************************/
		/************************ 0xD0 ************************/
		/******************************************************/

	case RET_NC:
		returnInstructionConditional(!isFlagSet(CpuFlags::CARRY));
		break;

	case POP_DE:
		popMemoryIntoRegisterPair(d, e);
		break;

	case JP_NC_nn:
		jumpConditional(!isFlagSet(CpuFlags::CARRY));
		break;

	case CALL_NC_nn:
		callImmediateSubroutineIfConditionSatisfied(!isFlagSet(CpuFlags::CARRY));
		break;

	case PUSH_DE:
		push16BitsOntoStackPointer(d, e);
		break;

	case SUB_A_n:
		substractImmediateValueFrom8BitsRegister(a);
		break;

	case RST_10:
		callRestartRoutine(0x10);
		break;

	case RET_C:
		returnInstructionConditional(isFlagSet(CpuFlags::CARRY));
		break;

	case RETI:
		returnInstructionAfterInterrupt();
		break;

	case JP_C_nn:
		jumpConditional(isFlagSet(CpuFlags::CARRY));
		break;

	case CALL_C_nn:
		callImmediateSubroutineIfConditionSatisfied(isFlagSet(CpuFlags::CARRY));
		break;

	case SBC_A_n:
		subImmediateValueAndCarryTo8BitsRegister(a);
		break;

	case RST_18:
		callRestartRoutine(0x18);
		break;

		/******************************************************/
		/************************ 0xE0 ************************/
		/******************************************************/

	case LDH_nm_A:
        loadAccumulatorInHighMemoryValue();
		break;

	case POP_HL:
		popMemoryIntoRegisterPair(h, l);
		break;

	case LD_Cm_A:
		loadAccumulatorInHighMemoryValue(c);
		break;

	case PUSH_HL:
		push16BitsOntoStackPointer(h, l);
		break;

	case AND_n:
		logicalAndBetweenAccumulatorAndImmediateValue();
		break;

	case RST_20:
		callRestartRoutine(0x20);
		break;

	case ADD_SP_d:
		ADD_SP_X(static_cast<sbyte>(mmu.read(pc++)));
		break;

	case JP_HLm:
		jumpToAddrIn16BitsRegister(h, l);
		break;

	case LD_nnm_A:
		load8BitsRegisterAtImmediateAddress(a);
		break;

	case XOR_n:
		logicalXorBetweenAccumulatorAndImmediateValue();
		break;

	case RST_28:
		callRestartRoutine(0x28);
		break;
		/******************************************************/
		/************************ 0xF0 ************************/
		/******************************************************/

	case LDH_A_nm:
        loadHighMemoryValueInAccumulator();
		break;

	case POP_AF:
		popMemoryIntoRegisterPair(a, f);
		break;

	case LD_A_Cm:
		loadHighMemoryValueInAccumulator(c);
		break;

	case DI:
		disableInterrupts();
		break;

	case PUSH_AF:
		push16BitsOntoStackPointer(a, f);
		break;

	case OR_n:
		logicalOrBetweenAccumulatorAndImmediateValue();
		break;

	case RST_30:
		callRestartRoutine(0x30);
		break;

	case LDHL_SP_d:
        load16BitsRegisterAndImmediateOffsetIn16BitsRegister(h, l, sp);
		break;

	case LD_SP_HL:
		loadTwo8BitsRegisterIn16BitsRegister(sp, h, l);
		break;

	case LD_A_nnm:
        loadImmediate16BitsValueIn8BitsRegister(a);
		break;

	case EI:
		enableInterrupts();
		break;

	case CP_n:
		compareAccumulatorAndImmediateValue();
		break;

	case RST_38:
		callRestartRoutine(0x38);
		break;

	default:
		throw UnhandledInstructionException(opCode);
		return;
	}
}

void CPU::executeExtendedInstruction(const byte& opCode)
{
	using namespace extendedInstructions;

	switch (opCode)
	{

		/******************************************************/
		/************************ 0x00 ************************/
		/******************************************************/

	case RLC_B:
		rotateRegisterLeftCircularExtended(b);
		break;

	case RLC_C:
		rotateRegisterLeftCircularExtended(c);
		break;

	case RLC_D:
		rotateRegisterLeftCircularExtended(d);
		break;

	case RLC_E:
		rotateRegisterLeftCircularExtended(e);
		break;

	case RLC_H:
		rotateRegisterLeftCircularExtended(h);
		break;

	case RLC_L:
		rotateRegisterLeftCircularExtended(l);
		break;

	case RLC_HLm:
		rotateValueInMemoryLeftCircular(h, l);
		break;

	case RLC_A:
		rotateRegisterLeftCircularExtended(a);
		break;

	case RRC_B:
		rotateRegisterRightCircularExtended(b);
		break;

	case RRC_C:
		rotateRegisterRightCircularExtended(c);
		break;

	case RRC_D:
		rotateRegisterRightCircularExtended(d);
		break;

	case RRC_E:
		rotateRegisterRightCircularExtended(e);
		break;

	case RRC_H:
		rotateRegisterRightCircularExtended(h);
		break;

	case RRC_L:
		rotateRegisterRightCircularExtended(l);
		break;

	case RRC_HLm:
		rotateValueInMemoryRightCircular(h, l);
		break;

	case RRC_A:
		rotateRegisterRightCircularExtended(a);
		break;

		/******************************************************/
		/************************ 0x10 ************************/
		/******************************************************/

	case RL_B:
		rotateRegisterLeftExtended(b);
		break;

	case RL_C:
		rotateRegisterLeftExtended(c);
		break;

	case RL_D:
		rotateRegisterLeftExtended(d);
		break;

	case RL_E:
		rotateRegisterLeftExtended(e);
		break;

	case RL_H:
		rotateRegisterLeftExtended(h);
		break;

	case RL_L:
		rotateRegisterLeftExtended(l);
		break;

	case RL_HLm:
		rotateValueInMemoryLeft(h, l);
		break;

	case RL_A:
		rotateRegisterLeftExtended(a);
		break;

	case RR_B:
		rotateRegisterRightExtended(b);
		break;

	case RR_C:
		rotateRegisterRightExtended(c);
		break;

	case RR_D:
		rotateRegisterRightExtended(d);
		break;

	case RR_E:
		rotateRegisterRightExtended(e);
		break;

	case RR_H:
		rotateRegisterRightExtended(h);
		break;

	case RR_L:
		rotateRegisterRightExtended(l);
		break;

	case RR_HLm:
		rotateValueInMemoryRight(h, l);
		break;

	case RR_A:
		rotateRegisterRightExtended(a);
		break;

		/******************************************************/
		/************************ 0x20 ************************/
		/******************************************************/

	case SLA_B:
		SLA_X(b);
		break;

	case SLA_C:
		SLA_X(c);
		break;

	case SLA_D:
		SLA_X(d);
		break;

	case SLA_E:
		SLA_X(e);
		break;

	case SLA_H:
		SLA_X(h);
		break;

	case SLA_HLm:
		SLA_XYm(h, l);
		break;

	case SLA_A:
		SLA_X(a);
		break;

	case SRA_B:
		SRA_X(b);
		break;

	case SRA_C:
		SRA_X(c);
		break;

	case SRA_D:
		SRA_X(d);
		break;

	case SRA_E:
		SRA_X(e);
		break;

	case SRA_H:
		SRA_X(h);
		break;

	case SRA_HLm:
		SRA_XYm(h, l);
		break;

	case SRA_A:
		SRA_X(a);
		break;

		/******************************************************/
		/************************ 0x30 ************************/
		/******************************************************/

	case SWAP_B:
		SWAP_X(b);
		break;

	case SWAP_C:
		SWAP_X(c);
		break;

	case SWAP_D:
		SWAP_X(d);
		break;

	case SWAP_E:
		SWAP_X(e);
		break;

	case SWAP_H:
		SWAP_X(h);
		break;

	case SWAP_HLm:
		SWAP_XYm(h, l);
		break;

	case SWAP_A:
		SWAP_X(a);
		break;

	case SRL_B:
		SRL_X(b);
		break;

	case SRL_C:
		SRL_X(c);
		break;

	case SRL_D:
		SRL_X(d);
		break;

	case SRL_E:
		SRL_X(e);
		break;

	case SRL_H:
		SRL_X(h);
		break;

	case SRL_HLm:
		SRL_XYm(h, l);
		break;

	case SRL_A:
		SRL_X(a);
		break;

		/******************************************************/
		/************************ 0x40 ************************/
		/******************************************************/

	case BIT_0_B:
		BIT_X_Y(0, b);
		break;

	case BIT_0_C:
		BIT_X_Y(0, c);
		break;

	case BIT_0_D:
		BIT_X_Y(0, d);
		break;

	case BIT_0_E:
		BIT_X_Y(0, e);
		break;

	case BIT_0_H:
		BIT_X_Y(0, h);
		break;

	case BIT_0_HLm:
		BIT_X_YZm(0, h, l);
		break;

	case BIT_0_A:
		BIT_X_Y(0, a);
		break;

	case BIT_1_B:
		BIT_X_Y(1, b);
		break;

	case BIT_1_C:
		BIT_X_Y(1, c);
		break;

	case BIT_1_D:
		BIT_X_Y(1, d);
		break;

	case BIT_1_E:
		BIT_X_Y(1, e);
		break;

	case BIT_1_H:
		BIT_X_Y(1, h);
		break;

	case BIT_1_HLm:
		BIT_X_YZm(1, h, l);
		break;

	case BIT_1_A:
		BIT_X_Y(1, a);
		break;

		/******************************************************/
		/************************ 0x50 ************************/
		/******************************************************/

	case BIT_2_B:
		BIT_X_Y(2, b);
		break;

	case BIT_2_C:
		BIT_X_Y(2, c);
		break;

	case BIT_2_D:
		BIT_X_Y(2, d);
		break;

	case BIT_2_E:
		BIT_X_Y(2, e);
		break;

	case BIT_2_H:
		BIT_X_Y(2, h);
		break;

	case BIT_2_HLm:
		BIT_X_YZm(2, h, l);
		break;

	case BIT_2_A:
		BIT_X_Y(2, a);
		break;

	case BIT_3_B:
		BIT_X_Y(3, b);
		break;

	case BIT_3_C:
		BIT_X_Y(3, c);
		break;

	case BIT_3_D:
		BIT_X_Y(3, d);
		break;

	case BIT_3_E:
		BIT_X_Y(3, e);
		break;

	case BIT_3_H:
		BIT_X_Y(3, h);
		break;

	case BIT_3_HLm:
		BIT_X_YZm(3, h, l);
		break;

	case BIT_3_A:
		BIT_X_Y(3, a);
		break;

		/******************************************************/
		/************************ 0x60 ************************/
		/******************************************************/

	case BIT_4_B:
		BIT_X_Y(4, b);
		break;

	case BIT_4_C:
		BIT_X_Y(4, c);
		break;

	case BIT_4_D:
		BIT_X_Y(4, d);
		break;

	case BIT_4_E:
		BIT_X_Y(4, e);
		break;

	case BIT_4_H:
		BIT_X_Y(4, h);
		break;

	case BIT_4_HLm:
		BIT_X_YZm(4, h, l);
		break;

	case BIT_4_A:
		BIT_X_Y(4, a);
		break;

	case BIT_5_B:
		BIT_X_Y(5, b);
		break;

	case BIT_5_C:
		BIT_X_Y(5, c);
		break;

	case BIT_5_D:
		BIT_X_Y(5, d);
		break;

	case BIT_5_E:
		BIT_X_Y(5, e);
		break;

	case BIT_5_H:
		BIT_X_Y(5, h);
		break;

	case BIT_5_HLm:
		BIT_X_YZm(5, h, l);
		break;

	case BIT_5_A:
		BIT_X_Y(5, a);
		break;

		/******************************************************/
		/************************ 0x70 ************************/
		/******************************************************/

	case BIT_6_B:
		BIT_X_Y(6, b);
		break;

	case BIT_6_C:
		BIT_X_Y(6, c);
		break;

	case BIT_6_D:
		BIT_X_Y(6, d);
		break;

	case BIT_6_E:
		BIT_X_Y(6, e);
		break;

	case BIT_6_H:
		BIT_X_Y(6, h);
		break;

	case BIT_6_HLm:
		BIT_X_YZm(6, h, l);
		break;

	case BIT_6_A:
		BIT_X_Y(6, a);
		break;

	case BIT_7_B:
		BIT_X_Y(7, b);
		break;

	case BIT_7_C:
		BIT_X_Y(7, c);
		break;

	case BIT_7_D:
		BIT_X_Y(7, d);
		break;

	case BIT_7_E:
		BIT_X_Y(7, e);
		break;

	case BIT_7_H:
		BIT_X_Y(7, h);
		break;

	case BIT_7_HLm:
		BIT_X_YZm(7, h, l);
		break;

	case BIT_7_A:
		BIT_X_Y(7, a);
		break;

		/******************************************************/
		/************************ 0x80 ************************/
		/******************************************************/

	case RES_0_B:
		RES_X_Y(0, b);
		break;

	case RES_0_C:
		RES_X_Y(0, c);
		break;

	case RES_0_D:
		RES_X_Y(0, d);
		break;

	case RES_0_E:
		RES_X_Y(0, e);
		break;

	case RES_0_H:
		RES_X_Y(0, h);
		break;

	case RES_0_HLm:
		RES_X_YZm(0, h, l);
		break;

	case RES_0_A:
		RES_X_Y(0, a);
		break;

	case RES_1_B:
		RES_X_Y(1, b);
		break;

	case RES_1_C:
		RES_X_Y(1, c);
		break;

	case RES_1_D:
		RES_X_Y(1, d);
		break;

	case RES_1_E:
		RES_X_Y(1, e);
		break;

	case RES_1_H:
		RES_X_Y(1, h);
		break;

	case RES_1_HLm:
		RES_X_YZm(1, h, l);
		break;

	case RES_1_A:
		RES_X_Y(1, a);
		break;

		/******************************************************/
		/************************ 0x90 ************************/
		/******************************************************/

	case RES_2_B:
		RES_X_Y(2, b);
		break;

	case RES_2_C:
		RES_X_Y(2, c);
		break;

	case RES_2_D:
		RES_X_Y(2, d);
		break;

	case RES_2_E:
		RES_X_Y(2, e);
		break;

	case RES_2_H:
		RES_X_Y(2, h);
		break;

	case RES_2_HLm:
		RES_X_YZm(2, h, l);
		break;

	case RES_2_A:
		RES_X_Y(2, a);
		break;

	case RES_3_B:
		RES_X_Y(3, b);
		break;

	case RES_3_C:
		RES_X_Y(3, c);
		break;

	case RES_3_D:
		RES_X_Y(3, d);
		break;

	case RES_3_E:
		RES_X_Y(3, e);
		break;

	case RES_3_H:
		RES_X_Y(3, h);
		break;

	case RES_3_HLm:
		RES_X_YZm(3, h, l);
		break;

	case RES_3_A:
		RES_X_Y(3, a);
		break;

		/******************************************************/
		/************************ 0xA0 ************************/
		/******************************************************/

	case RES_4_B:
		RES_X_Y(4, b);
		break;

	case RES_4_C:
		RES_X_Y(4, c);
		break;

	case RES_4_D:
		RES_X_Y(4, d);
		break;

	case RES_4_E:
		RES_X_Y(4, e);
		break;

	case RES_4_H:
		RES_X_Y(4, h);
		break;

	case RES_4_HLm:
		RES_X_YZm(4, h, l);
		break;

	case RES_4_A:
		RES_X_Y(4, a);
		break;

	case RES_5_B:
		RES_X_Y(5, b);
		break;

	case RES_5_C:
		RES_X_Y(5, c);
		break;

	case RES_5_D:
		RES_X_Y(5, d);
		break;

	case RES_5_E:
		RES_X_Y(5, e);
		break;

	case RES_5_H:
		RES_X_Y(5, h);
		break;

	case RES_5_HLm:
		RES_X_YZm(5, h, l);
		break;

	case RES_5_A:
		RES_X_Y(5, a);
		break;

		/******************************************************/
		/************************ 0xB0 ************************/
		/******************************************************/

	case RES_6_B:
		RES_X_Y(6, b);
		break;

	case RES_6_C:
		RES_X_Y(6, c);
		break;

	case RES_6_D:
		RES_X_Y(6, d);
		break;

	case RES_6_E:
		RES_X_Y(6, e);
		break;

	case RES_6_H:
		RES_X_Y(6, h);
		break;

	case RES_6_HLm:
		RES_X_YZm(6, h, l);
		break;

	case RES_6_A:
		RES_X_Y(6, a);
		break;

	case RES_7_B:
		RES_X_Y(7, b);
		break;

	case RES_7_C:
		RES_X_Y(7, c);
		break;

	case RES_7_D:
		RES_X_Y(7, d);
		break;

	case RES_7_E:
		RES_X_Y(7, e);
		break;

	case RES_7_H:
		RES_X_Y(7, h);
		break;

	case RES_7_HLm:
		RES_X_YZm(7, h, l);
		break;

	case RES_7_A:
		RES_X_Y(7, a);
		break;

		/******************************************************/
		/************************ 0xC0 ************************/
		/******************************************************/

	case SET_0_B:
		SET_X_Y(0, b);
		break;

	case SET_0_C:
		SET_X_Y(0, c);
		break;

	case SET_0_D:
		SET_X_Y(0, d);
		break;

	case SET_0_E:
		SET_X_Y(0, e);
		break;

	case SET_0_H:
		SET_X_Y(0, h);
		break;

	case SET_0_HLm:
		SET_X_YZm(0, h, l);
		break;

	case SET_0_A:
		SET_X_Y(0, a);
		break;

	case SET_1_B:
		SET_X_Y(1, b);
		break;

	case SET_1_C:
		SET_X_Y(1, c);
		break;

	case SET_1_D:
		SET_X_Y(1, d);
		break;

	case SET_1_E:
		SET_X_Y(1, e);
		break;

	case SET_1_H:
		SET_X_Y(1, h);
		break;

	case SET_1_HLm:
		SET_X_YZm(1, h, l);
		break;

	case SET_1_A:
		SET_X_Y(1, a);
		break;

		/******************************************************/
		/************************ 0xD0 ************************/
		/******************************************************/

	case SET_2_B:
		SET_X_Y(2, b);
		break;

	case SET_2_C:
		SET_X_Y(2, c);
		break;

	case SET_2_D:
		SET_X_Y(2, d);
		break;

	case SET_2_E:
		SET_X_Y(2, e);
		break;

	case SET_2_H:
		SET_X_Y(2, h);
		break;

	case SET_2_HLm:
		SET_X_YZm(2, h, l);
		break;

	case SET_2_A:
		SET_X_Y(2, a);
		break;

	case SET_3_B:
		SET_X_Y(3, b);
		break;

	case SET_3_C:
		SET_X_Y(3, c);
		break;

	case SET_3_D:
		SET_X_Y(3, d);
		break;

	case SET_3_E:
		SET_X_Y(3, e);
		break;

	case SET_3_H:
		SET_X_Y(3, h);
		break;

	case SET_3_HLm:
		SET_X_YZm(3, h, l);
		break;

	case SET_3_A:
		SET_X_Y(3, a);
		break;

		/******************************************************/
		/************************ 0xE0 ************************/
		/******************************************************/

	case SET_4_B:
		SET_X_Y(4, b);
		break;

	case SET_4_C:
		SET_X_Y(4, c);
		break;

	case SET_4_D:
		SET_X_Y(4, d);
		break;

	case SET_4_E:
		SET_X_Y(4, e);
		break;

	case SET_4_H:
		SET_X_Y(4, h);
		break;

	case SET_4_HLm:
		SET_X_YZm(4, h, l);
		break;

	case SET_4_A:
		SET_X_Y(4, a);
		break;

	case SET_5_B:
		SET_X_Y(5, b);
		break;

	case SET_5_C:
		SET_X_Y(5, c);
		break;

	case SET_5_D:
		SET_X_Y(5, d);
		break;

	case SET_5_E:
		SET_X_Y(5, e);
		break;

	case SET_5_H:
		SET_X_Y(5, h);
		break;

	case SET_5_HLm:
		SET_X_YZm(5, h, l);
		break;

	case SET_5_A:
		SET_X_Y(5, a);
		break;

		/******************************************************/
		/************************ 0xF0 ************************/
		/******************************************************/

	case SET_6_B:
		SET_X_Y(6, b);
		break;

	case SET_6_C:
		SET_X_Y(6, c);
		break;

	case SET_6_D:
		SET_X_Y(6, d);
		break;

	case SET_6_E:
		SET_X_Y(6, e);
		break;

	case SET_6_H:
		SET_X_Y(6, h);
		break;

	case SET_6_HLm:
		SET_X_YZm(6, h, l);
		break;

	case SET_6_A:
		SET_X_Y(6, a);
		break;

	case SET_7_B:
		SET_X_Y(7, b);
		break;

	case SET_7_C:
		SET_X_Y(7, c);
		break;

	case SET_7_D:
		SET_X_Y(7, d);
		break;

	case SET_7_E:
		SET_X_Y(7, e);
		break;

	case SET_7_H:
		SET_X_Y(7, h);
		break;

	case SET_7_HLm:
		SET_X_YZm(7, h, l);
		break;

	case SET_7_A:
		SET_X_Y(7, a);
		break;

	default:
		throw UnhandledExtendedInstructionException(opCode);
		return;
	}
}

bool CPU::isFlagSet(CPU::CpuFlags flag) const
{
	return (f & flag) > 0;
}

void CPU::setHalfCarryFlag(bool state)
{
	setFlagIfTrue(state, CpuFlags::HALF_CARRY);
}

void CPU::setCarryFlag(bool state)
{
	setFlagIfTrue(state, CpuFlags::CARRY);
}

byte CPU::getFlag() const
{
	return f;
}

void CPU::resetFlags()
{
	f = 0x00;
}
