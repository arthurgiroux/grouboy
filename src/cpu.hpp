#ifndef CPU_H
#define CPU_H

#include "instructions.hpp"
#include "mmu.hpp"
#include "types.hpp"
#include "utils.hpp"

/**
 * Implementation of the CPU for the classic GameBoy.
 *
 * The GameBoy CPU is based on a Z80 with some extra instructions and
 * some other instructions are removed.
 * It has a 7 internal registers, one flag register a program counter and a stack pointer.
 * It has a standard instruction set + an extended instruction set available through a special opcode.
 */
class CPU
{
  public:
	explicit CPU(MMU& mmu);

	~CPU();

	/**
	 * Fetch the next instruction from the memory, decode it and execute it.
	 *
	 * @throws UnhandledInstructionException if an instruction is not handled by the CPU
	 * @throws UnhandledExtendedInstructionException if an extended instruction is not handled by the CPU
	 *
	 * @return the number of ticks taken by the instructions to be executed
	 */
	int fetchDecodeAndExecute();

	/**
	 * Get the current value of the 8 bits register A.
	 * @return the value of the register
	 */
	byte getRegisterA() const
	{
		return a;
	}

	/**
	 * Get the current value of the 8 bits register B.
	 * @return the value of the register
	 */
	byte getRegisterB() const
	{
		return b;
	}

	/**
	 * Get the current value of the 8 bits register C.
	 * @return the value of the register
	 */
	byte getRegisterC() const
	{
		return c;
	}

	/**
	 * Get the current value of the 8 bits register D.
	 * @return the value of the register
	 */
	byte getRegisterD() const
	{
		return d;
	}

	/**
	 * Get the current value of the 8 bits register E.
	 * @return the value of the register
	 */
	byte getRegisterE() const
	{
		return e;
	}

	/**
	 * Get the current value of the 8 bits register H.
	 * @return the value of the register
	 */
	byte getRegisterH() const
	{
		return h;
	}

	/**
	 * Get the current value of the 8 bits register L.
	 * @return the value of the register
	 */
	byte getRegisterL() const
	{
		return l;
	}

	/**
	 * Retrieve the current address of the program counter.
	 * The program counter is the address in memory of the next instruction to fetch.
	 *
	 * @return the 16 bits value of the program counter.
	 */
	uint16_t getProgramCounter() const
	{
		return pc;
	}

	/**
	 * Retrieve the current address of the stack pointer.
	 * The stack pointer is the address in memory of the stack,
	 * a reserved area used for internal operations.
	 *
	 * @return the 16 bits value of the stack pointer.
	 */

	uint16_t getStackPointer() const
	{
		return sp;
	}

	/**
	 * Exception raised when an instruction parsed by the CPU is not handled.
	 */
	class UnhandledInstructionException : public std::exception
	{
	  public:
		explicit UnhandledInstructionException(byte opCode) : opCode(opCode)
		{
			str = utils::string_format("Unknown instruction with opcode=%x", opCode);
		}

		const char* what() const noexcept override
		{
			return str.c_str();
		}

	  protected:
		byte opCode = 0;
		std::string str;
	};

	/**
	 * Exception raised when an extended instruction parsed by the CPU is not handled.
	 */
	class UnhandledExtendedInstructionException : public UnhandledInstructionException
	{
	  public:
		explicit UnhandledExtendedInstructionException(byte opCode) : UnhandledInstructionException(opCode)
		{
			str = utils::string_format("Unknown extended instruction with opcode=%x", opCode);
		}

		const char* what() const noexcept override
		{
			return str.c_str();
		}
	};

#ifndef UNIT_TESTING
  private:
#endif

	/**
	 * Possible values for the CPU flag.
	 */
	enum CpuFlags : byte
	{
		ZERO =
		    0x80, /// Set when the result of a math operation is zero or two values match when using the CP instruction.
		SUBSTRACTION = 0x40, /// Set if a subtraction was performed in the last math instruction.
		HALF_CARRY = 0x20,   /// Set if a carry occurred from the lower nibble in the last math operation.
		CARRY = 0x10 /// Set if a carry occurred from the last math operation or if register A is the smaller value when
		             /// executing the CP instruction.
	};

