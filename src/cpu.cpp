#include <iostream>
#include "cpu.hpp"
#include "instructions.hpp"
#include "mmu.hpp"

/* RESSOURCES:
	http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
	http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
	http://www.8052.com/
*/

CPU::CPU(MMU* mmu_) {
	tick = 0;
	lastInstructionTicks = 0;
	pc = 0;
	sp = 0;
	mmu = mmu_;
}

void CPU::exec() {
	process(mmu->read(pc));
	pc++;
	tick += lastInstructionTicks;
}

CPU::~CPU() {

}

#define LD_XY_NN(X, Y) 	Y = mmu->read(pc); \
						X = mmu->read(pc+1); \
						pc += 2; \
						lastInstructionTicks = 3;

#define LD_X_NN(X) 		X = mmu->read(pc); \
						pc += 2; \
						lastInstructionTicks = 3;

#define LD_X_N(X) 		X = mmu->read(pc); \
						pc++; \
						lastInstructionTicks = 2;

#define LD_XYm_Z(X, Y, Z)	mmu->write((X << 8) | Y, Z); \
							lastInstructionTicks = 2;

#define LD_XYm_N(X, Y)		mmu->write((X << 8) | Y, mmu->read(pc)); \
							pc++; \
							lastInstructionTicks = 3;

#define LD_XYm_I_Z(X, Y, Z)		mmu->write((X << 8) | Y, Z); \
								Y++; \
								if (Y == 0)	X++; \
								lastInstructionTicks = 2;

#define LD_X_YZm_I(X, Y, Z)		X = mmu->read((Y << 8) | Z); \
								Z++; \
								if (Z == 0)	Y++; \
								lastInstructionTicks = 2;

#define LD_X_YZm_D(X, Y, Z)		X = mmu->read((Y << 8) | Z); \
								if (Y > 0) Y--; \
								else Z--; \
								lastInstructionTicks = 2;
							
#define LD_XYm_D_Z(X, Y, Z)		mmu->write((X << 8) | Y, Z); \
								if (Y > 0) Y--; \
								else Z--; \
								lastInstructionTicks = 2;

#define LD_X_YZm(X, Y, Z) 	X = mmu->read((Y << 8) | Z); \
							lastInstructionTicks = 1;

#define LD_NNm_X(X) 	{ \
							uint16_t addr = ((mmu->read(pc) << 8) & mmu->read(pc+1)); \
							X = mmu->read(addr); \
							pc += 2; \
							lastInstructionTicks = 5; \
						}

#define LD_X_Y(X, Y) 		X = Y; \
							lastInstructionTicks = 1;

#define INC_XY(X, Y) 	Y++; \
						if (Y == 0) X++; \
						lastInstructionTicks = 2;

#define INC_XYm(X, Y) 	{ \
						byte value = mmu->read((X << 8) | Y); \
						value++; \
						mmu->write((X << 8) | Y, value); \
						if (value == 0) f |= FLAG_ZERO; \
						else if (!((value-1) & 0x10) && (value & 0x10)) f |= FLAG_HALF_CARRY; \
						f &= RESET_SUB; \
						lastInstructionTicks = 3; \
						} \

#define DEC_XYm(X, Y) 	{ \
						byte value = mmu->read((X << 8) | Y); \
						value--; \
						mmu->write((X << 8) | Y, value); \
						if (value == 0) f |= FLAG_ZERO; \
						else if (((value+1) & 0x10) && !(value & 0x10)) f |= FLAG_HALF_CARRY; /* TODO: NOT SURE ABOUT THIS */ \
						f |= RESET_SUB; \
						lastInstructionTicks = 3; \
						} \

#define INC_X(X) 		X++; \
						if (X == 0) f |= FLAG_HALF_CARRY & FLAG_ZERO; \
						f &= RESET_SUB; \
						lastInstructionTicks = 1;

#define DEC_X(X) 		X--; \
						if (X == 255) f |= FLAG_HALF_CARRY; \
						else f &= FLAG_HALF_CARRY; \
						if (X == 0) f |= FLAG_ZERO; \
						else f &= FLAG_ZERO; \
						f |= FLAG_SUB; \
						lastInstructionTicks = 1;

