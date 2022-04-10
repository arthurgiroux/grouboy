#include <iostream>
#include <cassert>
#include "cpu.hpp"
#include "instructions.hpp"
#include "mmu.hpp"

/* RESSOURCES:
	http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
	http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
	http://www.8052.com/
	http://gameboy.mongenel.com/dmg/opcodes.html
*/

CPU::CPU(MMU& mmu_) : mmu(mmu_) {
	tick = 0;
	lastInstructionTicks = 0;
	pc = 0;
	sp = 0;
	halted = false;
	interrupts = false;
	ticksBeforeEnablingInterrupts = 0;
	ticksBeforeDisablingInterrupts = 0;
}

int CPU::exec() {

	if (ticksBeforeEnablingInterrupts > 0) {
		ticksBeforeEnablingInterrupts--;
	}
	else if (ticksBeforeDisablingInterrupts > 0) {
		ticksBeforeDisablingInterrupts--;
	}

	process(mmu.read(pc++));
	tick += lastInstructionTicks;

	if (ticksBeforeEnablingInterrupts == 1) {
		interrupts = true;
		ticksBeforeEnablingInterrupts = 0;
	}
	else if (ticksBeforeDisablingInterrupts == 1) {
		interrupts = false;
		ticksBeforeDisablingInterrupts = 0;
	}

	return lastInstructionTicks;

}

void CPU::setFlag(byte flag) {
	f |= flag;
}

void CPU::unsetFlag(byte flag) {
	f &= ~flag;
}

void CPU::setFlagIfTrue(bool condition, byte flag) {
	if (condition) {
		setFlag(flag);
	}
	else {
		unsetFlag(flag);
	}
}

void CPU::changeZeroValueFlag(byte value) {
	setFlagIfTrue(value == 0, FLAG_ZERO);
}

CPU::~CPU() {

}

void CPU::LD_XY_NN(byte& X, byte& Y) {
	Y = mmu.read(pc);
	X = mmu.read(pc + 1);
	pc += 2;
	lastInstructionTicks = 3;
}

void CPU::LD_XY_Z_N(byte& X, byte& Y, uint16_t Z) {
	unsetFlag(FLAG_ZERO);
	unsetFlag(FLAG_SUB);
	uint16_t value = (X << 8) | Y;
	uint16_t xy = static_cast<int8_t>(mmu.read(pc)) + Z;
	pc++;
	X = (xy >> 8);
	Y = xy & 0x00FF;
	// we check for overflow
	setFlagIfTrue((value > xy), FLAG_CARRY);
	// check for half carry
	setFlagIfTrue((value <= 0xFF) && (xy > 0xFF), FLAG_HALF_CARRY);
	lastInstructionTicks = 3;
}

void CPU::LD_X_NN(uint16_t& X) {
	X = mmu.readWord(pc);
	pc += 2;
	lastInstructionTicks = 3;
}

void CPU::LD_X_NNm(byte& X) {
	X = mmu.read(mmu.readWord(pc));
	pc += 2;
	lastInstructionTicks = 4;
}

void CPU::LD_NNm_X(byte X) {
	mmu.write(mmu.readWord(pc), X);
	pc += 2;
	lastInstructionTicks = 4;
}

void CPU::LD_X_N(byte& X) {
	X = mmu.read(pc);
	pc++;
	lastInstructionTicks = 2;
}

void CPU::LD_XYm_Z(byte X, byte Y, byte Z) {
	mmu.write((X << 8) | Y, Z);
	lastInstructionTicks = 2;
}

void CPU::LD_XYm_N(byte X, byte Y) {
	mmu.write((X << 8) | Y, mmu.read(pc));
	pc++;
	lastInstructionTicks = 3;
}

void CPU::LD_XYm_I_Z(byte& X, byte& Y, byte Z) {
	mmu.write((X << 8) | Y, Z);
	// Increment the LSB part of the address
	Y++;
	// If we overflowed then we increment the MSB part
	if (Y == 0) {
		X++;
	}
	lastInstructionTicks = 2;
}

void CPU::LD_X_YZm_I(byte& X, byte& Y, byte& Z) {
	X = mmu.read((Y << 8) | Z);
	// Increment the LSB part of the address
	Z++;
	// If we overflowed then we increment the MSB part
	if (Z == 0) {
		Y++;
	}
	lastInstructionTicks = 2;
}

void CPU::LD_X_YZm_D(byte& X, byte& Y, byte& Z) {
	X = mmu.read((Y << 8) | Z);
	// Decrement the MSB part of the address if it's set
	if (Y > 0) {
		Y--;
	}
	// Decrement the LSB part otherwise
	else {
		Z--;
	}
	lastInstructionTicks = 2;
}

void CPU::LD_XYm_D_Z(byte& X, byte& Y, byte& Z) {
	uint16_t XY = (X << 8) | Y;
	mmu.write(XY, Z);
	XY--;
	X = XY >> 8;
	Y = XY & 0xFF;
	//// Decrement the MSB part of the address if it's set
	//if (Y > 0) {
	//	Y--;
	//}
	//// Decrement the LSB part otherwise
	//else {
	//	X--;
	//}
	lastInstructionTicks = 2;
}

void CPU::LD_X_YZm(byte& X, byte Y, byte Z) {
	X = mmu.read((Y << 8) | Z);
	lastInstructionTicks = 2;
}

void CPU::LD_NNm_X(uint16_t& X) {
	mmu.writeWord(mmu.readWord(pc), X);
	pc += 2;
	lastInstructionTicks = 5;
}

void CPU::LD_X_Y(byte& X, byte Y) {
	X = Y;
	lastInstructionTicks = 1;
}

void CPU::INC_XY(byte& X, byte& Y) {
	// We increment the LSB
	Y++;
	// If we overflowed the LSB then we increment the MSB
	if (Y == 0) {
		X++;
	}
	lastInstructionTicks = 2;
}

