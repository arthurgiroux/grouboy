#ifndef CPU_H
#define CPU_H

#include "types.hpp"
#include "mmu.hpp"
#include "instructions.hpp"

class CPU {
public:
	explicit CPU(MMU& mmu);
	~CPU();
	int exec();

	byte getRegisterA() const {
	    return a;
	}

	byte getRegisterB() const {
	    return b;
	}

    byte getRegisterC() const {
	    return c;
	}

    byte getRegisterD() const {
	    return d;
	}

    byte getRegisterE() const {
	    return e;
	}

    byte getRegisterF() const {
	    return f;
	}

    byte getRegisterH() const {
	    return h;
	}

    byte getRegisterL() const {
	    return l;
	}

    uint16_t getProgramCounter() const {
	    return pc;
	}

    uint16_t getStackPointer() const {
	    return sp;
	}

#ifndef UNIT_TESTING
private:
#endif

    enum CpuFlags : byte {
        ZERO = 0x80, // Set when the result of a math operation is zero or two values match when using the CP instruction.
        SUBSTRACTION = 0x40, // Set if a subtraction was performed in the last math instruction.
        HALF_CARRY = 0x20, // Set if a carry occurred from the lower nibble in the last math operation.
        CARRY = 0x10 // Set if a carry occurred from the last math operation or if register A is the smaller value when executing the CP instruction.
    };

	// Process the given opcode as a base opcode
	void process(const byte& opCode);

	// Process the given opcode as an extended opcode
	void processExtended(const byte& opCode);

	// registers
	byte a{};
	byte b{};
	byte c{};
	byte d{};
	byte e{};
	byte h{};
	byte l{};
	// flag register
	byte f{};

	// current CPU tick
	int tick;

	// How many ticks the latest instruction took
	int lastInstructionTicks;

	// program counter
	uint16_t pc;
	// stack pointer
	uint16_t sp;

	// Memory management unit
	MMU& mmu;

	// Whether of not the CPU is halted
	bool halted;

	bool interrupts;

	// How many ticks left before enabling the interrupts
	byte ticksBeforeEnablingInterrupts;

	// How many ticks left before disabling the interrupts
	byte ticksBeforeDisablingInterrupts;

	// Load the byte pointed by pc into X and the value pointed by pc+1 into Y
	void LD_XY_NN(byte& X, byte& Y);

	// Load the 
	void LD_XY_Z_N(byte& X, byte& Y, uint16_t Z);

	// Load the word pointed by pc into X
	void LD_X_NN(uint16_t& X);

	// Load the byte pointed by the address pointed by pc into X
	void LD_X_NNm(byte& X);

	// Load X in the address pointed by pc
	void LD_NNm_X(byte X);

	// Load the byte pointed by pc into X
	void LD_X_N(byte& X);

	// Load the byte Z at the address pointed by the word XY
	void LD_XYm_Z(byte X, byte Y, byte Z);

	// Load the byte pointed by pc at the address pointed by the word XY
	void LD_XYm_N(byte X, byte Y);

	// Load the byte Z at the address pointed by the word XY and
	// increment the address XY
	void LD_XYm_I_Z(byte& X, byte& Y, byte Z);

	// Load the byte pointed by the word YZ into X and
	// increment the address YZ
	void LD_X_YZm_I(byte& X, byte& Y, byte& Z);

	// Load the byte pointed by the word YZ into X and
	// decrement the address YZ
	void LD_X_YZm_D(byte& X, byte& Y, byte& Z);

	// Load the byte Z at the address pointed by the word XY and
	// decrement the address XY
	void LD_XYm_D_Z(byte& X, byte& Y, byte& Z);

	// Load the byte pointed by YZ into X
	void LD_X_YZm(byte& X, byte Y, byte Z);

	// Load the word pointed by the address contained in pc into X
	void LD_NNm_X(uint16_t& X);

	// Load the byte Y into X
	void LD_X_Y(byte& X, byte Y);

	// Increment the value of XY
	void INC_XY(byte& X, byte& Y);
	void INC_XY(uint16_t& XY);

	// Increment the value pointed by XY
	void INC_XYm(byte X, byte Y);

	// Decrement the value pointed by XY
	void DEC_XYm(byte X, byte Y);

	// Increment the value of X
	void INC_X(byte& X);

	// Decrement the value of X
	void DEC_X(byte& X);

	// Decrement the value of XY
	void DEC_XY(byte& X, byte& Y);
	void DEC_XY(uint16_t& XY);

	// Rotate the value of X to the left using the carry
	void RLC_X(byte& X);

	// Rotate the value pointed by XY to the left using the carry
	void RLC_XYm(byte X, byte Y);

	// Rotate the value of X to the right using the carry
	void RRC_X(byte& X);

	// Rotate the value pointed by XY to the right using the carry
	void RRC_XYm(byte X, byte Y);

	// Rotate the value of X to the left
	void RL_X(byte& X);

	// Rotate the value pointed by XY to the left
	void RL_XYm(byte X, byte Y);

	// Rotate the value of X to the right
	void RR_X(byte& X);

	// Rotate the value pointed by XY to the right
	void RR_XYm(byte X, byte Y);

	// Shift left arithmetic of X
	void SLA_X(byte& X);

	// Shift left arithmetic of the value pointed by XY
	void SLA_XYm(byte X, byte Y);

	// Shift right arithmetic of X
	void SRA_X(byte X);

	// Shift right arithmetic of the value pointed by XY
	void SRA_XYm(byte X, byte Y);

	// Shift right logical of X
	void SRL_X(byte& X);

	// Shift right logical of the value pointed by XY
	void SRL_XYm(byte X, byte Y);