#define DEC_XY(X, Y) 	{ \
							uint16_t xy = (X << 8) | Y; \
							xy--; \
							X = (xy >> 8); \
							Y = xy; \
							lastInstructionTicks = 2; \
						}


#define RLC_X(X)		{ \
							byte curFlag = f & FLAG_CARRY; \
							f = ((X & 0x80) >> 3) & RESET_SUB & RESET_ZERO & RESET_HALF_CARRY; /* Set the carry flag to the highest bit of X */ \
							X = ((X << 1) | (curFlag >> 4)); /* Rotate the accumulator and set the last bit to the original carry flag */ \
							lastInstructionTicks = 1; \
						}

#define RRC_X(X)		{ \
							byte curFlag = f & FLAG_CARRY; \
							f = ((X & 0x01) >> 3) & RESET_SUB & RESET_ZERO & RESET_HALF_CARRY; /* Set the carry flag to the lowest bit of X */ \
							X = ((curFlag << 3) | (X >> 1)); /* Rotate the accumulator and set the first bit to the original carry flag */ \
							lastInstructionTicks = 1; \
						}

// TODO: Carry ?
#define RL_X(X)				f = f & RESET_SUB & RESET_ZERO & RESET_HALF_CARRY; \
							X = ((X << 1) | (X >> 7)); \
							lastInstructionTicks = 1; \

#define RR_X(X)				f = f & RESET_SUB & RESET_ZERO & RESET_HALF_CARRY; \
							X = ((X << 7) | (X >> 1)); \
							lastInstructionTicks = 1; \


#define ADD_WX_YZ(W, X, Y, Z) { \
							uint32_t comp = (W << 8) | X; \
							comp += (Y << 8) | Z; \
							if (comp > 0xFFFF) f |= FLAG_CARRY; \
							else f &= RESET_CARRY; \
							if ((X << 8) > 0xFFFF) f |= FLAG_CARRY; \
							else f &= RESET_CARRY; \
							f &= RESET_SUB; \
							W = ((comp & 0xFF00) >> 8); \
							X = (comp & 0xFF); \
							lastInstructionTicks = 2; \
						}

#define ADD_XY_Z(X, Y, Z) { \
							uint32_t comp = (X << 8) | Y; \
							comp += Z; \
							if (comp > 0xFFFF) f |= FLAG_CARRY; \
							else f &= RESET_CARRY; \
							if ((X << 8) > 0xFFFF) f |= FLAG_CARRY; \
							else f &= RESET_CARRY; \
							f &= RESET_SUB; \
							X = ((comp & 0xFF00) >> 8); \
							Y = (comp & 0xFF); \
							lastInstructionTicks = 2; \
						}

#define ADD_X_Y(X, Y) { \
						byte value = X; \
						X += Y; \
						if (!X) f |= FLAG_CARRY | FLAG_ZERO; \
						else f &= RESET_CARRY & RESET_ZERO; \
						if (!(value & 0x10) && (X & 0x10)) f |= FLAG_HALF_CARRY; \
						else f &= FLAG_HALF_CARRY; \
						f &= RESET_SUB; \
						lastInstructionTicks = 1; \
					}

#define ADC_X_Y(X, Y) { \
						byte value = X; \
						X += Y; \
						if (f & FLAG_CARRY) X++; \
						if (!X) f |= FLAG_CARRY | FLAG_ZERO; \
						else f &= RESET_CARRY & RESET_ZERO; \
						if (!(value & 0x10) && (X & 0x10)) f |= FLAG_HALF_CARRY; \
						else f &= FLAG_HALF_CARRY; \
						f &= RESET_SUB; \
						lastInstructionTicks = 1; \
					}