void CPU::INC_XY(uint16_t& XY) {
	// We increment the value
	XY++;
	lastInstructionTicks = 2;
}

void CPU::INC_XYm(byte X, byte Y) {
	unsetFlag(FLAG_SUB);
	byte oldvalue = mmu.read((X << 8) | Y);
	byte value = oldvalue + 1;
	mmu.write((X << 8) | Y, value);
	// Set the ZERO flag if the value overflowed
	changeZeroValueFlag(value);
	// we check if we have an half carry
	if ((oldvalue & 0x10) == 0 && (value & 0x10) > 0) {
		setFlag(FLAG_HALF_CARRY);
	}

	lastInstructionTicks = 3;
}

void CPU::DEC_XYm(byte X, byte Y) {
	setFlag(FLAG_SUB);
	byte oldvalue = mmu.read((X << 8) | Y);
	byte value = oldvalue - 1;
	mmu.write((X << 8) | Y, value);
	// Set the ZERO flag if the value overflowed
	changeZeroValueFlag(value);
	// we check if we have an half carry
	setFlagIfTrue((oldvalue & 0x10) > 0 && (value & 0x10) == 0, FLAG_HALF_CARRY);
	lastInstructionTicks = 3;
}

void CPU::INC_X(byte& X) {
	unsetFlag(FLAG_SUB);

	X++;
	// If we overflowed we set the half carry and zero flags
	setFlagIfTrue((X == 0), FLAG_HALF_CARRY);
	changeZeroValueFlag(X);
	lastInstructionTicks = 1;
}

// TODO: check reset flags
void CPU::DEC_X(byte& X) {
	X--;
	// If we underflowed we set the half carry flag
	setFlagIfTrue((X == 0xFF), FLAG_HALF_CARRY);
	changeZeroValueFlag(X);
	setFlag(FLAG_SUB);
	lastInstructionTicks = 1;
}

void CPU::DEC_XY(byte& X, byte& Y) {
	uint16_t xy = (X << 8) | Y;
	xy--;
	X = (xy >> 8);
	Y = (xy & 0x00FF);
	lastInstructionTicks = 2;
}

void CPU::DEC_XY(uint16_t& XY) {
	XY--;
	lastInstructionTicks = 2;
}


void CPU::RLC_X(byte& X) {
	unsetFlag(FLAG_SUB);
	unsetFlag(FLAG_ZERO);
	unsetFlag(FLAG_HALF_CARRY);
	byte curFlag = f & FLAG_CARRY;
	/* Set the carry flag to the highest bit of X */
	setFlagIfTrue((X & 0x80) > 0, FLAG_CARRY);
	/* Rotate the accumulator and set the last bit to the original carry flag */
	X = ((X << 1) | (curFlag >> 4));
	changeZeroValueFlag(X);
	lastInstructionTicks = 2;
}

void CPU::RLC_XYm(byte X, byte Y) {
	byte value = mmu.read((X << 8) | Y);
	RLC_X(value);
	mmu.write((X << 8) | Y, value);
	lastInstructionTicks = 4;
}

void CPU::RRC_X(byte& X) {
	unsetFlag(FLAG_SUB);
	unsetFlag(FLAG_ZERO);
	unsetFlag(FLAG_HALF_CARRY);
	byte curFlag = f & FLAG_CARRY;
	/* Set the carry flag to the lowest bit of X */
	setFlagIfTrue((X & 0x01) > 0, FLAG_CARRY);
	/* Rotate the accumulator and set the first bit to the original carry flag */
	X = ((curFlag << 3) | (X >> 1));
	changeZeroValueFlag(X);
	lastInstructionTicks = 2;
}

void CPU::RRC_XYm(byte X, byte Y) {
	byte value = mmu.read((X << 8) | Y);
	RRC_X(value);
	// Write back the value to memory
	mmu.write((X << 8) | Y, value);
	lastInstructionTicks = 4;
}

void CPU::RL_X(byte& X) {
	unsetFlag(FLAG_SUB);
	unsetFlag(FLAG_ZERO);
	unsetFlag(FLAG_HALF_CARRY);
	setFlagIfTrue((X & 0x80) > 0, FLAG_CARRY);
	X = ((X << 1) | (X >> 7));
	changeZeroValueFlag(X);
	lastInstructionTicks = 2;
}

void CPU::RL_XYm(byte X, byte Y) {
	byte value = mmu.read((X << 8) | Y);
	RL_X(value);
	// Write back the value to memory
	mmu.write((X << 8) | Y, value);
	lastInstructionTicks = 4;
}

void CPU::RR_X(byte& X) {
	unsetFlag(FLAG_SUB);
	unsetFlag(FLAG_ZERO);
	unsetFlag(FLAG_HALF_CARRY);
	setFlagIfTrue((X & 0x01) > 0, FLAG_CARRY);
	X = ((X << 7) | (X >> 1));
	changeZeroValueFlag(X);
	lastInstructionTicks = 2;
}

void CPU::RR_XYm(byte X, byte Y) {
	byte value = mmu.read((X << 8) | Y);
	RR_X(value);
	// Write back the value to memory
	mmu.write((X << 8) | Y, value);
	lastInstructionTicks = 4;
}

void CPU::SLA_X(byte& X) {
	unsetFlag(FLAG_SUB);
	unsetFlag(FLAG_ZERO);
	unsetFlag(FLAG_HALF_CARRY);
	setFlagIfTrue((X & 0x80) > 0, FLAG_CARRY);
	X = (X << 1);
	changeZeroValueFlag(X);
	lastInstructionTicks = 2;
}

void CPU::SLA_XYm(byte X, byte Y) {
	byte value = mmu.read((X << 8) | Y);
	SLA_X(value);
	mmu.write((X << 8) | Y, value);
	lastInstructionTicks = 4;
}

