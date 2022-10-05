#include "cpu.hpp"
#include <iostream>

#include <memory>

using namespace utils;

CPU::CPU(MMU& mmu_) : mmu(mmu_)
{
	tick = 0;
	lastInstructionTicks = 0;
	pc = 0;
	sp = 0;
	halted = false;
	interruptsEnabled = false;
	a = 0;
	b = 0;
	c = 0;
	d = 0;
	e = 0;
	h = 0;
	l = 0;
	f = 0;

    interruptHandlers.push_back(std::make_unique<InterruptHandlerVBlank>(this, &mmu));
    interruptHandlers.push_back(std::make_unique<InterruptHandlerLCDStat>(this, &mmu));
    interruptHandlers.push_back(std::make_unique<InterruptHandlerTimer>(this, &mmu));
    interruptHandlers.push_back(std::make_unique<InterruptHandlerSerial>(this, &mmu));
    interruptHandlers.push_back(std::make_unique<InterruptHandlerJoypad>(this, &mmu));
}

int CPU::fetchDecodeAndExecute()
{
	handleInterrupts();

	if (halted) {
		return 1;
	}

	if (interruptsEnabledRequested)
	{
		interruptsEnabled = true;
		interruptsEnabledRequested = false;
	}

	executeInstruction(mmu.read(pc++));
	tick += lastInstructionTicks;

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

void CPU::handleInterrupts()
{
	if (!interruptsEnabled) {
		return;
	}

	for (auto& handler : interruptHandlers) {
		if (handler->handle()) {
            interruptsEnabled = false;
			break;
		}
	}
}

CPU::~CPU() = default;

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
		stopInstruction();
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
		rotateRegisterLeft(a);
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
		rotateRegisterRight(a);
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
		addTwo8BitsRegistersToTwo8BitsRegisters(h, l, getMsbFromWord(sp), getLsbFromWord(sp));
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
		haltInstruction();
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
		addImmediateValueToStackPointer();
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
		// The lower nibble of the flag register is hardwired to 0
		// We reset this part if it was affected by the operation
		f &= 0xF0;
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
		shiftLeftArithmeticRegister(b);
		break;

	case SLA_C:
		shiftLeftArithmeticRegister(c);
		break;

	case SLA_D:
		shiftLeftArithmeticRegister(d);
		break;

	case SLA_E:
		shiftLeftArithmeticRegister(e);
		break;

	case SLA_H:
		shiftLeftArithmeticRegister(h);
		break;

	case SLA_L:
		shiftLeftArithmeticRegister(l);
		break;

	case SLA_HLm:
		shiftLeftArithmeticMemory(createAddrFromHighAndLowBytes(h, l));
		break;

	case SLA_A:
		shiftLeftArithmeticRegister(a);
		break;

	case SRA_B:
		shiftRightArithmeticRegister(b);
		break;

	case SRA_C:
		shiftRightArithmeticRegister(c);
		break;

	case SRA_D:
		shiftRightArithmeticRegister(d);
		break;

	case SRA_E:
		shiftRightArithmeticRegister(e);
		break;

	case SRA_H:
		shiftRightArithmeticRegister(h);
		break;

	case SRA_L:
		shiftRightArithmeticRegister(l);
		break;

	case SRA_HLm:
		shiftRightArithmeticMemory(createAddrFromHighAndLowBytes(h, l));
		break;

	case SRA_A:
		shiftRightArithmeticRegister(a);
		break;

		/******************************************************/
		/************************ 0x30 ************************/
		/******************************************************/

	case SWAP_B:
		swapNibblesInRegister(b);
		break;

	case SWAP_C:
		swapNibblesInRegister(c);
		break;

	case SWAP_D:
		swapNibblesInRegister(d);
		break;

	case SWAP_E:
		swapNibblesInRegister(e);
		break;

	case SWAP_H:
		swapNibblesInRegister(h);
		break;

	case SWAP_L:
		swapNibblesInRegister(l);
		break;

	case SWAP_HLm:
		swapNibblesInMemory(createAddrFromHighAndLowBytes(h, l));
		break;

	case SWAP_A:
		swapNibblesInRegister(a);
		break;

	case SRL_B:
		shiftRightLogicalRegister(b);
		break;

	case SRL_C:
		shiftRightLogicalRegister(c);
		break;

	case SRL_D:
		shiftRightLogicalRegister(d);
		break;

	case SRL_E:
		shiftRightLogicalRegister(e);
		break;

	case SRL_H:
		shiftRightLogicalRegister(h);
		break;

	case SRL_L:
		shiftRightLogicalRegister(l);
		break;

	case SRL_HLm:
		shiftRightLogicalMemory(createAddrFromHighAndLowBytes(h, l));
		break;

	case SRL_A:
		shiftRightLogicalRegister(a);
		break;

		/******************************************************/
		/************************ 0x40 ************************/
		/******************************************************/

	case BIT_0_B:
		isBitSetForValue(b, 0);
		break;

	case BIT_0_C:
		isBitSetForValue(c, 0);
		break;

	case BIT_0_D:
		isBitSetForValue(d, 0);
		break;

	case BIT_0_E:
		isBitSetForValue(e, 0);
		break;

	case BIT_0_H:
		isBitSetForValue(h, 0);
		break;

	case BIT_0_L:
		isBitSetForValue(l, 0);
		break;

	case BIT_0_HLm:
		isBitSetInMemory(createAddrFromHighAndLowBytes(h, l), 0);
		break;

	case BIT_0_A:
		isBitSetForValue(a, 0);
		break;

	case BIT_1_B:
		isBitSetForValue(b, 1);
		break;

	case BIT_1_C:
		isBitSetForValue(c, 1);
		break;

	case BIT_1_D:
		isBitSetForValue(d, 1);
		break;

	case BIT_1_E:
		isBitSetForValue(e, 1);
		break;

	case BIT_1_H:
		isBitSetForValue(h, 1);
		break;

	case BIT_1_L:
		isBitSetForValue(l, 1);
		break;

	case BIT_1_HLm:
		isBitSetInMemory(createAddrFromHighAndLowBytes(h, l), 1);
		break;

	case BIT_1_A:
		isBitSetForValue(a, 1);
		break;

		/******************************************************/
		/************************ 0x50 ************************/
		/******************************************************/

	case BIT_2_B:
		isBitSetForValue(b, 2);
		break;

	case BIT_2_C:
		isBitSetForValue(c, 2);
		break;

	case BIT_2_D:
		isBitSetForValue(d, 2);
		break;

	case BIT_2_E:
		isBitSetForValue(e, 2);
		break;

	case BIT_2_H:
		isBitSetForValue(h, 2);
		break;

	case BIT_2_L:
		isBitSetForValue(l, 2);
		break;

	case BIT_2_HLm:
		isBitSetInMemory(createAddrFromHighAndLowBytes(h, l), 2);
		break;

	case BIT_2_A:
		isBitSetForValue(a, 2);
		break;

	case BIT_3_B:
		isBitSetForValue(b, 3);
		break;

	case BIT_3_C:
		isBitSetForValue(c, 3);
		break;

	case BIT_3_D:
		isBitSetForValue(d, 3);
		break;

	case BIT_3_E:
		isBitSetForValue(e, 3);
		break;

	case BIT_3_H:
		isBitSetForValue(h, 3);
		break;

	case BIT_3_L:
		isBitSetForValue(l, 3);
		break;

	case BIT_3_HLm:
		isBitSetInMemory(createAddrFromHighAndLowBytes(h, l), 3);
		break;

	case BIT_3_A:
		isBitSetForValue(a, 3);
		break;

		/******************************************************/
		/************************ 0x60 ************************/
		/******************************************************/

	case BIT_4_B:
		isBitSetForValue(b, 4);
		break;

	case BIT_4_C:
		isBitSetForValue(c, 4);
		break;

	case BIT_4_D:
		isBitSetForValue(d, 4);
		break;

	case BIT_4_E:
		isBitSetForValue(e, 4);
		break;

	case BIT_4_H:
		isBitSetForValue(h, 4);
		break;

	case BIT_4_L:
		isBitSetForValue(l, 4);
		break;

	case BIT_4_HLm:
		isBitSetInMemory(createAddrFromHighAndLowBytes(h, l), 4);
		break;

	case BIT_4_A:
		isBitSetForValue(a, 4);
		break;

	case BIT_5_B:
		isBitSetForValue(b, 5);
		break;

	case BIT_5_C:
		isBitSetForValue(c, 5);
		break;

	case BIT_5_D:
		isBitSetForValue(d, 5);
		break;

	case BIT_5_E:
		isBitSetForValue(e, 5);
		break;

	case BIT_5_H:
		isBitSetForValue(h, 5);
		break;

	case BIT_5_L:
		isBitSetForValue(l, 5);
		break;

	case BIT_5_HLm:
		isBitSetInMemory(createAddrFromHighAndLowBytes(h, l), 5);
		break;

	case BIT_5_A:
		isBitSetForValue(a, 5);
		break;

		/******************************************************/
		/************************ 0x70 ************************/
		/******************************************************/

	case BIT_6_B:
		isBitSetForValue(b, 6);
		break;

	case BIT_6_C:
		isBitSetForValue(c, 6);
		break;

	case BIT_6_D:
		isBitSetForValue(d, 6);
		break;

	case BIT_6_E:
		isBitSetForValue(e, 6);
		break;

	case BIT_6_H:
		isBitSetForValue(h, 6);
		break;

	case BIT_6_L:
		isBitSetForValue(l, 6);
		break;

	case BIT_6_HLm:
		isBitSetInMemory(createAddrFromHighAndLowBytes(h, l), 6);
		break;

	case BIT_6_A:
		isBitSetForValue(a, 6);
		break;

	case BIT_7_B:
		isBitSetForValue(b, 7);
		break;

	case BIT_7_C:
		isBitSetForValue(c, 7);
		break;

	case BIT_7_D:
		isBitSetForValue(d, 7);
		break;

	case BIT_7_E:
		isBitSetForValue(e, 7);
		break;

	case BIT_7_H:
		isBitSetForValue(h, 7);
		break;

	case BIT_7_L:
		isBitSetForValue(l, 7);
		break;

	case BIT_7_HLm:
		isBitSetInMemory(createAddrFromHighAndLowBytes(h, l), 7);
		break;

	case BIT_7_A:
		isBitSetForValue(a, 7);
		break;

		/******************************************************/
		/************************ 0x80 ************************/
		/******************************************************/

	case RES_0_B:
		resetBitForValue(b, 0);
		break;

	case RES_0_C:
		resetBitForValue(c, 0);
		break;

	case RES_0_D:
		resetBitForValue(d, 0);
		break;

	case RES_0_E:
		resetBitForValue(e, 0);
		break;

	case RES_0_H:
		resetBitForValue(h, 0);
		break;

	case RES_0_L:
		resetBitForValue(l, 0);
		break;

	case RES_0_HLm:
		resetBitInMemory(createAddrFromHighAndLowBytes(h, l), 0);
		break;

	case RES_0_A:
		resetBitForValue(a, 0);
		break;

	case RES_1_B:
		resetBitForValue(b, 1);
		break;

	case RES_1_C:
		resetBitForValue(c, 1);
		break;

	case RES_1_D:
		resetBitForValue(d, 1);
		break;

	case RES_1_E:
		resetBitForValue(e, 1);
		break;

	case RES_1_H:
		resetBitForValue(h, 1);
		break;

	case RES_1_L:
		resetBitForValue(l, 1);
		break;

	case RES_1_HLm:
		resetBitInMemory(createAddrFromHighAndLowBytes(h, l), 1);
		break;

	case RES_1_A:
		resetBitForValue(a, 1);
		break;

		/******************************************************/
		/************************ 0x90 ************************/
		/******************************************************/

	case RES_2_B:
		resetBitForValue(b, 2);
		break;

	case RES_2_C:
		resetBitForValue(c, 2);
		break;

	case RES_2_D:
		resetBitForValue(d, 2);
		break;

	case RES_2_E:
		resetBitForValue(e, 2);
		break;

	case RES_2_H:
		resetBitForValue(h, 2);
		break;

	case RES_2_L:
		resetBitForValue(l, 2);
		break;

	case RES_2_HLm:
		resetBitInMemory(createAddrFromHighAndLowBytes(h, l), 2);
		break;

	case RES_2_A:
		resetBitForValue(a, 2);
		break;

	case RES_3_B:
		resetBitForValue(b, 3);
		break;

	case RES_3_C:
		resetBitForValue(c, 3);
		break;

	case RES_3_D:
		resetBitForValue(d, 3);
		break;

	case RES_3_E:
		resetBitForValue(e, 3);
		break;

	case RES_3_H:
		resetBitForValue(h, 3);
		break;

	case RES_3_L:
		resetBitForValue(l, 3);
		break;

	case RES_3_HLm:
		resetBitInMemory(createAddrFromHighAndLowBytes(h, l), 3);
		break;

	case RES_3_A:
		resetBitForValue(a, 3);
		break;

		/******************************************************/
		/************************ 0xA0 ************************/
		/******************************************************/

	case RES_4_B:
		resetBitForValue(b, 4);
		break;

	case RES_4_C:
		resetBitForValue(c, 4);
		break;

	case RES_4_D:
		resetBitForValue(d, 4);
		break;

	case RES_4_E:
		resetBitForValue(e, 4);
		break;

	case RES_4_H:
		resetBitForValue(h, 4);
		break;

	case RES_4_L:
		resetBitForValue(l, 4);
		break;

	case RES_4_HLm:
		resetBitInMemory(createAddrFromHighAndLowBytes(h, l), 4);
		break;

	case RES_4_A:
		resetBitForValue(a, 4);
		break;

	case RES_5_B:
		resetBitForValue(b, 5);
		break;

	case RES_5_C:
		resetBitForValue(c, 5);
		break;

	case RES_5_D:
		resetBitForValue(d, 5);
		break;

	case RES_5_E:
		resetBitForValue(e, 5);
		break;

	case RES_5_H:
		resetBitForValue(h, 5);
		break;

	case RES_5_L:
		resetBitForValue(l, 5);
		break;

	case RES_5_HLm:
		resetBitInMemory(createAddrFromHighAndLowBytes(h, l), 5);
		break;

	case RES_5_A:
		resetBitForValue(a, 5);
		break;

		/******************************************************/
		/************************ 0xB0 ************************/
		/******************************************************/

	case RES_6_B:
		resetBitForValue(b, 6);
		break;

	case RES_6_C:
		resetBitForValue(c, 6);
		break;

	case RES_6_D:
		resetBitForValue(d, 6);
		break;

	case RES_6_E:
		resetBitForValue(e, 6);
		break;

	case RES_6_H:
		resetBitForValue(h, 6);
		break;

	case RES_6_L:
		resetBitForValue(l, 6);
		break;

	case RES_6_HLm:
		resetBitInMemory(createAddrFromHighAndLowBytes(h, l), 6);
		break;

	case RES_6_A:
		resetBitForValue(a, 6);
		break;

	case RES_7_B:
		resetBitForValue(b, 7);
		break;

	case RES_7_C:
		resetBitForValue(c, 7);
		break;

	case RES_7_D:
		resetBitForValue(d, 7);
		break;

	case RES_7_E:
		resetBitForValue(e, 7);
		break;

	case RES_7_H:
		resetBitForValue(h, 7);
		break;

	case RES_7_L:
		resetBitForValue(l, 7);
		break;

	case RES_7_HLm:
		resetBitInMemory(createAddrFromHighAndLowBytes(h, l), 7);
		break;

	case RES_7_A:
		resetBitForValue(a, 7);
		break;

		/******************************************************/
		/************************ 0xC0 ************************/
		/******************************************************/

	case SET_0_B:
		setBitForValue(b, 0);
		break;

	case SET_0_C:
		setBitForValue(c, 0);
		break;

	case SET_0_D:
		setBitForValue(d, 0);
		break;

	case SET_0_E:
		setBitForValue(e, 0);
		break;

	case SET_0_H:
		setBitForValue(h, 0);
		break;

	case SET_0_L:
		setBitForValue(l, 0);
		break;

	case SET_0_HLm:
		setBitInMemory(createAddrFromHighAndLowBytes(h, l), 0);
		break;

	case SET_0_A:
		setBitForValue(a, 0);
		break;

	case SET_1_B:
		setBitForValue(b, 1);
		break;

	case SET_1_C:
		setBitForValue(c, 1);
		break;

	case SET_1_D:
		setBitForValue(d, 1);
		break;

	case SET_1_E:
		setBitForValue(e, 1);
		break;

	case SET_1_H:
		setBitForValue(h, 1);
		break;

	case SET_1_L:
		setBitForValue(l, 1);
		break;

	case SET_1_HLm:
		setBitInMemory(createAddrFromHighAndLowBytes(h, l), 1);
		break;

	case SET_1_A:
		setBitForValue(a, 1);
		break;

		/******************************************************/
		/************************ 0xD0 ************************/
		/******************************************************/

	case SET_2_B:
		setBitForValue(b, 2);
		break;

	case SET_2_C:
		setBitForValue(c, 2);
		break;

	case SET_2_D:
		setBitForValue(d, 2);
		break;

	case SET_2_E:
		setBitForValue(e, 2);
		break;

	case SET_2_H:
		setBitForValue(h, 2);
		break;

	case SET_2_L:
		setBitForValue(l, 2);
		break;

	case SET_2_HLm:
		setBitInMemory(createAddrFromHighAndLowBytes(h, l), 2);
		break;

	case SET_2_A:
		setBitForValue(a, 2);
		break;

	case SET_3_B:
		setBitForValue(b, 3);
		break;

	case SET_3_C:
		setBitForValue(c, 3);
		break;

	case SET_3_D:
		setBitForValue(d, 3);
		break;

	case SET_3_E:
		setBitForValue(e, 3);
		break;

	case SET_3_H:
		setBitForValue(h, 3);
		break;

	case SET_3_L:
		setBitForValue(l, 3);
		break;

	case SET_3_HLm:
		setBitInMemory(createAddrFromHighAndLowBytes(h, l), 3);
		break;

	case SET_3_A:
		setBitForValue(a, 3);
		break;

		/******************************************************/
		/************************ 0xE0 ************************/
		/******************************************************/

	case SET_4_B:
		setBitForValue(b, 4);
		break;

	case SET_4_C:
		setBitForValue(c, 4);
		break;

	case SET_4_D:
		setBitForValue(d, 4);
		break;

	case SET_4_E:
		setBitForValue(e, 4);
		break;

	case SET_4_H:
		setBitForValue(h, 4);
		break;

	case SET_4_L:
		setBitForValue(l, 4);
		break;

	case SET_4_HLm:
		setBitInMemory(createAddrFromHighAndLowBytes(h, l), 4);
		break;

	case SET_4_A:
		setBitForValue(a, 4);
		break;

	case SET_5_B:
		setBitForValue(b, 5);
		break;

	case SET_5_C:
		setBitForValue(c, 5);
		break;

	case SET_5_D:
		setBitForValue(d, 5);
		break;

	case SET_5_E:
		setBitForValue(e, 5);
		break;

	case SET_5_H:
		setBitForValue(h, 5);
		break;

	case SET_5_L:
		setBitForValue(l, 5);
		break;

	case SET_5_HLm:
		setBitInMemory(createAddrFromHighAndLowBytes(h, l), 5);
		break;

	case SET_5_A:
		setBitForValue(a, 5);
		break;

		/******************************************************/
		/************************ 0xF0 ************************/
		/******************************************************/

	case SET_6_B:
		setBitForValue(b, 6);
		break;

	case SET_6_C:
		setBitForValue(c, 6);
		break;

	case SET_6_D:
		setBitForValue(d, 6);
		break;

	case SET_6_E:
		setBitForValue(e, 6);
		break;

	case SET_6_H:
		setBitForValue(h, 6);
		break;

	case SET_6_L:
		setBitForValue(l, 6);
		break;

	case SET_6_HLm:
		setBitInMemory(createAddrFromHighAndLowBytes(h, l), 6);
		break;

	case SET_6_A:
		setBitForValue(a, 6);
		break;

	case SET_7_B:
		setBitForValue(b, 7);
		break;

	case SET_7_C:
		setBitForValue(c, 7);
		break;

	case SET_7_D:
		setBitForValue(d, 7);
		break;

	case SET_7_E:
		setBitForValue(e, 7);
		break;

	case SET_7_H:
		setBitForValue(h, 7);
		break;

	case SET_7_L:
		setBitForValue(l, 7);
		break;

	case SET_7_HLm:
		setBitInMemory(createAddrFromHighAndLowBytes(h, l), 7);
		break;

	case SET_7_A:
		setBitForValue(a, 7);
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