	/**
	 * Execute an instruction that is part of the standard instruction set.
	 *
	 * @throws UnhandledInstructionException if the given instruction is not handled by the CPU
	 * @param opCode the opcode of the instruction to execute
	 */
	void executeInstruction(const byte& opCode);

	/**
	 * Execute an instruction that is part of the extended instruction set.
	 *
	 * @throws UnhandledExtendedInstructionException if the given instruction is not handled by the CPU
	 * @param opCode the opcode of the instruction to execute
	 */
	void executeExtendedInstruction(const byte& opCode);

	// internal registers
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

	/**
	 * Perform no operation.
	 *
	 * @opcodes:
	 *     0x00
	 * @flags_affected: N/A
	 * @number_of_ticks: 1
	 */
	void NoOperation();

	/**
	 * Load the 16 bits pointed by the program counter into registers.
	 *
	 * @param msbRegister the register that will be set to the 8 bit MSB of the value.
	 * @param lsbRegister the register that will be set to the 8 bit LSB of the value.
	 * @opcodes:
	 *     0x01 0x11 0x21 0x31
	 * @flags_affected: N/A
	 * @number_of_ticks: 3
	 */
	void load16BitsValueInRegisters(byte& msbRegister, byte& lsbRegister);

	// Load the
	void LD_XY_Z_N(byte& X, byte& Y, uint16_t Z);

	// Load the word pointed by pc into X
	void LD_X_NN(uint16_t& X);

	// Load the byte pointed by the address pointed by pc into X
	void LD_X_NNm(byte& X);

	/**
	 * Load the 16 bits value of the given register to the immediate address.
	 *
	 * @param reg   the register from which to load the value
	 * @opcodes:
	 *     0x08
	 * @flags_affected: N/A
	 * @number_of_ticks: 5
	 */
	void load16BitsRegisterAtImmediateAddress(uint16_t reg);

	/**
	 * Load the 8 bits value of the given register to the immediate address.
	 *
	 * @param reg   the register from which to load the value
	 * @opcodes:
	 *     0xEA
	 * @flags_affected: N/A
	 * @number_of_ticks: 4
	 */
	void load8BitsRegisterAtImmediateAddress(byte reg);

	/**
	 * Load the immediate value in the given register.
	 *
	 * @param reg   the register in which to load the value
	 * @opcodes:
	 *     0x06 0x16 0x26 0x0E 0x1E 0x2E 0x3E
	 * @flags_affected: N/A
	 * @number_of_ticks: 2
	 */
	void loadImmediateValueInRegister(byte& reg);

	/**
	 * Load the given value to the memory pointed by the given address.
	 *
	 * @param addrMsb   the MSB part of the address
	 * @param addrLsb   the LSB part of the address
	 * @param value     the value to load in memory
	 * @opcodes:
	 *     0x02 0x12 0x70 0x71 0x72 0x73 0x74 0x75 0x76
	 * @flags_affected: N/A
	 * @number_of_ticks: 2
	 */
	void loadValueToMemoryAtAddr(byte addrMsb, byte addrLsb, byte value);

	// Load the byte pointed by pc at the address pointed by the word XY
	void LD_XYm_N(byte X, byte Y);

	/**
	 * Load the given value to the memory pointed by the given address and
	 * increment the address.
	 *
	 * @param addrMsb   the MSB part of the address
	 * @param addrLsb   the LSB part of the address
	 * @param value     the value to load in memory
	 * @opcodes:
	 *     0x22
	 * @flags_affected: N/A
	 * @number_of_ticks: 2
	 */
	void loadValueToMemoryAndIncreaseAddr(byte& addrMsb, byte& addrLsb, byte value);

	// Load the byte pointed by the word YZ into X and
	// increment the address YZ
	void LD_X_YZm_I(byte& X, byte& Y, byte& Z);

	// Load the byte pointed by the word YZ into X and
	// decrement the address YZ
	void LD_X_YZm_D(byte& X, byte& Y, byte& Z);