void CPU::SRA_X(byte X) {
	unsetFlag(FLAG_SUB);
	unsetFlag(FLAG_ZERO);
	unsetFlag(FLAG_HALF_CARRY);
	unsetFlag(FLAG_CARRY);
	X = ((X & 0x80) | (X >> 1));
	changeZeroValueFlag(X);
	lastInstructionTicks = 2;
}

void CPU::SRA_XYm(byte X, byte Y) {
	byte value = mmu.read((X << 8) | Y);
	SRA_X(value);
	mmu.write((X << 8) | Y, value);
	lastInstructionTicks = 4;
}


void CPU::SRL_X(byte& X) {
	unsetFlag(FLAG_SUB);
	unsetFlag(FLAG_ZERO);
	unsetFlag(FLAG_HALF_CARRY);
	setFlagIfTrue((X & 0x01) > 0, FLAG_CARRY);
	X = (X >> 1);
	changeZeroValueFlag(X);
	lastInstructionTicks = 2;
}

void CPU::SRL_XYm(byte X, byte Y) {
	byte value = mmu.read((X << 8) | Y);
	SRL_X(value);
	mmu.write((X << 8) | Y, value);
	lastInstructionTicks = 4;
}


void CPU::SWAP_X(byte& X) {
	unsetFlag(FLAG_SUB);
	unsetFlag(FLAG_ZERO);
	unsetFlag(FLAG_HALF_CARRY);
	unsetFlag(FLAG_CARRY);
	// swap the two nibbles
	X = ((X >> 4) | (X << 4));
	changeZeroValueFlag(X);
	lastInstructionTicks = 2;
}

void CPU::SWAP_XYm(byte X, byte Y) {
	byte value = mmu.read((X << 8) | Y);
	SWAP_X(value);
	mmu.write((X << 8) | Y, value);
	lastInstructionTicks = 4;
}

void CPU::BIT_X_Y(byte X, byte Y) {
	unsetFlag(FLAG_SUB);
	setFlag(FLAG_HALF_CARRY);
	// If the value of the bit is set then we set the flag zero
	changeZeroValueFlag((Y >> X) & 0x01);
	lastInstructionTicks = 2;
}

void CPU::BIT_X_YZm(byte X, byte Y, byte Z) {
	byte value = mmu.read((Y << 8) | Z);
	BIT_X_Y(X, value);
	lastInstructionTicks = 4;
}

void CPU::RES_X_Y(byte X, byte& Y) {
	Y = (Y & ~(0x01 << X));
	lastInstructionTicks = 2;
}

void CPU::RES_X_YZm(byte X, byte Y, byte Z) {
	byte value = mmu.read((Y << 8) | Z);
	RES_X_Y(X, value);
	mmu.write((Y << 8) | Z, value);
	lastInstructionTicks = 4;
}

void CPU::SET_X_Y(byte X, byte& Y) {
	Y = (Y | (0x01 << X));
	lastInstructionTicks = 2;
}

void CPU::SET_X_YZm(byte X, byte Y, byte Z) {
	byte value = mmu.read((Y << 8) | Z);
	SET_X_Y(X, value);
	mmu.write((Y << 8) | Z, value);
	lastInstructionTicks = 4;
}


void CPU::ADD_WX_YZ(byte& W, byte& X, byte Y, byte Z) {
	unsetFlag(FLAG_SUB);
	uint32_t comp = (W << 8) | X;
	comp += (Y << 8) | Z;
	// If we overflowed we set the carry
	setFlagIfTrue(comp > 0xFFFF, FLAG_CARRY);
	// If we overflowed the lower part then we set the carry
	setFlagIfTrue(comp > X, FLAG_HALF_CARRY);
	W = ((comp & 0xFF00) >> 8);
	X = (comp & 0xFF);
	lastInstructionTicks = 2;
}

void CPU::ADD_XY_Z(byte& X, byte& Y, byte Z) {
	uint32_t comp = (X << 8) | Y;
	ADD_WX_YZ(X, Y, 0, Z);
	lastInstructionTicks = 2;
}

void CPU::ADD_X_Y(byte& X, byte Y) {
	unsetFlag(FLAG_SUB);
	byte value = X;
	X += Y;
	// we check for overflow
	setFlagIfTrue((value > X), FLAG_CARRY);
	// check for half carry
	setFlagIfTrue((value <= 0x0F) && (X > 0x0F), FLAG_HALF_CARRY);
	changeZeroValueFlag(X);
	lastInstructionTicks = 1;
}

void CPU::ADD_SP_X(sbyte X) {
	unsetFlag(FLAG_SUB);
	unsetFlag(FLAG_ZERO);
	uint16_t value = sp;
	sp += X;
	// we check for overflow
	setFlagIfTrue((value > sp), FLAG_CARRY);
	// check for half carry
	setFlagIfTrue((value <= 0x00FF) && (sp > 0x00FF), FLAG_HALF_CARRY);
	lastInstructionTicks = 4;
}

void CPU::ADD_X_N(byte& X) {
	ADD_X_Y(X, mmu.read(pc));
	lastInstructionTicks = 2;
}

void CPU::ADC_X_Y(byte& X, byte Y) {
	unsetFlag(FLAG_SUB);
	byte value = X;
	X += Y;
	if ((f & FLAG_CARRY) > 0) {
		X++;
	}
	// we check for overflow
	setFlagIfTrue((value > X), FLAG_CARRY);
	// check for half carry
	setFlagIfTrue((value <= 0x0F) && (X > 0x0F), FLAG_HALF_CARRY);
	changeZeroValueFlag(X);
	lastInstructionTicks = 1;
}

void CPU::ADC_X_N(byte X) {
	ADC_X_Y(X, mmu.read(pc));
	lastInstructionTicks = 2;
}

void CPU::ADD_X_YZm(byte& X, byte Y, byte Z) {
	ADD_X_Y(X, mmu.read((Y << 8) | Z));
	lastInstructionTicks = 2;
}

