#ifndef CPU_H
#define CPU_H

#include "instructions.hpp"
#include "interrupt_handler.hpp"
#include "mmu.hpp"
#include "types.hpp"
#include "utils.hpp"
#include <map>

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
	 * Possible values for the CPU flag.
	 */
	enum CpuFlags : byte
	{
		/**
		 * Set when the result of a math operation is zero or two values match when using the CP instruction.
		 */
		ZERO = 0x80,
		/**
		 * Set if a subtraction was performed in the last math instruction.
		 */
		SUBSTRACTION = 0x40,
		/**
		 * Set if a carry occurred from the lower nibble in the last math operation.
		 */
		HALF_CARRY = 0x20,
		/**
		 * Set if a carry occurred from the last math operation or if register A is the smaller value when executing the
		 * CP instruction.
		 */
		CARRY = 0x10
	};

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
	 * Set the value of register A.
	 *
	 * @param value     The value to set.
	 */
	void setRegisterA(byte value)
	{
		a = value;
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
	 * Set the value of register B.
	 *
	 * @param value     The value to set.
	 */
	void setRegisterB(byte value)
	{
		b = value;
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
	 * Set the value of register C.
	 *
	 * @param value     The value to set.
	 */
	void setRegisterC(byte value)
	{
		c = value;
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
	 * Set the value of register D.
	 *
	 * @param value     The value to set.
	 */
	void setRegisterD(byte value)
	{
		d = value;
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
	 * Set the value of register E.
	 *
	 * @param value     The value to set.
	 */
	void setRegisterE(byte value)
	{
		e = value;
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
	 * Set the value of register H.
	 *
	 * @param value     The value to set.
	 */
	void setRegisterH(byte value)
	{
		h = value;
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
	 * Set the value of register L.
	 *
	 * @param value     The value to set.
	 */
	void setRegisterL(byte value)
	{
		l = value;
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
	 * Set the value of the program counter.
	 *
	 * @param value The value to set.
	 */
	void setProgramCounter(uint16_t value)
	{
		pc = value;
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
	 * Set the value of the stack pointer.
	 *
	 * @param value The value to set.
	 */
	void setStackPointer(uint16_t value)
	{
		sp = value;
	}

	/**
	 * Reset the CPU to the original state.
	 */
	void reset();

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

	/**
	 * Disable interrupts routine.
	 *
	 * @opcodes:
	 *     0xF3
	 * @flags_affected: N/A
	 * @number_of_ticks: 1
	 */
	void disableInterrupts();

#ifndef UNIT_TESTING
  private:
#endif
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

	/**
	 * Load a 16 bits register + an immediate signed offset into another 16 bits register.
	 *
	 * @param msbRegister   the register that will be set to the 8 bit MSB of the value.
	 * @param lsbRegister   the register that will be set to the 8 bit LSB of the value.
	 * @param otherReg      The register containing the value to load
	 * @opcodes:
	 *     0xF8
	 * @flags_affected: Zero, Substraction, Half-carry, Carry
	 * @number_of_ticks: 3
	 */
	void load16BitsRegisterAndImmediateOffsetIn16BitsRegister(byte& msbRegister, byte& lsbRegister, uint16_t otherReg);

	/**
	 * Load two 8 bits register into a 16 bits register.
	 *
	 * @param reg      The register where to load the value
	 * @param msbValue   the msb value to use.
	 * @param lsbValue   the lsb value to use.

	 * @opcodes:
	 *     0xF9
	 * @flags_affected: N/A
	 * @number_of_ticks: 2
	 */
	void loadTwo8BitsRegisterIn16BitsRegister(uint16_t& reg, byte msbValue, byte lsbValue);

	/**
	 * Load the 16 bits immediate value into the given register.
	 *
	 * @param reg   the register in which to load the value
	 * @opcodes:
	 *     0x31
	 * @flags_affected: N/A
	 * @number_of_ticks: 3
	 */
	void load16BitsImmediateValueIntoRegister(uint16_t& reg);

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
	 * Load the immediate 16 bits value in the given 8 bits register.
	 *
	 * @param reg   the register in which to load the value
	 * @opcodes:
	 *     0xFA
	 * @flags_affected: N/A
	 * @number_of_ticks: 4
	 */
	void loadImmediate16BitsValueIn8BitsRegister(byte& reg);

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

	/**
	 * Load the 8 bits immediate value into memory at the given address.
	 *
	 * @param addrMsb   the MSB part of the address
	 * @param addrLsb   the LSB part of the address
	 * @opcodes:
	 *     0x36
	 * @flags_affected: N/A
	 * @number_of_ticks: 3
	 */
	void load8BitsImmediateValueAtMemoryAddress(byte addrMsb, byte addrLsb);

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
	void loadValueToMemoryAndIncrementAddr(byte& addrMsb, byte& addrLsb, byte value);

	/**
	 * Load value from memory into the given register and
	 * increment the address.
	 *
	 * @param reg       the register where to load the value
	 * @param addrMsb   the MSB part of the address
	 * @param addrLsb   the LSB part of the address
	 * @opcodes:
	 *     0x2A
	 * @flags_affected: N/A
	 * @number_of_ticks: 2
	 */
	void loadValueFromMemoryAndIncrementAddr(byte& reg, byte& addrMsb, byte& addrLsb);

	/**
	 * Load value from memory into the given register and
	 * decrement the address.
	 *
	 * @param reg       the register where to load the value
	 * @param addrMsb   the MSB part of the address
	 * @param addrLsb   the LSB part of the address
	 * @opcodes:
	 *     0x3A
	 * @flags_affected: N/A
	 * @number_of_ticks: 2
	 */
	void loadValueFromMemoryAndDecreaseAddr(byte& reg, byte& addrMsb, byte& addrLsb);

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

	/**
	 * Load an 8 value into into an 8 bit register.
	 *
	 * @param reg          the register where to load the value
	 * @param value        the value to load
	 * @opcodes:
	 *     0x40 0x41 0x42 0x43 0x44 0x45 0x47 0x48 0x49 0x4A 0x4B 0x4C 0x4D 0x4F
	 *     0x50 0x51 0x52 0x53 0x54 0x55 0x57 0x58 0x59 0x5A 0x5B 0x5C 0x5D 0x5F
	 *     0x60 0x61 0x62 0x63 0x64 0x65 0x67 0x68 0x69 0x6A 0x6B 0x6C 0x6D 0x6F
	 *     0x78 0x79 0x7A 0x7B 0x7C 0x7D 0x7F
	 * @flags_affected: N/A
	 * @number_of_ticks: 1
	 */
	void load8BitsValueInRegister(byte& reg, byte value);

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
	 * @number_of_ticks: 1
	 */
	void rotateRegisterLeft(byte& reg);

	/**
	 * Rotate the value inside the given register to the
	 * left by 1 bit.
	 *
	 * The carry is used as a buffer for the rotation.
	 *
	 * See {@link #rotateRegisterLeft(byte&)} for details.
	 * @param reg   the register to rotate to the left by 1 bit.
	 * @opcodes:
	 *     0x10 0x11 0x12 0x13 0x14 0x15 0x17
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
	 * @number_of_ticks: 1
	 */
	void rotateRegisterRight(byte& reg);

	/**
	 * Rotate the value inside the given register to the
	 * right by 1 bit.
	 *
	 * The carry is used as a buffer for the rotation.
	 *
	 * See {@link #rotateRegisterRight(byte&)} for details.
	 *
	 * @param reg   the register to rotate to the right by 1 bit.
	 * @opcodes:
	 *     0x18 0x19 0x1A 0x1B 0x1C 0x1D 0x1F
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

	/**
	 * Shift left arithmetic of a register.
	 * The value is shifted by 1 to the left and the bit 7 is put in the carry flag.
	 *
	 * @param reg   the register to shift
	 *
	 * @opcodes:
	 *     0x20 0x21 0x22 0x23 0x24 0x25 0x27
	 *
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void shiftLeftArithmeticRegister(byte& reg);

	/**
	 * Shift left arithmetic of a memory value.
	 * The value is shifted by 1 to the left and the bit 7 is put in the carry flag.
	 *
	 * @param memoryAddr   the memory address of value to shift
	 *
	 * @opcodes:
	 *     0x26
	 *
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 4
	 */
	void shiftLeftArithmeticMemory(uint16_t memoryAddr);

	/**
	 * Shift right arithmetic of a register.
	 * The value is shifted by 1 to the right and the bit 0 is put in the carry flag.
	 * The bit 7 is kept intact.
	 *
	 * @param reg   the register to shift
	 *
	 * @opcodes:
	 *     0x28 0x29 0x2A 0x2B 0x2C 0x2D 0x2F
	 *
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void shiftRightArithmeticRegister(byte& reg);

	/**
	 * Shift right arithmetic of a memory value.
	 * The value is shifted by 1 to the right and the bit 0 is put in the carry flag.
	 * The bit 7 is kept intact.
	 *
	 * @param memoryAddr   the memory address of value to shift
	 *
	 * @opcodes:
	 *     0x2E
	 *
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 4
	 */
	void shiftRightArithmeticMemory(uint16_t memoryAddr);

	/**
	 * Shift right logical of a register.
	 * The value is shifted by 1 to the right and the bit 0 is put in the carry flag.
	 *
	 * @param reg   the register to shift
	 *
	 * @opcodes:
	 *     0x38 0x39 0x3A 0x3B 0x3C 0x3D 0x3F
	 *
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void shiftRightLogicalRegister(byte& reg);

	/**
	 * Shift right logical of a memory value.
	 * The value is shifted by 1 to the right and the bit 0 is put in the carry flag.
	 *
	 * @param memoryAddr   the memory address of value to shift
	 *
	 * @opcodes:
	 *     0x3E
	 *
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 4
	 */
	void shiftRightLogicalMemory(uint16_t memoryAddr);

	/**
	 * Swap the two nibbles of a register value.
	 *
	 * @param reg   the register to swap
	 *
	 * @opcodes:
	 *     0x30 0x31 0x32 0x33 0x34 0x35 0x37
	 *
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void swapNibblesInRegister(byte& reg);

	/**
	 * Swap the two nibbles of a memory value.
	 *
	 * @param memoryAddr   the memory address of value to swap
	 *
	 * @opcodes:
	 *     0x36
	 *
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 4
	 */
	void swapNibblesInMemory(uint16_t memoryAddr);

	/**
	 * Test if the bit at "bitPosition" is set for the given value.
	 * The Zero flag will be set if the given bit is not set.
	 *
	 * @param reg           the value to test
	 * @param bitPosition   the position of the bit to test ([0,7])
	 *
	 * @opcodes:
	 *     0x40 0x41 0x42 0x43 0x44 0x45 0x47 0x48 0x49 0x4A 0x4B 0x4C 0x4D 0x4F
	 *     0x50 0x51 0x52 0x53 0x54 0x55 0x57 0x58 0x59 0x5A 0x5B 0x5C 0x5D 0x5F
	 *     0x60 0x61 0x62 0x63 0x64 0x65 0x67 0x68 0x69 0x6A 0x6B 0x6C 0x6D 0x6F
	 *     0x70 0x71 0x72 0x73 0x74 0x75 0x77 0x78 0x79 0x7A 0x7B 0x7C 0x7D 0x7F
	 *
	 * @flags_affected: Zero, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void isBitSetForValue(byte value, byte bitPosition);

	/**
	 * Test if the bit at "bitPosition" is set for the value at the given memory address.
	 * The Zero flag will be set if the given bit is not set.
	 *
	 * @param memoryAddr   the memory address of the value to test
	 * @param bitPosition   the position of the bit to test ([0,7])
	 *
	 * @opcodes:
	 *     0x46 0x56 0x66 0x76 0x4E 0x5E 0x6E 0x7E
	 *
	 * @flags_affected: Zero, Half-carry, Substraction
	 * @number_of_ticks: 4
	 */
	void isBitSetInMemory(uint16_t memoryAddr, byte bitPosition);

	/**
	 * Reset the bit at "bitPosition" for the given value.
	 *
	 * @param reg           the value to change
	 * @param bitPosition   the position of the bit to reset ([0,7])
	 *
	 * @opcodes:
	 *     0x80 0x81 0x82 0x83 0x84 0x85 0x87 0x88 0x89 0x8A 0x8B 0x8C 0x8D 0x8F
	 *     0x90 0x91 0x92 0x93 0x94 0x95 0x97 0x98 0x99 0x9A 0x9B 0x9C 0x9D 0x9F
	 *     0xA0 0xA1 0xA2 0xA3 0xA4 0xA5 0xA7 0xA8 0xA9 0xAA 0xAB 0xAC 0xAD 0xAF
	 *     0xB0 0xB1 0xB2 0xB3 0xB4 0xB5 0xB7 0xB8 0xB9 0xBA 0xBB 0xBC 0xBD 0xBF
	 *
	 * @flags_affected: N/A
	 * @number_of_ticks: 2
	 */
	void resetBitForValue(byte& value, byte bitPosition);

	/**
	 * Reset the bit at "bitPosition" at the given memory address.
	 *
	 * @param memoryAddr   the memory address of the value to change
	 * @param bitPosition   the position of the bit to reset ([0,7])
	 *
	 * @opcodes:
	 *     0x86 0x96 0xA6 0xB6 0x8E 0x9E 0xAE 0xBE
	 *
	 * @flags_affected: N/A
	 * @number_of_ticks: 4
	 */
	void resetBitInMemory(uint16_t memoryAddr, byte bitPosition);

	/**
	 * Set the bit at "bitPosition" for the given value.
	 *
	 * @param reg           the value to change
	 * @param bitPosition   the position of the bit to set ([0,7])
	 *
	 * @opcodes:
	 *     0xC0 0xC1 0xC2 0xC3 0xC4 0xC5 0xC7 0xC8 0xC9 0xCA 0xCB 0xCC 0xCD 0xCF
	 *     0xD0 0xD1 0xD2 0xD3 0xD4 0xD5 0xD7 0xD8 0xD9 0xDA 0xDB 0xDC 0xDD 0xDF
	 *     0xE0 0xE1 0xE2 0xE3 0xE4 0xE5 0xE7 0xE8 0xE9 0xEA 0xEB 0xEC 0xED 0xEF
	 *     0xF0 0xF1 0xF2 0xF3 0xF4 0xF5 0xF7 0xF8 0xF9 0xFA 0xFB 0xFC 0xFD 0xFF
	 *
	 * @flags_affected: N/A
	 * @number_of_ticks: 2
	 */
	void setBitForValue(byte& value, byte bitPosition);

	/**
	 * Set the bit at "bitPosition" at the given memory address.
	 *
	 * @param memoryAddr   the memory address of the value to change
	 * @param bitPosition   the position of the bit to set ([0,7])
	 *
	 * @opcodes:
	 *     0xC6 0xD6 0xE6 0xF6 0xCE 0xDE 0xEE 0xFE
	 *
	 * @flags_affected: N/A
	 * @number_of_ticks: 4
	 */
	void setBitInMemory(uint16_t memoryAddr, byte bitPosition);

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

	/**
	 * Arithmetic add an 8 bits value to an 8 bits register
	 *
	 * @param reg      the register to add the value to
	 * @param value    the value to add
	 * @opcodes:
	 *     0x80 0x81 0x82 0x83 0x84 0x85 0x87
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 1
	 */
	void add8BitsValueTo8BitsRegister(byte& reg, byte value);

	/**
	 * Arithmetic add immediate value to an 8 bits register
	 *
	 * @param reg      the register to add the value to
	 * @opcodes:
	 *     0xC6
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void addImmediateValueTo8BitsRegister(byte& reg);

	/**
	 * Arithmetic add value from memory to an 8 bits register
	 *
	 * @param reg      the register to add the value to
	 * @param addrMsb   the msb part of the address where the value is stored
	 * @param addrLsb   the Lsb part of the address where the value is stored
	 * @opcodes:
	 *     0x86
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void addValueFromMemoryTo8BitsRegister(byte& reg, byte addrMsb, byte addrLsb);

	/**
	 * Arithmetic add an 8 bits value and the carry to an 8 bits register
	 *
	 * @param reg      the register to add the value to
	 * @param value    the value to add
	 * @opcodes:
	 *     0x88 0x89 0x8A 0x8B 0x8C 0x8D 0x8F
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 1
	 */
	void add8BitsValueAndCarryTo8BitsRegister(byte& reg, byte value);

	/**
	 * Arithmetic add immediate value and carry to an 8 bits register
	 *
	 * @param reg      the register to add the value to
	 * @opcodes:
	 *     0xCE
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void addImmediateValueAndCarryTo8BitsRegister(byte& reg);

	/**
	 * Arithmetic add value from memory and carry to an 8 bits register
	 *
	 * @param reg      the register to add the value to
	 * @param addrMsb   the msb part of the address where the value is stored
	 * @param addrLsb   the Lsb part of the address where the value is stored
	 * @opcodes:
	 *     0x8E
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void addValueFromMemoryAndCarryTo8BitsRegister(byte& reg, byte addrMsb, byte addrLsb);

	/**
	 * Arithmetic subtract an 8 bits value from an 8 bits register
	 *
	 * @param reg      the register to substract the value from
	 * @param value    the value to substract
	 * @opcodes:
	 *     0x90 0x91 0x92 0x93 0x94 0x95 0x97
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 1
	 */
	void substract8BitsValueFrom8BitsRegister(byte& reg, byte value);

	/**
	 * Arithmetic substract immediate value from an 8 bits register
	 *
	 * @param reg      the register to substract the value from
	 * @opcodes:
	 *     0xCE
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void substractImmediateValueFrom8BitsRegister(byte& reg);

	/**
	 * Arithmetic substract value in memory from an 8 bits register
	 *
	 * @param reg      the register to substract the value from
	 * @param addrMsb   the msb part of the address where the value is stored
	 * @param addrLsb   the Lsb part of the address where the value is stored
	 * @opcodes:
	 *     0x96
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void substractValueInMemoryFrom8BitsRegister(byte& reg, byte addrMsb, byte addrLsb);

	/**
	 * Arithmetic substract an 8 bits value and the carry from an 8 bits register
	 *
	 * @param reg      the register to substract the value from
	 * @param value    the value to substract
	 * @opcodes:
	 *     0x98 0x99 0x9A 0x9B 0x9C 0x9D 0x9F
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 1
	 */
	void sub8BitsValueAndCarryTo8BitsRegister(byte& reg, byte value);

	/**
	 * Arithmetic substract an immediate value and the carry from an 8 bits register
	 *
	 * @param reg      the register to substract the value from
	 * @opcodes:
	 *     0xDE
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void subImmediateValueAndCarryTo8BitsRegister(byte& reg);

	/**
	 * Arithmetic subtract a value from memory and the carry from an 8 bits register
	 *
	 * @param reg      the register to substract the value from
	 * @param addrMsb   the msb part of the address where the value is stored
	 * @param addrLsb   the Lsb part of the address where the value is stored
	 * @opcodes:
	 *     0x9E
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void subValueFromMemoryAndCarryTo8BitsRegister(byte& reg, byte addrMsb, byte addrLsb);

	/**
	 * Logical "AND" between the accumulator and an 8 bits register.
	 * The result will be stored in the accumulator.
	 *
	 * @param reg      the register to use for the second operand
	 * @opcodes:
	 *     0xA0 0xA1 0xA2 0xA3 0xA4 0xA5 0xA7
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 1
	 */
	void logicalAndBetweenAccumulatorAnd8BitsRegister(byte value);

	/**
	 * Logical "AND" between the accumulator and an immediate value.
	 * The result will be stored in the accumulator.
	 *
	 * @opcodes:
	 *     0xE6
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void logicalAndBetweenAccumulatorAndImmediateValue();

	/**
	 * Logical "AND" between the accumulator and a value in memory.
	 *
	 * @param addrMsb   the msb part of the address where the second operand is stored
	 * @param addrLsb   the Lsb part of the address where the second operand is stored
	 * @opcodes:
	 *     0xA6
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void logicalAndBetweenAccumulatorAndValueInMemory(byte addrMsb, byte addrLsb);

	/**
	 * Logical "XOR" between the accumulator and an 8 bits register.
	 * The result will be stored in the accumulator.
	 *
	 * @param reg      the register to use for the second operand
	 * @opcodes:
	 *     0xA8 0xA9 0xAA 0xAB 0xAC 0xAD 0xAF
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 1
	 */
	void logicalXorBetweenAccumulatorAnd8BitsRegister(byte value);

	/**
	 * Logical "XOR" between the accumulator and an immediate value.
	 * The result will be stored in the accumulator.
	 *
	 * @opcodes:
	 *     0xEE
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void logicalXorBetweenAccumulatorAndImmediateValue();

	/**
	 * Logical "XOR" between the accumulator and a value in memory.
	 *
	 * @param addrMsb   the msb part of the address where the second operand is stored
	 * @param addrLsb   the Lsb part of the address where the second operand is stored
	 * @opcodes:
	 *     0xAE
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void logicalXorBetweenAccumulatorAndValueInMemory(byte addrMsb, byte addrLsb);

	/**
	 * Logical "OR" between the accumulator and an 8 bits register.
	 * The result will be stored in the accumulator.
	 *
	 * @param reg      the register to use for the second operand
	 * @opcodes:
	 *     0xB0 0xB1 0xB2 0xB3 0xB4 0xB5 0xB7
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 1
	 */
	void logicalOrBetweenAccumulatorAnd8BitsRegister(byte value);

	/**
	 * Logical "OR" between the accumulator and an immediate value.
	 * The result will be stored in the accumulator.
	 *
	 * @opcodes:
	 *     0xFE
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void logicalOrBetweenAccumulatorAndImmediateValue();

	/**
	 * Logical "OR" between the accumulator and a value in memory.
	 *
	 * @param addrMsb   the msb part of the address where the second operand is stored
	 * @param addrLsb   the Lsb part of the address where the second operand is stored
	 * @opcodes:
	 *     0xB6
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void logicalOrBetweenAccumulatorAndValueInMemory(byte addrMsb, byte addrLsb);

	/**
	 * Compare the accumulator with the value of a register.
	 * If the accumulator and the value are equal, "Zero" flag is set
	 * If the accumulator is greater than the value, "Half carry" flag is set
	 * If the accumulator is smaller than the value, "Carry" flag is set
	 *
	 * @param value      the value to compare to
	 * @opcodes:
	 *     0xB8 0xB9 0xBA 0xBB 0xBC 0xBD 0xBF
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 1
	 */
	void compareAccumulatorAndRegister(byte value);

	/**
	 * Compare the accumulator with a value in memory.
	 * If the accumulator and the value are equal, "Zero" flag is set
	 * If the accumulator is greater than the value, "Half carry" flag is set
	 * If the accumulator is smaller than the value, "Carry" flag is set
	 *
	 * @param addrMsb   the msb part of the address where the value is stored
	 * @param addrLsb   the Lsb part of the address where the value is stored
	 * @opcodes:
	 *     0xBE
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void compareAccumulatorAndValueInMemory(byte addrMsb, byte addrLsb);

	/**
	 * Compare the accumulator with an immediate value.
	 * If the accumulator and the value are equal, "Zero" flag is set
	 * If the accumulator is greater than the value, "Half carry" flag is set
	 * If the accumulator is smaller than the value, "Carry" flag is set
	 *
	 * @opcodes:
	 *     0xFE
	 * @flags_affected: Zero, Carry, Half-carry, Substraction
	 * @number_of_ticks: 2
	 */
	void compareAccumulatorAndImmediateValue();

	/**
	 * Pop contents from the memory stack into register pairs.
	 *
	 * @param regMsb   the register where the msb will be stored
	 * @param regLsb   the register where the lsb will be stored
	 *
	 * @opcodes:
	 *     0xC1 0xD1 0xE1 0xF1
	 * @flags_affected: N/A
	 * @number_of_ticks: 3
	 */
	void popMemoryIntoRegisterPair(byte& regMsb, byte& regLsb);

	/**
	 * Enable interrupts routine.
	 *
	 * @opcodes:
	 *     0xFB
	 * @flags_affected: N/A
	 * @number_of_ticks: 1
	 */
	void enableInterrupts();

	/**
	 * Stop system clock and oscillator circuit.
	 *
	 * @opcodes:
	 *     0x10
	 * @flags_affected: N/A
	 * @number_of_ticks: 1
	 */
	void stopInstruction();

	/**
	 * Stop system clock and enter halt mode.
	 * Oscillator circuit and LCD controller continue to operate.
	 *
	 * @opcodes:
	 *     0x76
	 * @flags_affected: N/A
	 * @number_of_ticks: 1
	 */
	void haltInstruction();

	/**
	 * Return control to the caller of the subroutine.
	 *
	 * @opcodes:
	 *     0xC9
	 * @flags_affected: N/A
	 * @number_of_ticks: 4
	 */
	void returnInstruction();

	/**
	 * Return control to the caller of the subroutine if the condition is satisfied.
	 *
	 * @opcodes:
	 *     0xCA 0xDA 0xC8 0xD8
	 * @flags_affected: N/A
	 * @number_of_ticks: 5 if condition satisfied, 2 otherwise
	 */
	void returnInstructionConditional(bool cond);

	/**
	 * Return control to the caller of the subroutine after an interrupt.
	 *
	 * @opcodes:
	 *     0xD9
	 * @flags_affected: N/A
	 * @number_of_ticks: 4
	 */
	void returnInstructionAfterInterrupt();

	/**
	 * Push the current program counter on the stack pointer
	 * and load the immediate value in the program counter.
	 *
	 * @opcodes:
	 *     0xCD
	 * @flags_affected: N/A
	 * @number_of_ticks: 6
	 */
	void callImmediateSubroutine();

	/**
	 * Push the current program counter on the stack pointer
	 * and load the immediate value in the program counter.
	 *
	 * @opcodes:
	 *     0xC4 0xD4 0xCC 0xDC
	 * @flags_affected: N/A
	 * @number_of_ticks: 6 if condition satisfied, 3 otherwise
	 */
	void callImmediateSubroutineIfConditionSatisfied(bool cond);

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

	/**
	 * Logical complement (XOR) of the given register
	 *
	 * @opcodes:
	 *     0x2F
	 * @flags_affected: Substraction, Half-carry
	 * @number_of_ticks: 1
	 */
	void complementRegister(byte& reg);

	/**
	 * Set the carry flag
	 *
	 * @opcodes:
	 *     0x37
	 * @flags_affected: Substraction, Half-carry, Carry
	 * @number_of_ticks: 1
	 */
	void setCarryFlagInstruction();

	/**
	 * Inverts the carry flag
	 *
	 * @opcodes:
	 *     0x3F
	 * @flags_affected: Substraction, Half-carry, Carry
	 * @number_of_ticks: 1
	 */
	void invertsCarryFlag();

	/**
	 * Push the content of a register pair onto the stack pointer.
	 *
	 * @param regMsb   the register containing the msb that will be pushed on the sp
	 * @param regLsb   the register containing the lsb that will be pushed on the sp
	 *
	 * @opcodes:
	 *     0xC5 0xD5 0xE5 0xF5
	 * @flags_affected: N/A
	 * @number_of_ticks: 4
	 */
	void push16BitsOntoStackPointer(byte regMsb, byte regLsb);

	/**
	 * Call one of the predefined restart routine.
	 * The program counter will jump to the routine at the given address.
	 *
	 * @param memoryAddr       the address of the routine
	 *
	 * @opcodes:
	 *     0xC7 0xD7 0xE7 0xF7 0xCF 0xDF 0xEF 0xFF
	 * @flags_affected: N/A
	 * @number_of_ticks: 4
	 */
	void callRestartRoutine(byte memoryAddr);

	/**
	 * Load the content of the accumulator to the memory address
	 * ($FF00 + immediate byte)
	 *
	 * @opcodes:
	 *     0xE0
	 * @flags_affected: N/A
	 * @number_of_ticks: 3
	 */
	void loadAccumulatorInHighMemoryValue();

	/**
	 * Load the content of memory address ($FF00 + immediate byte)
	 * into accumulator
	 *
	 * @opcodes:
	 *     0xF0
	 * @flags_affected: N/A
	 * @number_of_ticks: 3
	 */
	void loadHighMemoryValueInAccumulator();

	/**
	 * Load the content of the accumulator to the memory address
	 * ($FF00 + other register value)
	 *
	 * @param   reg     the register to use for the memory value
	 * @opcodes:
	 *     0xE2
	 * @flags_affected: N/A
	 * @number_of_ticks: 2
	 */
	void loadAccumulatorInHighMemoryValue(byte reg);

	/**
	 * Load the content of memory address ($FF00 + other register value)
	 * into accumulator
	 *
	 * @param   reg     the register to use for the memory value
	 *
	 * @opcodes:
	 *     0xF2
	 * @flags_affected: N/A
	 * @number_of_ticks: 2
	 */
	void loadHighMemoryValueInAccumulator(byte reg);

	/**
	 * Add the content of the 8-bits immediate value to the stack pointer.
	 *
	 * @opcodes:
	 *     0xE8
	 * @flags_affected: Zero, Substraction, Half-Carry, Carry
	 * @number_of_ticks: 4
	 */
	void addImmediateValueToStackPointer();

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

	void handleInterrupts();

	/**
	 * Reset all CPU flags
	 */
	void resetFlags();

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

	// Whether or not interrupts are enabled
	bool interruptsEnabled;

	bool interruptsEnabledRequested;

	std::vector<std::unique_ptr<InterruptHandler>> interruptHandlers = {};
};

#endif