    /**
     * Load the given value to the memory pointed by the given address and
     * decrement the address.
     *
     * @param addrMsb   the MSB part of the address
     * @param addrLsb   the LSB part of the address
     * @param value     the value to load in memory
     * @opcodes:
     *     0x32
     * @flags_affected: N/A
     * @number_of_ticks: 2
     */
	void loadValueToMemoryAndDecreaseAddr(byte& addrMsb, byte& addrLsb, byte value);

	// Load the byte pointed by YZ into X
	/**
	 * Load the value from memory at the given address into the given register.
	 *
	 * @param reg       The 8 bits register where the value will be loaded
	 * @param addrMsb   The msb of the address
	 * @param addrLsb   The lsb of the address
	 * @opcodes:
	 *     0x46 0x56 0x66 0x0A 0x1A 0x4E 0x5E 0x6E 0x7E
	 * @flags_affected: N/A
	 * @number_of_ticks: 2
	 */
	void loadValueFromMemoryInto8BitsRegister(byte& reg, byte addrMsb, byte addrLsb);

	// Load the byte Y into X
	void LD_X_Y(byte& X, byte Y);

	/**
	 * Increment the 16 bits value of the register made of two 8 bits registers.
	 *
	 * @param msbRegister   the MSB register
	 * @param lsbRegister   the LSB register
	 * @opcodes:
	 *     0x03 0x13 0x23
	 * @flags_affected: N/A
	 * @number_of_ticks: 2
	 */
	void incrementRegistersValue(byte& msbRegister, byte& lsbRegister);

	/**
	 * Increment the 16 bits value of the register.
	 *
	 * @param reg   the register to increment
	 * @opcodes:
	 *     0x33
	 * @flags_affected: N/A
	 * @number_of_ticks: 2
	 */
	void incrementRegisterValue(uint16_t& reg);

	/**
	 * Increment the value in memory pointed by the given address.
	 *
	 * @param addrMsb   the MSB part of the address
	 * @param addrLsb   the LSB part of the address
	 * @opcodes:
	 *     0x34
	 * @flags_affected: Zero, Half-carry, Substraction
	 * @number_of_ticks: 3
	 */
	void incrementValueInMemoryAtAddr(byte addrMsb, byte addrLsb);

	/**
	 * Increment the 8 bits value of the register.
	 *
	 * @param reg   the register
	 * @opcodes:
	 *     0x04 0x14 0x24 0x0C 0x1C 0x2C 0x3C
	 * @flags_affected: Zero, Half-carry, Substraction
	 * @number_of_ticks: 1
	 */
	void incrementRegisterValue(byte& reg);

	/**
	 * Decrement the value in memory pointed by the given address.
	 *
	 * @param addrMsb   the MSB part of the address
	 * @param addrLsb   the LSB part of the address
	 * @opcodes:
	 *     0x35
	 * @flags_affected: Zero, Half-carry, Substraction
	 * @number_of_ticks: 3
	 */
	void decrementValueInMemoryAtAddr(byte addrMsb, byte addrLsb);

	/**
	 * Decrement the 8 bits value of the register.
	 *
	 * @param reg   the register
	 * @opcodes:
	 *     0x05 0x15 0x25 0x0D 0x1D 0x2D 0x3D
	 * @flags_affected: Zero, Half-carry, Substraction
	 * @number_of_ticks: 1
	 */
	void decrementRegisterValue(byte& reg);

	/**
	 * Decrement the 16 bits value of the register made of two 8 bits registers.
	 *
	 * @param msbRegister   the MSB register
	 * @param lsbRegister   the LSB register
	 * @opcodes:
	 *     0x0B 0x1B 0x2B
	 * @flags_affected: N/A
	 * @number_of_ticks: 2
	 */
	void decrementRegistersValue(byte& msgRegister, byte& lsbRegister);

	/**
	 * Decrement the 16 bits value of the register.
	 *
	 * @param reg   the register to increment
	 * @opcodes:
	 *     0x3B
	 * @flags_affected: N/A
	 * @number_of_ticks: 2
	 */
	void decrementRegisterValue(uint16_t& reg);