void CPU::ADC_X_YZm(byte& X, byte Y, byte Z) {
	ADC_X_Y(X, mmu.read((Y << 8) | Z));
	lastInstructionTicks = 2;
}


void CPU::SUB_X_Y(byte& X, byte Y) {
	setFlag(FLAG_SUB);
	byte value = X;
	X -= Y;
	// we check for overflow
	setFlagIfTrue((value < X), FLAG_CARRY);
	// check for half carry
	setFlagIfTrue((value > 0x0F) && (X <= 0x0F), FLAG_HALF_CARRY);
	changeZeroValueFlag(X);
	lastInstructionTicks = 1;
}

void CPU::SUB_X_N(byte& X) {
	SUB_X_Y(X, mmu.read(pc));
	lastInstructionTicks = 2;
}

void CPU::SBC_X_Y(byte& X, byte Y) {
	setFlag(FLAG_SUB);
	byte value = X;
	X -= Y;
	if ((f & FLAG_CARRY) > 0) {
		X--;
	}
	// we check for overflow
	setFlagIfTrue((value < X), FLAG_CARRY);
	// check for half carry
	setFlagIfTrue((value > 0x0F) && (X <= 0x0F), FLAG_HALF_CARRY);
	changeZeroValueFlag(X);
	lastInstructionTicks = 1;
}

void CPU::SBC_X_N(byte& X) {
	SBC_X_Y(X, mmu.read(pc));
	lastInstructionTicks = 2;
}

void CPU::SUB_X_YZm(byte& X, byte Y, byte Z) {
	SUB_X_Y(X, mmu.read((Y << 8) | Z));
	lastInstructionTicks = 2;
}

void CPU::SBC_X_YZm(byte& X, byte Y, byte Z) {
	SBC_X_Y(X, mmu.read((Y << 8) | Z));
	lastInstructionTicks = 2;
}

void CPU::AND_X(byte X) {
	unsetFlag(FLAG_SUB);
	setFlag(FLAG_HALF_CARRY);
	unsetFlag(FLAG_CARRY);
	a &= X;
	changeZeroValueFlag(a);
	lastInstructionTicks = 1;
}

void CPU::AND_XYm(byte X, byte Y) {
	AND_X(mmu.read((X << 8) | Y));
	lastInstructionTicks = 2;
}

void CPU::XOR_X(byte X) {
	unsetFlag(FLAG_SUB);
	unsetFlag(FLAG_HALF_CARRY);
	unsetFlag(FLAG_CARRY);
	a ^= X;
	changeZeroValueFlag(a);
	lastInstructionTicks = 1;
}

void CPU::XOR_XYm(byte X, byte Y) {
	XOR_X(mmu.read((X << 8) | Y));
	lastInstructionTicks = 2;
}

void CPU::OR_X(byte X) {
	unsetFlag(FLAG_SUB);
	unsetFlag(FLAG_HALF_CARRY);
	unsetFlag(FLAG_CARRY);
	a |= X;
	changeZeroValueFlag(a);
	lastInstructionTicks = 1;
}

void CPU::OR_XYm(byte X, byte Y) {
	OR_X(mmu.read((X << 8) | Y));
	lastInstructionTicks = 2;
}

void CPU::OR_N() {
	OR_X(mmu.read(pc));
	lastInstructionTicks = 2;
}

void CPU::AND_N() {
	AND_X(mmu.read(pc));
	lastInstructionTicks = 2;
}

void CPU::CP_X(byte X) {
	setFlag(FLAG_SUB);
	setFlagIfTrue(a == X, FLAG_ZERO);
	setFlagIfTrue(a > X, FLAG_HALF_CARRY);
	setFlagIfTrue(a < X, FLAG_CARRY);
	lastInstructionTicks = 1;
}

void CPU::CP_XYm(byte X, byte Y) {
	CP_X(mmu.read((X << 8) | Y));
	lastInstructionTicks = 2;
}

void CPU::POP_XY(byte X, byte Y) {
	Y = mmu.read(sp);
	X = mmu.read(sp + 1);
	sp += 2;
	lastInstructionTicks = 3;
}

void CPU::RET_X(bool cond) {
	uint16_t addr = mmu.readWord(sp);
	sp += 2;
	if (cond) {
		pc = addr;
		lastInstructionTicks = 5;
	}
	else {
		lastInstructionTicks = 2;
	}
}

void CPU::JP_X_NN(bool cond) {
	uint16_t addr = mmu.readWord(pc);
	pc += 2;
	if (cond) {
		pc = addr;
		lastInstructionTicks = 4;
	}
	else {
		lastInstructionTicks = 3;
	}
}

void CPU::JP_XYm(byte X, byte Y) {
	pc = mmu.read((X << 8) | Y);
	lastInstructionTicks = 1;
}

void CPU::CALL_X_NN(bool cond) {
	if (cond) {
	    sp -= 2;
	    mmu.writeWord(sp, pc+2);
		pc = mmu.read(pc);
		lastInstructionTicks = 6;
	}
	else {
		lastInstructionTicks = 3;
	}
}

void CPU::PUSH_XY(byte X, byte Y) {
	sp -= 2;
	mmu.write(sp, X);
	mmu.write(sp + 1, Y);
	lastInstructionTicks = 4;
}


void CPU::RST_X(byte X) {
	sp -= 2;
	mmu.writeWord(sp, pc);
	pc = X;
	lastInstructionTicks = 4;
}

void CPU::LDH_Nm_X(byte X) {
	mmu.write(0xFF00 | mmu.read(pc), X);
	pc++;
	lastInstructionTicks = 3;
}

void CPU::LDH_X_Nm(byte& X) {
	X = mmu.read(0xFF00 + mmu.read(pc));
	pc++;
	lastInstructionTicks = 3;
}