#define ADD_X_YZm(X, Y, Z) { \
						byte value = X; \
						X += mmu->read((Y << 8) | Z); \
						if (!X) f |= FLAG_CARRY | FLAG_ZERO; \
						else f &= RESET_CARRY & RESET_ZERO; \
						if (!(value & 0x10) && (X & 0x10)) f |= FLAG_HALF_CARRY; \
						else f &= FLAG_HALF_CARRY; \
						f &= RESET_SUB; \
						lastInstructionTicks = 1; \
					}

#define ADC_X_YZm(X, Y, Z) { \
						byte value = X; \
						X += mmu->read((Y << 8) | Z); \
						if (f & FLAG_CARRY) X++; \
						if (!X) f |= FLAG_CARRY | FLAG_ZERO; \
						else f &= RESET_CARRY & RESET_ZERO; \
						if (!(value & 0x10) && (X & 0x10)) f |= FLAG_HALF_CARRY; \
						else f &= FLAG_HALF_CARRY; \
						f &= RESET_SUB; \
						lastInstructionTicks = 1; \
					}


#define SUB_X_Y(X, Y) { \
						byte value = X; \
						X -= Y; \
						if (!X) f |= FLAG_CARRY | FLAG_ZERO; \
						else f &= RESET_CARRY & RESET_ZERO; \
						if (!(value & 0x10) && (X & 0x10)) f |= FLAG_HALF_CARRY; \
						else f &= FLAG_HALF_CARRY; \
						f |= FLAG_SUB; \
						lastInstructionTicks = 1; \
					}

#define SBC_X_Y(X, Y) { \
						byte value = X; \
						X -= Y; \
						if (f & FLAG_CARRY) X--; \
						if (!X) f |= FLAG_CARRY | FLAG_ZERO; \
						else f &= RESET_CARRY & RESET_ZERO; \
						if (!(value & 0x10) && (X & 0x10)) f |= FLAG_HALF_CARRY; \
						else f &= FLAG_HALF_CARRY; \
						f |= FLAG_SUB; \
						lastInstructionTicks = 1; \
					}

#define SUB_X_YZm(X, Y, Z) { \
						byte value = X; \
						X -= mmu->read((Y << 8) | Z); \
						if (!X) f |= FLAG_CARRY | FLAG_ZERO; \
						else f &= RESET_CARRY & RESET_ZERO; \
						if (!(value & 0x10) && (X & 0x10)) f |= FLAG_HALF_CARRY; \
						else f &= FLAG_HALF_CARRY; \
						f |= FLAG_SUB; \
						lastInstructionTicks = 2; \
					}

#define SBC_X_YZm(X, Y, Z) { \
						byte value = X; \
						X -= mmu->read((Y << 8) | Z); \
						if (f & FLAG_CARRY) X++; \
						if (!X) f |= FLAG_CARRY | FLAG_ZERO; \
						else f &= RESET_CARRY & RESET_ZERO; \
						if (!(value & 0x10) && (X & 0x10)) f |= FLAG_HALF_CARRY; \
						else f &= FLAG_HALF_CARRY; \
						f |= FLAG_SUB; \
						lastInstructionTicks = 2; \
					}

#define AND_X(X)	a &= X; \
					if (!a) f |= FLAG_ZERO; \
					else f &= RESET_ZERO; \
					f |= FLAG_HALF_CARRY; \
					f &= RESET_CARRY & RESET_SUB; \
					lastInstructionTicks = 1;

#define AND_XYm(X, Y)	a &= mmu->read((X << 8) | Y); \
						if (!a) f |= FLAG_ZERO; \
						else f &= RESET_ZERO; \
						f |= FLAG_HALF_CARRY; \
						f &= RESET_CARRY & RESET_SUB; \
						lastInstructionTicks = 2;

#define XOR_X(X)	a ^= X; \
					if (!a) f |= FLAG_ZERO; \
					else f &= RESET_ZERO; \
					f &= RESET_HALF_CARRY & RESET_CARRY & RESET_SUB; \
					lastInstructionTicks = 1;

#define XOR_XYm(X, Y)	a ^= mmu->read((X << 8) | Y); \
						if (!a) f |= FLAG_ZERO; \
						else f &= RESET_ZERO; \
						f &= RESET_HALF_CARRY & RESET_CARRY & RESET_SUB; \
						lastInstructionTicks = 2;