	/**
	 * Rotate circularly the value inside the given register to the
	 * left by 1 bit.
	 * the 7th bit is copied into the carry flag.
	 *
	 * Here's some example of rotating the value 10000000
	 * several times.
	 *
	 * |Carry| | Value |
	 *  0      10000000
	 *  1      00000001
	 *  0      00000010
	 *
	 * @param reg   the register to rotate to the left by 1 bit.
	 * @opcodes:
	 *     0x07
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 1
	 */
	void rotateRegisterLeftCircular(byte& reg);

	/**
	 * Rotate left circular for the extended instruction set.
	 * See {@link #rotateRegisterLeftCircular(byte&)} for details.
	 *
	 * @param reg   the register to rotate to the left by 1 bit.
	 * @opcodes:
	 *     0x00 0x01 0x02 0x03 0x04 0x05 0x07 0x08
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void rotateRegisterLeftCircularExtended(byte& reg);

	/**
	 * Rotate left circularly for a value in memory.
	 * See {@link #rotateRegisterLeftCircular(byte&)} for details.
	 *
	 * @param addrMsb   the msb part of the address where to perform the operation
	 * @param addrLsb   the Lsb part of the address where to perform the operation
	 * @opcodes:
	 *     0x06
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 4
	 */
	void rotateValueInMemoryLeftCircular(byte addrMsb, byte addrLsb);

	/**
	 * Rotate circularly the value inside the given register to the
	 * right by 1 bit.
	 * the 0th bit is copied into the carry flag.
	 *
	 * Here's some example of rotating the value 00000001
	 * several times.
	 *
	 * |Carry| | Value |
	 *  0      00000001
	 *  1      10000000
	 *  0      01000000
	 *
	 * @param reg   the register to rotate to the right by 1 bit.
	 * @opcodes:
	 *     0x0F
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 1
	 */
	void rotateRegisterRightCircular(byte& reg);

	/**
	 * Rotate right circularly for the extended instruction set.
	 * See {@link #rotateRegisterRightCircular(byte&)} for details.
	 *
	 * @param reg   the register to rotate to the right by 1 bit.
	 * @opcodes:
	 *     0x08 0x09 x0A 0x0B 0x0C 0x0D 0x0F
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void rotateRegisterRightCircularExtended(byte& reg);

	/**
	 * Rotate right circularly for a value in memory.
	 * See {@link #rotateRegisterRightCircular(byte&)} for details.
	 *
	 * @param addrMsb   the msb part of the address where to perform the operation
	 * @param addrLsb   the Lsb part of the address where to perform the operation
	 * @opcodes:
	 *     0x0E
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 4
	 */
	void rotateValueInMemoryRightCircular(byte addrMsb, byte addrLsb);

	/**
	 * Rotate the value inside the given register to the
	 * left by 1 bit.
	 *
	 * The carry is used as a buffer for the rotation.
	 *
	 * Here's some example of rotating the value 10000000
	 * several times.
	 *
	 * |Carry| | Value |
	 *  0      10000000
	 *  1      00000000
	 *  0      00000001
	 *
	 * @param reg   the register to rotate to the left by 1 bit.
	 * @opcodes:
	 *     0x17
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void rotateRegisterLeftExtended(byte& reg);

	/**
	 * Rotate left a value in memory.
	 * See {@link #rotateRegisterLeftExtended(byte&)} for details.
	 *
	 * @param addrMsb   the msb part of the address where to perform the operation
	 * @param addrLsb   the Lsb part of the address where to perform the operation
	 * @opcodes:
	 *     0x16
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 4
	 */
	void rotateValueInMemoryLeft(byte addrMsb, byte addrLsb);

	/**
	 * Rotate the value inside the given register to the
	 * right by 1 bit.
	 *
	 * The carry is used as a buffer for the rotation.
	 *
	 * Here's some example of rotating the value 00000001
	 * several times.
	 *
	 * |Carry| | Value |
	 *  0      00000001
	 *  1      00000000
	 *  0      10000000
	 *
	 * @param reg   the register to rotate to the right by 1 bit.
	 * @opcodes:
	 *     0x1F
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void rotateRegisterRightExtended(byte& reg);

	/**
	 * Rotate right a value in memory.
	 * See {@link #rotateRegisterRightExtended(byte&)} for details.
	 *
	 * @param addrMsb   the msb part of the address where to perform the operation
	 * @param addrLsb   the Lsb part of the address where to perform the operation
	 * @opcodes:
	 *     0x1E
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 4
	 */
	void rotateValueInMemoryRight(byte addrMsb, byte addrLsb);

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