void CPU::LD_Xm_Y(byte X, byte Y) {
	mmu.write(0xFF00 | X, Y);
	lastInstructionTicks = 2;
}

void CPU::LD_X_Ym(byte& X, byte Y) {
	X = mmu.read(0xFF00 | Y);
	lastInstructionTicks = 2;
}

void CPU::JR_COND_N(bool condition) {
	int8_t addr = static_cast<int8_t>(mmu.read(pc));
	pc++;
	if (condition) {
		pc += addr;
		lastInstructionTicks = 3;
	}
	else {
		lastInstructionTicks = 2;
	}
}

void CPU::DAA_() {
	unsetFlag(FLAG_SUB);

	if (((a & 0x0F) > 9) || (f & FLAG_HALF_CARRY) > 0) {
		a += 0x06;
		setFlag(FLAG_HALF_CARRY);
	}
	else {
		unsetFlag(FLAG_HALF_CARRY);
	}

	if (((a >> 4) > 9) || (f & FLAG_CARRY) > 0) {
		a += 0x60;
		setFlag(FLAG_CARRY);
	}
	else {
		unsetFlag(FLAG_CARRY);
	}
	lastInstructionTicks = 1;
}

void CPU::CPL_() {
	// a = ~a
	a ^= 0xFF;
	setFlag(FLAG_SUB);
	setFlag(FLAG_HALF_CARRY);
	lastInstructionTicks = 1;
}

void CPU::SCF_() {
	unsetFlag(FLAG_HALF_CARRY);
	unsetFlag(FLAG_SUB);
	setFlag(FLAG_CARRY);
	lastInstructionTicks = 1;
}

void CPU::CCF_() {
	unsetFlag(FLAG_HALF_CARRY);
	unsetFlag(FLAG_SUB);
	setFlagIfTrue((f & FLAG_CARRY) == 0, FLAG_CARRY);
	lastInstructionTicks = 1;
}

/*
void CPU::LDH_Xm_Y(byte X, byte Y) {
	mmu.write(mmu.read(0xFF00 + mmu.read(pc)), X);
	mmu.write(0xFF00 + mmu.read(X), Y);
	lastInstructionTicks = 3;
}

void CPU::LDH_X_Nm(byte& X) {
	(X = mmu.read(0xFF00 + mmu.read(pc));
	pc++;
	lastInstructionTicks = 3;
}*/

static int instruction = 0;


