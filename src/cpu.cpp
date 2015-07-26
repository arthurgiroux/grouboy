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
							lastInstructionTicks = 1;

#define LD_NNm_X(X) 	{ \
							uint16_t addr = ((mmu->read(pc) << 8) & mmu->read(pc+1)); \
							X = mmu->read(addr); \
							pc += 2; \
							lastInstructionTicks = 3; \
						}

#define INC_XY(X, Y) 	Y++; \
						if (Y == 0) X++; \
						lastInstructionTicks = 1;

#define INC_X(X) 		X++; \
						if (X == 0) f |= FLAG_HALF_CARRY; /* TODO: ZERO */\
						f &= RESET_SUB; \
						lastInstructionTicks = 1;

#define DEC_X(X) 		X--; \
						if (X == 255) f |= FLAG_HALF_CARRY; \
						else f &= FLAG_HALF_CARRY; \
						if (X == 0) f |= FLAG_ZERO; \
						else f &= FLAG_ZERO; \
						f |= FLAG_SUB; \
						lastInstructionTicks = 1;


#define RLC_X(X)		{ \
							byte curFlag = f; \
							f = ((X & 0x80) >> 3) & RESET_SUB & RESET_ZERO & RESET_HALF_CARRY; /* Set the carry flag to the highest bit of X */ \
	    					X = ((X << 1) | (curFlag >> 7)); /* Rotate the accumulator and set the first bit to the original carry flag */ \
	    					lastInstructionTicks = 1; \
	    				}

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
	    					lastInstructionTicks = 3; \
	    				}


void CPU::process(const byte& opCode) {
	switch (opCode) {
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

		default:
			std::cerr << "OP CODE NOT IMPLEMENTED !" << std::endl;

	}

}