	// Swap the high and low nibble of X
	void SWAP_X(byte& X);

	// Swap the high and low nibble of the value pointed by XY
	void SWAP_XYm(byte X, byte Y);

	// Test if the bit at position X is set in Y
	void BIT_X_Y(byte X, byte Y);

	// Test if the bit at position X is set in the value pointed by YZ
	void BIT_X_YZm(byte X, byte Y, byte Z);

	// Reset the value of the bit at position X in Y
	void RES_X_Y(byte X, byte& Y);

	// Reset the value of the bit at position X in the value pointed by YZ
	void RES_X_YZm(byte X, byte Y, byte Z);

	// Set the value of the bit at position X in Y
	void SET_X_Y(byte X, byte& Y);

	// Set the value of the bit at position X in the value pointed by YZ
	void SET_X_YZm(byte X, byte Y, byte Z);

	// Add the value of YZ to the value of WX
	void ADD_WX_YZ(byte& W, byte& X, byte Y, byte Z);

	// Add the value of Z to the value of XY
	void ADD_XY_Z(byte& X, byte& Y, byte Z);

	// Add the value of Y to the value of X
	void ADD_X_Y(byte& X, byte Y);

	// Add the value pointed by pc to the value of X
	void ADD_X_N(byte& X);

	// Add the value pointed by YZ to the value of X
	void ADD_X_YZm(byte& X, byte Y, byte Z);

	// Add the value of Y and the value of the carry to the value of X
	void ADC_X_Y(byte& X, byte Y);

	// Add the value pointed by pc and the value of the carry to the value of X
	void ADC_X_N(byte X);

	// Add the value pointed by YZ and the value of the carry to the value of X
	void ADC_X_YZm(byte& X, byte Y, byte Z);

	// Substract the value of Y from the value of X
	void SUB_X_Y(byte& X, byte Y);

	// Substract the value pointed by pc from the value of X
	void SUB_X_N(byte& X);

	// Substract the value of Y and the value of the carry from the value of X
	void SBC_X_Y(byte& X, byte Y);

	// Substract the value pointed by pc and the value of the carry from the value of X
	void SBC_X_N(byte& X);

	// Substract the value pointed by YZ from the value of X
	void SUB_X_YZm(byte& X, byte Y, byte Z);

	// Substract the value pointed by YZ and the value of the carry from the value of X
	void SBC_X_YZm(byte& X, byte Y, byte Z);

	// Perform the logical AND between the "a" register and X
	void AND_X(byte X);

	// Perform the logical AND between the "a" register and value pointed by XY
	void AND_XYm(byte X, byte Y);

	// Perform the logical XOR between the "a" register and X
	void XOR_X(byte X);

	// Perform the logical XOR between the "a" register and value pointed by XY
	void XOR_XYm(byte X, byte Y);

	// Perform the logical OR between the "a" register and X
	void OR_X(byte X);

	// Perform the logical OR between the "a" register and value pointed by XY
	void OR_XYm(byte X, byte Y);

	// Perform the logical OR between the "a" register and value pointed by pc
	void OR_N();

	// Perform the logical AND between the "a" register and value pointed by pc
	void AND_N();

	//Compare the value of the register "a" and the value X
	// If a == X then flag zero is set
	// If a > X then flag half carry is set
	// Otherwise flag carry is set
	void CP_X(byte X);

	//Compare the value of the register "a" and the value pointed by XY
	// If a == XY then flag zero is set
	// If a > XY then flag half carry is set
	// Otherwise flag carry is set
	void CP_XYm(byte X, byte Y);

	// Pop the value pointed by sp in X and sp+1 in X
	void POP_XY(byte X, byte Y);

	// Pop the value pointed by sp in X and sp+1 in X
	void RET_X(bool cond);

	// If the condition is satisfied, jump to the address stored in memory
	void JP_X_NN(bool cond);

	// jump to the address stored at memory pointed by XY
	void JP_XYm(byte X, byte Y);

	// if the condition is satisfied, push the current address to the stack
	// and jump to the label
	void CALL_X_NN(bool cond);
	
	// If the condition is satisfied, jump to the relative address stored in memory
	void JR_COND_N(bool condition);

	// BCD corrects the value of the "a" register
	void DAA_();

	// Inverts all bits of a
	void CPL_();

	// Set the carry flag
	void SCF_();

	// Inverts the carry flag
	void CCF_();

	// Push the value XY on the stack pointer
	void PUSH_XY(byte X, byte Y);

	// Save the value of the program counter on the stack pointer and
	// set the value of pc to X
	void RST_X(byte X);

	// Load the value of X in the value pointed by 0xFF00 + pc
	void LDH_Nm_X(byte X);

	// Load the value pointed by 0xFF00 + pc in X
	void LDH_X_Nm(byte& X);

	void LD_Xm_Y(byte X, byte Y);

	void LD_X_Ym(byte& X, byte Y);

	void ADD_SP_X(sbyte X);

	// Load the value of X in the value pointed by 0xFF00 + pc
	//void LDH_Xm_Y(byte X, byte Y);

	// Load the value pointed by 0xFF + pc in X
	//void LDH_X_Nm(byte& X);

	// Set/unset the zero flag depending on value
	void changeZeroValueFlag(byte value);

	// Set the given flag if the condition is true, reset it otherwise
	void setFlagIfTrue(bool condition, CpuFlags flag);

	// Set the given flag
	void setFlag(CpuFlags flag);

	// Unset the given flag
	void unsetFlag(CpuFlags flag);

    // Set the given flag
    bool isFlagSet(CpuFlags flag) const;

    void setHalfCarryFlag(bool state);

    void setCarryFlag(bool state);

};

#endif