void CPU::process(const byte& opCode) {
	switch (opCode) {

		/******************************************************/
		/************************ 0x00 ************************/
		/******************************************************/

	case NOP:
            lastInstructionTicks = 1;
		break;

	case LD_BC_nn:
		LD_XY_NN(b, c);
		break;

	case LD_BCm_A:
		LD_XYm_Z(b, c, a);
		break;

	case INC_BC:
		INC_XY(b, c);
		break;

	case INC_B:
		INC_X(b);
		break;

	case DEC_B:
		DEC_X(b);
		break;

	case LD_B_n:
		LD_X_N(b);
		break;

	case RLC_A:
		RLC_X(a);
		break;

	case LD_nnm_SP:
		LD_NNm_X(sp);
		break;

	case ADD_HL_BC:
		ADD_WX_YZ(h, l, b, c);
		break;

	case LD_A_BCm:
		LD_X_YZm(a, b, c);
		break;

	case DEC_BC:
		DEC_XY(b, c);
		break;

	case INC_C:
		INC_X(c);
		break;

	case DEC_C:
		DEC_X(c);
		break;

	case LD_C_n:
		LD_X_N(c);
		break;

	case RRC_A:
		RRC_X(a);
		break;

		/******************************************************/
		/************************ 0x10 ************************/
		/******************************************************/

	case STOP:
		//TODO:
		// Stop cpu and gpu and wait for button to be pressed
		lastInstructionTicks = 2;
		break;

	case LD_DE_nn:
		LD_XY_NN(d, e);
		break;

	case LD_DEm_A:
		LD_XYm_Z(d, e, a);
		break;

	case INC_DE:
		INC_XY(d, e);
		break;

	case INC_D:
		INC_X(d);
		break;

	case DEC_D:
		DEC_X(d);
		break;

	case LD_D_n:
		LD_X_N(d);
		break;

	case RL_A:
		RL_X(a);
		break;

	case JR_n:
		JR_COND_N(true);
		break;

	case ADD_HL_DE:
		ADD_WX_YZ(h, l, d, e);
		break;

	case LD_A_DEm:
		LD_X_YZm(a, d, e);
		break;

	case DEC_DE:
		DEC_XY(d, e);
		break;

	case INC_E:
		INC_X(e);
		break;

	case DEC_E:
		DEC_X(e);
		break;

	case LD_E_n:
		LD_X_N(e);
		break;

	case RR_A:
		RR_X(a);
		break;

		/******************************************************/
		/************************ 0x20 ************************/
		/******************************************************/

	case JR_NZ_n:
		JR_COND_N((f & FLAG_ZERO) == 0);
		break;

	case LD_HL_nn:
		LD_XY_NN(h, l);
		break;

	case LD_HLm_I_A:
		LD_XYm_I_Z(h, l, a);
		break;

	case INC_HL:
		INC_XY(h, l);
		break;

	case INC_H:
		INC_X(h);
		break;

	case DEC_H:
		DEC_X(h);
		break;

	case LD_H_n:
		LD_X_N(h);
		break;

	case DAA:
		DAA_();
		break;

	case JR_Z_n:
		JR_COND_N((f & FLAG_ZERO) > 0);
		break;

	case ADD_HL_HL:
		ADD_WX_YZ(h, l, h, l);
		break;

	case LD_A_HLm_I:
		LD_X_YZm_I(a, h, l);
		break;

	case DEC_HL:
		DEC_XY(h, l);
		break;

	case INC_L:
		INC_X(l);
		break;

	case DEC_L:
		DEC_X(l);
		break;

	case LD_L_n:
		LD_X_N(l);
		break;

	case CPL:
		CPL_();
		break;

		/******************************************************/
		/************************ 0x30 ************************/
		/******************************************************/

	case JR_NC_n:
		JR_COND_N((f & FLAG_CARRY) == 0);
		break;

	case LD_SP_nn:
		LD_X_NN(sp);
		break;

	case LD_HLm_D_A:
		LD_XYm_D_Z(h, l, a);
		break;

	case INC_SP:
		INC_XY(sp);
		break;

	case INC_HLm:
		INC_XYm(h, l);
		break;

	case DEC_HLm:
		DEC_XYm(h, l);
		break;

	case LD_HLm_n:
		LD_XYm_N(h, l);
		break;

	case SCF:
		SCF_();
		break;

	case JR_C_n:
		JR_COND_N((f & FLAG_CARRY) > 0);
		break;

	case ADD_HL_SP:
		ADD_WX_YZ(h, l, (sp << 8), (sp & 0x00FF));
		break;

	case LD_A_HLm_D:
		LD_X_YZm_D(a, h, l);
		break;

	case DEC_SP:
		DEC_XY(sp);
		break;

	case INC_A:
		INC_X(a);
		break;

	case DEC_A:
		DEC_X(a);
		break;

	case LD_A_n:
		LD_X_N(a);
		break;

	case CCF:
		CCF_();
		break;

		/******************************************************/
		/************************ 0x40 ************************/
		/******************************************************/

	case LD_B_B:
		LD_X_Y(b, b);
		break;

	case LD_B_C:
		LD_X_Y(b, c);
		break;

	case LD_B_D:
		LD_X_Y(b, d);
		break;

	case LD_B_E:
		LD_X_Y(b, e);
		break;

	case LD_B_H:
		LD_X_Y(b, h);
		break;

	case LD_B_L:
		LD_X_Y(b, l);
		break;

	case LD_B_HLm:
		LD_X_YZm(b, h, l);
		break;

	case LD_B_A:
		LD_X_Y(b, a);
		break;

	case LD_C_B:
		LD_X_Y(c, b);
		break;

	case LD_C_C:
		LD_X_Y(c, c);
		break;

	case LD_C_D:
		LD_X_Y(c, d);
		break;

	case LD_C_E:
		LD_X_Y(c, e);
		break;

	case LD_C_H:
		LD_X_Y(c, h);
		break;

	case LD_C_L:
		LD_X_Y(c, l);
		break;

	case LD_C_HLm:
		LD_X_YZm(c, h, l);
		break;

	case LD_C_A:
		LD_X_Y(c, a);
		break;


		/******************************************************/
		/************************ 0x50 ************************/
		/******************************************************/

	case LD_D_B:
		LD_X_Y(d, b);
		break;

	case LD_D_C:
		LD_X_Y(d, c);
		break;

	case LD_D_D:
		LD_X_Y(d, d);
		break;

	case LD_D_E:
		LD_X_Y(d, e);
		break;

	case LD_D_H:
		LD_X_Y(d, h);
		break;

	case LD_D_L:
		LD_X_Y(d, l);
		break;

	case LD_D_HLm:
		LD_X_YZm(d, h, l);
		break;

	case LD_D_A:
		LD_X_Y(d, a);
		break;

	case LD_E_B:
		LD_X_Y(e, b);
		break;

	case LD_E_C:
		LD_X_Y(e, c);
		break;

	case LD_E_D:
		LD_X_Y(e, d);
		break;

	case LD_E_E:
		LD_X_Y(e, e);
		break;

	case LD_E_H:
		LD_X_Y(e, h);
		break;

	case LD_E_L:
		LD_X_Y(e, l);
		break;

	case LD_E_HLm:
		LD_X_YZm(e, h, l);
		break;

	case LD_E_A:
		LD_X_Y(e, a);
		break;

		/******************************************************/
		/************************ 0x60 ************************/
		/******************************************************/

	case LD_H_B:
		LD_X_Y(h, b);
		break;

	case LD_H_C:
		LD_X_Y(h, c);
		break;

	case LD_H_D:
		LD_X_Y(h, d);
		break;

	case LD_H_E:
		LD_X_Y(h, e);
		break;

	case LD_H_H:
		LD_X_Y(h, h);
		break;

	case LD_H_L:
		LD_X_Y(h, l);
		break;

	case LD_H_HLm:
		LD_X_YZm(h, h, l);
		break;

	case LD_H_A:
		LD_X_Y(h, a);
		break;

	case LD_L_B:
		LD_X_Y(l, b);
		break;

	case LD_L_C:
		LD_X_Y(l, c);
		break;

	case LD_L_D:
		LD_X_Y(l, d);
		break;

	case LD_L_E:
		LD_X_Y(l, e);
		break;

	case LD_L_H:
		LD_X_Y(l, h);
		break;

	case LD_L_L:
		LD_X_Y(l, l);
		break;

	case LD_L_HLm:
		LD_X_YZm(l, h, l);
		break;

	case LD_L_A:
		LD_X_Y(l, a);
		break;

		/******************************************************/
		/************************ 0x70 ************************/
		/******************************************************/

	case LD_HLm_B:
		LD_XYm_Z(h, l, b);
		break;

	case LD_HLm_C:
		LD_XYm_Z(h, l, c);
		break;

	case LD_HLm_D:
		LD_XYm_Z(h, l, d);
		break;

	case LD_HLm_E:
		LD_XYm_Z(h, l, e);
		break;

	case LD_HLm_H:
		LD_XYm_Z(h, l, h);
		break;

	case LD_HLm_L:
		LD_XYm_Z(h, l, l);
		break;

	case HALT:
		halted = true;
		lastInstructionTicks = 1;
		break;

	case LD_HLm_A:
		LD_XYm_Z(h, l, a);
		break;

	case LD_A_B:
		LD_X_Y(a, b);
		break;

	case LD_A_C:
		LD_X_Y(a, c);
		break;

	case LD_A_D:
		LD_X_Y(a, d);
		break;

	case LD_A_E:
		LD_X_Y(a, e);
		break;

	case LD_A_H:
		LD_X_Y(a, h);
		break;

	case LD_A_L:
		LD_X_Y(a, l);
		break;

	case LD_A_HLm:
		LD_X_YZm(a, h, l);
		break;

	case LD_A_A:
		LD_X_Y(a, a);
		break;

		/******************************************************/
		/************************ 0x80 ************************/
		/******************************************************/

	case ADD_A_B:
		ADD_X_Y(a, b);
		break;

	case ADD_A_C:
		ADD_X_Y(a, c);
		break;

	case ADD_A_D:
		ADD_X_Y(a, d);
		break;

	case ADD_A_E:
		ADD_X_Y(a, e);
		break;

	case ADD_A_H:
		ADD_X_Y(a, h);
		break;

	case ADD_A_L:
		ADD_X_Y(a, l);
		break;

	case ADD_A_HLm:
		ADD_X_YZm(a, h, l);
		break;

	case ADD_A_A:
		ADD_X_Y(a, a);
		break;

	case ADC_A_B:
		ADC_X_Y(a, b);
		break;

	case ADC_A_C:
		ADC_X_Y(a, c);
		break;

	case ADC_A_D:
		ADC_X_Y(a, d);
		break;

	case ADC_A_E:
		ADC_X_Y(a, e);
		break;

	case ADC_A_H:
		ADC_X_Y(a, h);
		break;

	case ADC_A_L:
		ADC_X_Y(a, l);
		break;

	case ADC_A_HLm:
		ADC_X_YZm(a, h, l);
		break;

	case ADC_A_A:
		ADC_X_Y(a, a);
		break;

		/******************************************************/
		/************************ 0x90 ************************/
		/******************************************************/

	case SUB_A_B:
		SUB_X_Y(a, b);
		break;

	case SUB_A_C:
		SUB_X_Y(a, c);
		break;

	case SUB_A_D:
		SUB_X_Y(a, d);
		break;

	case SUB_A_E:
		SUB_X_Y(a, e);
		break;

	case SUB_A_H:
		SUB_X_Y(a, h);
		break;

	case SUB_A_L:
		SUB_X_Y(a, l);
		break;

	case SUB_A_HLm:
		SUB_X_YZm(a, h, l);
		break;

	case SUB_A_A:
		SUB_X_Y(a, a);
		break;

	case SBC_A_B:
		SBC_X_Y(a, b);
		break;

	case SBC_A_C:
		SBC_X_Y(a, c);
		break;

	case SBC_A_D:
		SBC_X_Y(a, d);
		break;

	case SBC_A_E:
		SBC_X_Y(a, e);
		break;

	case SBC_A_H:
		SBC_X_Y(a, h);
		break;

	case SBC_A_L:
		SBC_X_Y(a, l);
		break;

	case SBC_A_HLm:
		SBC_X_YZm(a, h, l);
		break;

	case SBC_A_A:
		ADC_X_Y(a, a);
		break;

		/******************************************************/
		/************************ 0xA0 ************************/
		/******************************************************/

	case AND_B:
		AND_X(b);
		break;

	case AND_C:
		AND_X(c);
		break;

	case AND_D:
		AND_X(d);
		break;

	case AND_E:
		AND_X(e);
		break;

	case AND_H:
		AND_X(h);
		break;

	case AND_L:
		AND_X(l);
		break;

	case AND_HLm:
		AND_XYm(h, l);
		break;

	case AND_A:
		AND_X(a);
		break;

	case XOR_B:
		XOR_X(b);
		break;

	case XOR_C:
		XOR_X(c);
		break;

	case XOR_D:
		XOR_X(d);
		break;

	case XOR_E:
		XOR_X(e);
		break;

	case XOR_H:
		XOR_X(h);
		break;

	case XOR_L:
		XOR_X(l);
		break;

	case XOR_HLm:
		XOR_XYm(h, l);
		break;

	case XOR_A:
		XOR_X(a);
		break;

		/******************************************************/
		/************************ 0xB0 ************************/
		/******************************************************/

	case OR_B:
		OR_X(b);
		break;

	case OR_C:
		OR_X(c);
		break;

	case OR_D:
		OR_X(d);
		break;

	case OR_E:
		OR_X(e);
		break;

	case OR_H:
		OR_X(h);
		break;

	case OR_L:
		OR_X(l);
		break;

	case OR_HLm:
		OR_XYm(h, l);
		break;

	case OR_A:
		OR_X(a);
		break;

	case CP_B:
		CP_X(b);
		break;

	case CP_C:
		CP_X(c);
		break;

	case CP_D:
		CP_X(d);
		break;

	case CP_E:
		CP_X(e);
		break;

	case CP_H:
		CP_X(h);
		break;

	case CP_L:
		CP_X(l);
		break;

	case CP_HLm:
		CP_XYm(h, l);
		break;

	case CP_A:
		CP_X(a);
		break;


		/******************************************************/
		/************************ 0xC0 ************************/
		/******************************************************/

	case RET_NZ:
		RET_X((f & FLAG_ZERO) == 0);
		break;

	case POP_BC:
		POP_XY(b, c);
		break;

	case JP_NZ_nn:
		JP_X_NN((f & FLAG_ZERO) == 0);
		break;

	case JP_nn:
		JP_X_NN(true);
		break;

	case CALL_NZ_nn:
		CALL_X_NN((f & FLAG_ZERO) == 0);
		break;

	case PUSH_BC:
		PUSH_XY(b, c);
		break;

	case ADD_A_n:
		ADD_X_N(a);
		break;

	case RST_0:
		RST_X(0x00);
		break;

	case RET_Z:
		RET_X((f & FLAG_ZERO) > 0);
		break;

	case RET:
		RET_X(true);
		break;

	case JP_Z_nn:
		JP_X_NN((f & FLAG_ZERO) > 0);
		break;

	case EXT_OPS:
		processExtended(mmu.read(pc++));
		break;

	case CALL_Z_nn:
		CALL_X_NN((f & FLAG_ZERO) > 0);
		break;

	case CALL_nn:
		CALL_X_NN(true);
		break;

	case ADC_A_n:
		ADC_X_N(a);
		break;

	case RST_8:
		RST_X(0x08);
		break;

		/******************************************************/
		/************************ 0xD0 ************************/
		/******************************************************/

	case RET_NC:
		RET_X((f & FLAG_CARRY) == 0);
		break;

	case POP_DE:
		POP_XY(d, e);
		break;

	case JP_NC_nn:
		JP_X_NN((f & FLAG_CARRY) == 0);
		break;

	case CALL_NC_nn:
		CALL_X_NN((f & FLAG_CARRY) == 0);
		break;

	case PUSH_DE:
		PUSH_XY(d, e);
		break;

	case SUB_A_n:
		SUB_X_N(a);
		break;

	case RST_10:
		RST_X(0x10);
		break;

	case RET_C:
		RET_X((f & FLAG_CARRY) > 0);
		break;

	case RETI:
		RET_X(true);
		interrupts = true;
		break;

	case JP_C_nn:
		JP_X_NN((f & FLAG_CARRY) > 0);
		break;

	case CALL_C_nn:
		CALL_X_NN((f & FLAG_CARRY) > 0);
		break;

	case SBC_A_n:
		SBC_X_N(a);
		break;

	case RST_18:
		RST_X(0x18);
		break;

		/******************************************************/
		/************************ 0xE0 ************************/
		/******************************************************/

	case LDH_nm_A:
		LDH_Nm_X(a);
		break;

	case POP_HL:
		POP_XY(h, l);
		break;

	case LD_Cm_A:
		LD_Xm_Y(c, a);
		break;

	case PUSH_HL:
		PUSH_XY(h, l);
		break;

	case AND_n:
		AND_N();
		break;

	case RST_20:
		RST_X(0x20);
		break;

	case ADD_SP_d:
		ADD_SP_X(static_cast<sbyte>(mmu.read(pc++)));
		break;

	case JP_HLm:
		JP_XYm(h, l);
		break;

	case LD_nnm_A:
		LD_NNm_X(a);
		break;

	case XOR_n:
		XOR_X(mmu.read(pc++));
		break;

	case RST_28:
		RST_X(0x28);
		break;
		/******************************************************/
		/************************ 0xF0 ************************/
		/******************************************************/

	case LDH_A_nm:
		LDH_X_Nm(a);
		break;

	case POP_AF:
		POP_XY(a, f);
		break;

	case LD_A_Cm:
		LD_Xm_Y(h, l);
		break;

	case DI:
		ticksBeforeDisablingInterrupts = 2;
		lastInstructionTicks = 1;
		break;

	case PUSH_AF:
		PUSH_XY(a, f);
		break;

	case OR_n:
		OR_N();
		break;

	case RST_30:
		RST_X(0x30);
		break;

	case LDHL_SP_d:
		LD_XY_Z_N(h, l, sp);
		break;

	case LD_SP_HL:
		mmu.writeWord(sp, (h << 8) | l);
		lastInstructionTicks = 2;
		break;

	case LD_A_nnm:
		LD_X_NNm(a);
		break;

	case EI:
		ticksBeforeEnablingInterrupts = 2;
		lastInstructionTicks = 1;
		break;

	case CP_n:
		CP_X(mmu.read(pc++));
		break;

	case RST_38:
		RST_X(0x38);
		break;

	default:
		std::cerr << "OPCODE :" << std::hex << static_cast<int>(opCode) << " not implemented" << std::endl;
		return;
	}
}



