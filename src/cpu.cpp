#include <iostream>
#include "cpu.hpp"
#include "instructions.hpp"
#include "mmu.hpp"

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

#define LD_X_N(X) 		X = mmu->read(pc); \
						pc++; \
						lastInstructionTicks = 2;

#define LD_XYm_Z(X, Y, Z)	mmu->write((X << 8) | Y, Z); \
							lastInstructionTicks = 2;

#define LD_XYm_I_Z(X, Y, Z)		mmu->write((X << 8) | Y, Z); \
								Y++; \
								if (Y == 0)	X++; \
								lastInstructionTicks = 2;
							
#define LD_XYm_D_Z(X, Y, Z)		mmu->write((X << 8) | Y, Z); \
								if (Y == 0)	X--; \
								else X--; \
								lastInstructionTicks = 2;

#define LD_X_YZm(X, Y, Z) 	X = mmu->read((Y << 8) | Z); \
							lastInstructionTicks = 1;

#define LD_NNm_X(X) 	{ \
							uint16_t addr = ((mmu->read(pc) << 8) & mmu->read(pc+1)); \
							X = mmu->read(addr); \
							pc += 2; \
							lastInstructionTicks = 5; \
						}

#define INC_XY(X, Y) 	Y++; \
						if (Y == 0) X++; \
						lastInstructionTicks = 2;

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


void CPU::process(const byte& opCode) {
	switch (opCode) {
		// 0x00
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

		// 0x10

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

		// 0x20

		case JR_NZ_n:
			{
				uint16_t addr = mmu->read(pc);
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


		default:
			std::cerr << "OP CODE NOT IMPLEMENTED !" << std::endl;

	}

}