#define OR_X(X)		a |= X; \
					if (!a) f |= FLAG_ZERO; \
					else f &= RESET_ZERO; \
					f &= RESET_HALF_CARRY & RESET_CARRY & RESET_SUB; \
					lastInstructionTicks = 1;

#define OR_XYm(X, Y)	a |= mmu->read((X << 8) | Y); \
						if (!a) f |= FLAG_ZERO; \
						else f &= RESET_ZERO; \
						f &= RESET_HALF_CARRY & RESET_CARRY & RESET_SUB; \
						lastInstructionTicks = 2;

#define CP_X(X)		f &= RESET_SUB & RESET_ZERO & RESET_CARRY & RESET_HALF_CARRY; \
					if (a == X) f |= FLAG_ZERO; \
					else if (a > X) f |= FLAG_HALF_CARRY; \
					else f |= FLAG_CARRY; \
					lastInstructionTicks = 1;

#define CP_XYm(X, Y)	{ \
							byte value = mmu->read((X << 8) | Y); \
							f &= RESET_SUB & RESET_ZERO & RESET_CARRY & RESET_HALF_CARRY; \
							if (a == value) f |= FLAG_ZERO; \
							else if (a > value) f |= FLAG_HALF_CARRY; \
							else f |= FLAG_CARRY; \
							lastInstructionTicks = 1; \
						}

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
			pc += mmu->read(pc);
			pc++;
			lastInstructionTicks = 3;
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
			{
				byte addr = mmu->read(pc);
				pc++;
				if ((f & FLAG_ZERO) == 0) {
					pc += addr;
				}
				lastInstructionTicks = 3;
			}
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
			if (((a & 0xFF) > 9) || (f & FLAG_HALF_CARRY)) {
				a += 6;
				f |= FLAG_HALF_CARRY;
			}
			else {
				f &= RESET_HALF_CARRY;
			}

			if ((a > 0x9F) || (f & FLAG_CARRY)) {
				a += 0x60;
				f |= FLAG_CARRY;
			}
			else {
				f &= RESET_CARRY;
			}

			if (a == 0) {
				f |= FLAG_ZERO;
			}
			lastInstructionTicks = 1;
			break;

			case JR_Z_n:
			{
				byte value = mmu->read(pc);
				pc++;
				if (f & FLAG_ZERO) {
					pc += value;
				}
				lastInstructionTicks = 3;
			}
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
			a ^= 0xFF;
			f |= FLAG_SUB | FLAG_HALF_CARRY;
			lastInstructionTicks = 1;
			break;

		/******************************************************/
		/************************ 0x30 ************************/
		/******************************************************/

		case JR_NC_n:
			{
				byte addr = mmu->read(pc);
				pc++;
				if ((f & FLAG_CARRY) == 0) {
					pc += addr;
				}
				lastInstructionTicks = 3;
			}
			break;

		case LD_SP_nn:
			LD_X_NN(sp);
			break;

		case LD_HLm_D_A:
			LD_XYm_D_Z(h, l, a);
			break;

		case INC_SP:
			INC_X(sp);
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
			f &= RESET_SUB & RESET_HALF_CARRY;
			f |= FLAG_CARRY;
			lastInstructionTicks = 1;
			break;

		case JR_C_n:
			{
				byte value = mmu->read(pc);
				pc++;
				if (f & FLAG_CARRY) {
					pc += value;
				}
				lastInstructionTicks = 3;
			}
			break;

		case ADD_HL_SP:
			ADD_XY_Z(h, l, sp);
			break;

		case LD_A_HLm_D:
			LD_X_YZm_D(a, h, l);
			break;

		case DEC_SP:
			DEC_X(sp);
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
			f &= RESET_HALF_CARRY & RESET_SUB;
			f ^= FLAG_CARRY; 
			lastInstructionTicks = 1;
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
		/************************ 0x90 ************************/
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

		default:
			std::cerr << "OP CODE NOT IMPLEMENTED !" << std::endl;

	}

}