	/**
	 * Arithmetic operation of adding the 16 bits value made from two
	 * 8 bits register (YZ) to another two 8 bits register (WX)
	 *
	 * @param resultRegMsb    the msb of the register where the value will be added
	 * @param resultRegLsb    the lsb of the register where the value will be added
	 * @param valueRegMsb     the msb of the register containing the value to add
	 * @param valueRegLsb     the lsb of the register containing the value to add
	 * @opcodes:
	 *     0x09 0x19 0x29 0x39
	 * @flags_affected: Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void addTwo8BitsRegistersToTwo8BitsRegisters(byte& resultRegMsb, byte& resultRegLsb, byte valueRegMsb,
	                                             byte valueRegLsb);

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

	// Compare the value of the register "a" and the value X
	// If a == X then flag zero is set
	// If a > X then flag half carry is set
	// Otherwise flag carry is set
	void CP_X(byte X);

	// Compare the value of the register "a" and the value pointed by XY
	// If a == XY then flag zero is set
	// If a > XY then flag half carry is set
	// Otherwise flag carry is set
	void CP_XYm(byte X, byte Y);

	// Pop the value pointed by sp in X and sp+1 in X
	void POP_XY(byte X, byte Y);

	// Pop the value pointed by sp in X and sp+1 in X
	void RET_X(bool cond);

	// if the condition is satisfied, push the current address to the stack
	// and jump to the label
	void CALL_X_NN(bool cond);

	/**
	 * If the condition is true,
	 * Jump to the address pointed by the program counter +
	 * a relative offset in the immediate value.
	 *
	 * @param condition     The condition that needs to be true for the jump to happen
	 *
	 * @opcodes:
	 *     0x20 0x28 0x30 0x38
	 * @flags_affected: N/A
	 * @number_of_ticks: 3 if condition true, 2 otherwise
	 */
	void jumpRelativeConditional(bool condition);

	/**
	 * Jump to the address pointed by the program counter +
	 * a relative offset in the immediate value.

	 * @opcodes:
	 *     0x18
	 * @flags_affected: N/A
	 * @number_of_ticks: 3
	 */
	void jumpRelative();

	/**
	 * If the condition is true,
	 * Jump to the address pointed by the program counter
	 *
	 * @param condition     The condition that needs to be true for the jump to happen
	 *
	 * @opcodes:
	 *     0xC2 0xCA 0xD2 0xDA
	 * @flags_affected: N/A
	 * @number_of_ticks: 4 if condition true, 3 otherwise
	 */
	void jumpConditional(bool condition);

	/**
	* Jump to the address pointed by the program counter
    *
	* @opcodes:
	*     0xC3
	* @flags_affected: N/A
	* @number_of_ticks: 4
	*/
	void jump();

	/**
	 * Jump to the address pointed by the program counter
	 *
	 * @param addrMsb   the msb part of the address
	 * @param addrLsb   the Lsb part of the address
	 *
	 * @opcodes:
	 *     0xE9
	 * @flags_affected: N/A
	 * @number_of_ticks: 1
	 */
	void jumpToAddrIn16BitsRegister(byte addrMsb, byte addrLsb);

	/**
	 * Adjusts the sum of two packed BCD values to create a packed BCD result.
	 * The a register is the implied source and destination operand.
	 *
	 * @opcodes:
	 *     0x27
	 * @flags_affected: Zero, Carry, Half-carry
	 * @number_of_ticks: 1
	 */
	void decimalAdjustAccumulator();

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
	// void LDH_Xm_Y(byte X, byte Y);

	// Load the value pointed by 0xFF + pc in X
	// void LDH_X_Nm(byte& X);

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

	// Retrieve the current flag value
	byte getFlag() const;

	void setHalfCarryFlag(bool state);

	void setCarryFlag(bool state);

	/**
	 * Reset all CPU flags
	 */
	void resetFlags();
};

#endif