void CPU::processExtended(const byte& opCode) {
	switch (opCode) {

		/******************************************************/
		/************************ 0x00 ************************/
		/******************************************************/

	case RLC_B:
		RLC_X(b);
		break;

	case RLC_C:
		RLC_X(c);
		break;

	case RLC_D:
		RLC_X(d);
		break;

	case RLC_E:
		RLC_X(e);
		break;

	case RLC_H:
		RLC_X(h);
		break;

	case RLC_HLm:
		RLC_XYm(h, l);
		break;

	case RLC_A:
		RLC_X(a);
		break;

	case RRC_B:
		RRC_X(b);
		break;

	case RRC_C:
		RRC_X(c);
		break;

	case RRC_D:
		RRC_X(d);
		break;

	case RRC_E:
		RRC_X(e);
		break;

	case RRC_H:
		RRC_X(h);
		break;

	case RRC_HLm:
		RRC_XYm(h, l);
		break;

	case RRC_A:
		RRC_X(a);
		break;

		/******************************************************/
		/************************ 0x10 ************************/
		/******************************************************/

	case RL_B:
		RL_X(b);
		break;

	case RL_C:
		RL_X(c);
		break;

	case RL_D:
		RL_X(d);
		break;

	case RL_E:
		RL_X(e);
		break;

	case RL_H:
		RL_X(h);
		break;

	case RL_HLm:
		RL_XYm(h, l);
		break;

	case RL_A:
		RL_X(a);
		break;

	case RR_B:
		RR_X(b);
		break;

	case RR_C:
		RR_X(c);
		break;

	case RR_D:
		RR_X(d);
		break;

	case RR_E:
		RR_X(e);
		break;

	case RR_H:
		RR_X(h);
		break;

	case RR_HLm:
		RR_XYm(h, l);
		break;

	case RR_A:
		RR_X(a);
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
		std::cerr << "OPCODE EXTENDED :" << std::hex << static_cast<int>(opCode) << " not implemented" << std::endl;
		return;
	}
}
