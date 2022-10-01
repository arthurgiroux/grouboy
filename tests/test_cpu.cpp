#include "cpu.hpp"
#include "instructions.hpp"
#include "utils.hpp"
#include <gtest/gtest.h>

using namespace utils;

class CpuTest : public ::testing::Test
{
  protected:
	MMU mmu;
	CPU cpu = CPU(mmu);

	void assertFlagCanBeSetAndUnset(CPU::CpuFlags flag)
	{
		cpu.setFlag(flag);
		ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), flag == CPU::CpuFlags::ZERO);
		ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION), flag == CPU::CpuFlags::SUBSTRACTION);
		ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), flag == CPU::CpuFlags::CARRY);
		ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), flag == CPU::CpuFlags::HALF_CARRY);
		cpu.unsetFlag(flag);

		ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
		ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
		ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
		ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));

		cpu.setFlagIfTrue(true, flag);
		ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), flag == CPU::CpuFlags::ZERO);
		ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION), flag == CPU::CpuFlags::SUBSTRACTION);
		ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), flag == CPU::CpuFlags::CARRY);
		ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), flag == CPU::CpuFlags::HALF_CARRY);
		cpu.setFlagIfTrue(false, flag);

		ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
		ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
		ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
		ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
	}
};

class CpuInstructionTest : public ::testing::Test
{
  protected:
	MMU mmu;
	CPU cpu = CPU(mmu);

	void performInstructionAndAssertTicksAndFlag(byte instruction, int expectedTicks, byte expectedFlag)
	{
		mmu.write(0, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, expectedTicks);
		ASSERT_EQ(cpu.tick, expectedTicks);
		ASSERT_EQ(cpu.getFlag(), expectedFlag);
	}

	void testLoad16BitsValueInRegisters(byte instruction, uint16_t value, byte& msbRegister, byte& lsbRegister)
	{
		cpu.pc = 0x00;
		mmu.write(cpu.pc, instruction);
		mmu.writeWord(cpu.pc + 1, value);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 3);
		ASSERT_EQ(cpu.getFlag(), 0x00);
		ASSERT_EQ(msbRegister, value >> 8);
		ASSERT_EQ(lsbRegister, value & 0xFF);
		ASSERT_EQ(cpu.pc, 0x03);
	}

	void testLoad16BitsValueInRegisters(byte instruction, uint16_t value, uint16_t& reg)
	{
		cpu.pc = 0x00;
		mmu.write(cpu.pc, instruction);
		mmu.writeWord(cpu.pc + 1, value);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 3);
		ASSERT_EQ(cpu.getFlag(), 0x00);
		ASSERT_EQ(reg, value);
		ASSERT_EQ(cpu.pc, 0x03);
	}

	void testIncrement8BitsRegisters(byte instruction, const byte& msbRegister, const byte& lsbRegister)
	{
		cpu.pc = 0x00;
		uint16_t expected_pc = 1;
		for (int i = 1; i <= UINT16_MAX + 1; ++i)
		{
			uint16_t expected_value = i;
			mmu.write(cpu.pc, instruction);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, 2);
			ASSERT_EQ(cpu.getFlag(), 0x00);
			uint16_t regValue = (msbRegister << 8u) | lsbRegister;
			ASSERT_EQ(regValue, expected_value);
			ASSERT_EQ(cpu.pc, expected_pc);
			expected_pc++;
		}
	}

	void testIncrement16BitsRegister(byte instruction, const uint16_t& reg)
	{
		cpu.pc = 0x00;
		uint16_t expected_pc = 1;
		for (int i = 1; i <= UINT16_MAX + 1; ++i)
		{
			uint16_t expected_value = i;
			mmu.write(cpu.pc, instruction);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, 2);
			ASSERT_EQ(cpu.getFlag(), 0x00);
			ASSERT_EQ(reg, expected_value);
			ASSERT_EQ(cpu.pc, expected_pc);
			expected_pc++;
		}
	}

	void testIncrement8BitsMemoryValue(byte instruction, byte& msbRegister, byte& lsbRegister)
	{
		cpu.pc = 0x00;
		msbRegister = 0x0F;
		lsbRegister = 0xFF;
		mmu.write(createAddrFromHighAndLowBytes(msbRegister, lsbRegister), 0);
		uint16_t expected_pc = 1;
		for (int i = 1; i <= UINT8_MAX + 1; ++i)
		{
			byte expected_value = i;
			mmu.write(cpu.pc, instruction);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, 3);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), expected_value == 0x10);
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expected_value == 0);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
			ASSERT_EQ(mmu.read(createAddrFromHighAndLowBytes(msbRegister, lsbRegister)), expected_value);
			ASSERT_EQ(cpu.pc, expected_pc);
			expected_pc++;
		}
	}

	void testDecrement8BitsRegisters(byte instruction, const byte& msbRegister, const byte& lsbRegister)
	{
		cpu.pc = 0x00;
		uint16_t expected_pc = 1;
		for (int i = UINT16_MAX; i >= 0; --i)
		{
			uint16_t expected_value = i;
			mmu.write(cpu.pc, instruction);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, 2);
			ASSERT_EQ(cpu.getFlag(), 0x00);
			uint16_t regValue = (msbRegister << 8u) | lsbRegister;
			ASSERT_EQ(regValue, expected_value);
			ASSERT_EQ(cpu.pc, expected_pc);
			expected_pc++;
		}
	}

	void testDecrement16BitsRegister(byte instruction, const uint16_t& reg)
	{
		cpu.pc = 0x00;
		uint16_t expected_pc = 1;
		for (int i = UINT16_MAX; i >= 0; --i)
		{
			uint16_t expected_value = i;
			mmu.write(cpu.pc, instruction);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, 2);
			ASSERT_EQ(cpu.getFlag(), 0x00);
			ASSERT_EQ(reg, expected_value);
			ASSERT_EQ(cpu.pc, expected_pc);
			expected_pc++;
		}
	}

	void testDecrement8BitsMemoryValue(byte instruction, byte& msbRegister, byte& lsbRegister)
	{
		cpu.pc = 0x00;
		msbRegister = 0x0F;
		lsbRegister = 0xFF;
		mmu.write(createAddrFromHighAndLowBytes(msbRegister, lsbRegister), 0);
		uint16_t expected_pc = 1;
		for (int i = UINT8_MAX; i >= 0; --i)
		{
			byte expected_value = i;
			mmu.write(cpu.pc, instruction);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, 3);
			ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), expected_value == 0x0F);
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expected_value == 0);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
			ASSERT_EQ(mmu.read(createAddrFromHighAndLowBytes(msbRegister, lsbRegister)), expected_value);
			ASSERT_EQ(cpu.pc, expected_pc);
			expected_pc++;
		}
	}

	void testRotateLeftCircular(const std::vector<byte>& instructions, byte& reg, int expectedTicks, bool isZeroFlagUsed = true)
	{
		// We test the zero value first
		reg = 0b00000000;
		cpu.pc = 0x00;
		for (int i = 0; i < 10; ++i)
		{
			// Once zero is tested, we inject a bit to test all values
			if (i == 1) {
				reg = 0b00000001;
			}
			for (size_t j = 0; j < instructions.size(); ++j)
			{
				mmu.write(static_cast<uint16_t>(cpu.pc + j), instructions[j]);
			}
			byte registerValue = reg;
			byte expectedValue = (registerValue << 1) | (registerValue >> 7);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, expectedTicks);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), isZeroFlagUsed && expectedValue == 0);
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (registerValue & 0x80) > 0);
			ASSERT_EQ(reg, expectedValue);
		}
	}

	void testRotateLeftCircularFromMemory(const std::vector<byte>& instructions, byte& regMsb, byte& regLsb,
	                                      int expectedTicks)
	{
		regMsb = 0x42;
		regLsb = 0x11;
		uint16_t addr = createAddrFromHighAndLowBytes(regMsb, regLsb);
		mmu.write(addr, 0b00000001);
		cpu.pc = 0x00;
		for (int i = 0; i < 9; ++i)
		{
			for (size_t j = 0; j < instructions.size(); ++j)
			{
				mmu.write(static_cast<uint16_t>(cpu.pc + j), instructions[j]);
			}
			byte registerValue = mmu.read(addr);
			byte expectedValue = (registerValue << 1) | (registerValue >> 7);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, expectedTicks);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expectedValue == 0);
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (registerValue & 0x80) > 0);
			ASSERT_EQ(mmu.read(addr), expectedValue);
		}
	}

	void testRotateLeft(const std::vector<byte>& instructions, byte& reg, int expectedTicks, bool isZeroFlagUsed = true)
	{
		reg = 0b00000001;
		cpu.pc = 0x00;
		for (int i = 0; i < 9; ++i)
		{
			for (size_t j = 0; j < instructions.size(); ++j)
			{
				mmu.write(static_cast<uint16_t>(cpu.pc + j), instructions[j]);
			}
			byte registerValue = reg;
			bool isCarryFlagSet = cpu.isFlagSet(CPU::CpuFlags::CARRY);
			byte expectedValue = (registerValue << 1) | static_cast<int>(isCarryFlagSet);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, expectedTicks);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), isZeroFlagUsed && expectedValue == 0);
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (registerValue & 0x80) > 0);
			ASSERT_EQ(reg, expectedValue);
		}
	}

	void testRotateLeftFromMemory(const std::vector<byte>& instructions, byte& regMsb, byte& regLsb, int expectedTicks)
	{
		regMsb = 0x42;
		regLsb = 0x11;
		uint16_t addr = createAddrFromHighAndLowBytes(regMsb, regLsb);
		mmu.write(addr, 0b00000001);
		cpu.pc = 0x00;
		for (int i = 0; i < 9; ++i)
		{
			for (size_t j = 0; j < instructions.size(); ++j)
			{
				mmu.write(static_cast<uint16_t>(cpu.pc + j), instructions[j]);
			}
			byte registerValue = mmu.read(addr);
			bool isCarryFlagSet = cpu.isFlagSet(CPU::CpuFlags::CARRY);
			byte expectedValue = (registerValue << 1) | static_cast<int>(isCarryFlagSet);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, expectedTicks);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expectedValue == 0);
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (registerValue & 0x80) > 0);
			ASSERT_EQ(mmu.read(addr), expectedValue);
		}
	}

	void addTwo8BitsRegisterToTwo8BitsRegister(byte instruction, byte& resultRegMsb, byte& resultRegLsb,
	                                           byte& valueRegMsb, byte& valueRegLsb, std::vector<uint16_t>& startValues,
	                                           std::vector<uint16_t>& addValues, std::vector<int>& expectedFlags)
	{
		for (size_t i = 0; i < startValues.size(); ++i)
		{
			uint16_t startValue = startValues[i];
			uint16_t addValue = addValues[i];
			resultRegLsb = startValue & 0xFF;
			resultRegMsb = (startValue >> 8) & 0xFF;
			valueRegLsb = addValue & 0xFF;
			valueRegMsb = (addValue >> 8) & 0xFF;
			uint32_t expectedValue = startValue + addValue;
			mmu.write(cpu.pc, instruction);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, 2);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), ((expectedFlags[i] & CPU::CpuFlags::CARRY) > 0));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), ((expectedFlags[i] & CPU::CpuFlags::HALF_CARRY) > 0));
		}
	}

	void add16BitsRegisterToTwo8BitsRegister(byte instruction, byte& resultRegMsb, byte& resultRegLsb,
	                                         uint16_t& valueReg, std::vector<uint16_t>& startValues,
	                                         std::vector<uint16_t>& addValues, std::vector<int>& expectedFlags)
	{
		for (size_t i = 0; i < startValues.size(); ++i)
		{
			uint16_t startValue = startValues[i];
			uint16_t addValue = addValues[i];
			resultRegLsb = startValue & 0xFF;
			resultRegMsb = (startValue >> 8) & 0xFF;
			valueReg = addValue;
			uint32_t expectedValue = startValue + addValue;
			mmu.write(cpu.pc, instruction);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, 2);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), ((expectedFlags[i] & CPU::CpuFlags::CARRY) > 0));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), ((expectedFlags[i] & CPU::CpuFlags::HALF_CARRY) > 0));
		}
	}

	void testRotateRightCircular(const std::vector<byte>& instructions, byte& reg, int expectedTicks, bool isZeroFlagUsed = true)
	{
        // We test the zero value first
        reg = 0b00000000;
		cpu.pc = 0x00;
		for (int i = 0; i < 10; ++i)
		{
            // Once zero is tested, we inject a bit to test all values
            if (i == 1) {
				reg = 0b10000000;
			}

			for (size_t j = 0; j < instructions.size(); ++j)
			{
				mmu.write(static_cast<uint16_t>(cpu.pc + j), instructions[j]);
			}
			byte registerValue = reg;
			byte expectedValue = (registerValue << 7) | (registerValue >> 1);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, expectedTicks);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), isZeroFlagUsed &&expectedValue == 0);
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (registerValue & 0x01) > 0);
			ASSERT_EQ(reg, expectedValue);
		}
	}

	void testRotateRightCircularFromMemory(const std::vector<byte>& instructions, byte& regMsb, byte& regLsb,
	                                       int expectedTicks)
	{
		regMsb = 0x42;
		regLsb = 0x11;
		uint16_t addr = createAddrFromHighAndLowBytes(regMsb, regLsb);
		mmu.write(addr, 0b10000000);
		cpu.pc = 0x00;
		for (int i = 0; i < 9; ++i)
		{
			for (size_t j = 0; j < instructions.size(); ++j)
			{
				mmu.write(static_cast<uint16_t>(cpu.pc + j), instructions[j]);
			}
			byte registerValue = mmu.read(addr);
			byte expectedValue = (registerValue << 7) | (registerValue >> 1);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, expectedTicks);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expectedValue == 0);
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (registerValue & 0x01) > 0);
			ASSERT_EQ(mmu.read(addr), expectedValue);
		}
	}

	void testRotateRight(const std::vector<byte>& instructions, byte& reg, int expectedTicks, bool isZeroFlagUsed = true)
	{
		reg = 0b10000000;
		cpu.pc = 0x00;
		for (int i = 0; i < 9; ++i)
		{
			for (size_t j = 0; j < instructions.size(); ++j)
			{
				mmu.write(static_cast<uint16_t>(cpu.pc + j), instructions[j]);
			}
			byte registerValue = reg;
			bool isCarryFlagSet = cpu.isFlagSet(CPU::CpuFlags::CARRY);
			byte expectedValue = (isCarryFlagSet << 7) | (registerValue >> 1);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, expectedTicks);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), isZeroFlagUsed && expectedValue == 0);
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (registerValue & 0x01) > 0);
			ASSERT_EQ(reg, expectedValue);
		}
	}

	void testRotateRightFromMemory(const std::vector<byte>& instructions, byte& regMsb, byte& regLsb, int expectedTicks)
	{
		regMsb = 0x42;
		regLsb = 0x11;
		uint16_t addr = createAddrFromHighAndLowBytes(regMsb, regLsb);
		mmu.write(addr, 0b10000000);
		cpu.pc = 0x00;
		for (int i = 0; i < 9; ++i)
		{
			for (size_t j = 0; j < instructions.size(); ++j)
			{
				mmu.write(static_cast<uint16_t>(cpu.pc + j), instructions[j]);
			}
			byte registerValue = mmu.read(addr);
			bool isCarryFlagSet = cpu.isFlagSet(CPU::CpuFlags::CARRY);
			byte expectedValue = (isCarryFlagSet << 7) | (registerValue >> 1);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, expectedTicks);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expectedValue == 0);
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (registerValue & 0x01) > 0);
			ASSERT_EQ(mmu.read(addr), expectedValue);
		}
	}

	void testOp8BitsRegisterTo8BitsRegister(byte instruction, byte& reg, byte startValue, byte& regOther,
	                                        byte otherValue, byte expectedResult, bool carryFlag,
	                                        bool expectedHalfCarryFlag, bool expectedCarryFlag,
	                                        bool expectedSubstractionFlag)
	{
		cpu.pc = 0x00;
		reg = startValue;
		regOther = otherValue;
		cpu.setCarryFlag(carryFlag);
		mmu.write(cpu.pc, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 1);
		ASSERT_EQ(reg, expectedResult);
		ASSERT_EQ(cpu.isFlagSet(CPU::HALF_CARRY), expectedHalfCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::CARRY), expectedCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::SUBSTRACTION), expectedSubstractionFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::ZERO), expectedResult == 0);
		ASSERT_EQ(cpu.pc, 1);
	}

	void testOpFromMemoryToRegister(byte instruction, byte& reg, byte startValue, byte regAddrMsb, byte regAddrLsb,
	                                byte value, byte expectedResult, bool carryFlag, bool expectedHalfCarryFlag,
	                                bool expectedCarryFlag, bool expectedSubstractionFlag)
	{
		cpu.pc = 0x00;
		reg = startValue;
		mmu.write(createAddrFromHighAndLowBytes(regAddrMsb, regAddrLsb), value);
		mmu.write(cpu.pc, instruction);
		cpu.setCarryFlag(carryFlag);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_EQ(reg, expectedResult);
		ASSERT_EQ(cpu.isFlagSet(CPU::HALF_CARRY), expectedHalfCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::CARRY), expectedCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::SUBSTRACTION), expectedSubstractionFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::ZERO), expectedResult == 0);
		ASSERT_EQ(cpu.pc, 1);
	}

	void testOpImmediateValueToRegister(byte instruction, byte& reg, byte startValue, byte value, byte expectedResult,
	                                    bool carryFlag, bool expectedHalfCarryFlag, bool expectedCarryFlag,
	                                    bool expectedSubstractionFlag)
	{
		cpu.pc = 0x00;
		reg = startValue;
		mmu.write(cpu.pc, instruction);
		mmu.write(cpu.pc + 1, value);
		cpu.setCarryFlag(carryFlag);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_EQ(reg, expectedResult);
		ASSERT_EQ(cpu.isFlagSet(CPU::HALF_CARRY), expectedHalfCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::CARRY), expectedCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::SUBSTRACTION), expectedSubstractionFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::ZERO), expectedResult == 0);
		ASSERT_EQ(cpu.pc, 2);
	}

	void testAdd8BitsRegisterTo8BitsRegister(byte instruction, byte& reg, byte startValue, byte& otherReg, byte value,
	                                         byte expectedResult, bool carryFlag, bool expectedHalfCarryFlag,
	                                         bool expectedCarryFlag)
	{
		testOp8BitsRegisterTo8BitsRegister(instruction, reg, startValue, otherReg, value, expectedResult, carryFlag,
		                                   expectedHalfCarryFlag, expectedCarryFlag, false);
	}

	void testAddValueFromMemoryToRegister(byte instruction, byte& reg, byte startValue, byte regAddrMsb,
	                                      byte regAddrLsb, byte value, byte expectedResult, bool carryFlag,
	                                      bool expectedHalfCarryFlag, bool expectedCarryFlag)
	{
		testOpFromMemoryToRegister(instruction, reg, startValue, regAddrMsb, regAddrLsb, value, expectedResult,
		                           carryFlag, expectedHalfCarryFlag, expectedCarryFlag, false);
	}

	void testAddImmediateValueToRegister(byte instruction, byte& reg, byte startValue, byte value, byte expectedResult,
	                                     bool carryFlag, bool expectedHalfCarryFlag, bool expectedCarryFlag)
	{
		testOpImmediateValueToRegister(instruction, reg, startValue, value, expectedResult, carryFlag,
		                               expectedHalfCarryFlag, expectedCarryFlag, false);
	}

	void testAdd8BitsRegisterTo8BitsRegister(byte instruction, byte& reg, byte& otherReg)
	{
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0x00, false, false, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0x05, false, false, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x0F, otherReg, 0x05, 0x14, false, true, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x05, 0x04, false, true, true);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x01, 0x00, false, true, true);
        testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0xF0, otherReg, 0xF0, 0xE0, false, false, true);
	}

	void testAdd8BitsRegisterAndCarryTo8BitsRegister(byte instruction, byte& reg, byte& otherReg)
	{
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0x00, false, false, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0x01, true, false, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0x05, false, false, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0x06, true, false, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x0F, otherReg, 0x00, 0x10, true, true, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x0F, otherReg, 0x05, 0x14, false, true, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x05, 0x04, false, true, true);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x00, 0x00, true, true, true);
	}

	void testSub8BitsRegisterTo8BitsRegister(byte instruction, byte& reg, byte startValue, byte& otherReg, byte value,
	                                         byte expectedResult, bool carryFlag, bool expectedHalfCarryFlag,
	                                         bool expectedCarryFlag)
	{
		testOp8BitsRegisterTo8BitsRegister(instruction, reg, startValue, otherReg, value, expectedResult, carryFlag,
		                                   expectedHalfCarryFlag, expectedCarryFlag, true);
	}

	void testSubValueFromMemoryToRegister(byte instruction, byte& reg, byte startValue, byte regAddrMsb,
	                                      byte regAddrLsb, byte value, byte expectedResult, bool carryFlag,
	                                      bool expectedHalfCarryFlag, bool expectedCarryFlag)
	{
		testOpFromMemoryToRegister(instruction, reg, startValue, regAddrMsb, regAddrLsb, value, expectedResult,
		                           carryFlag, expectedHalfCarryFlag, expectedCarryFlag, true);
	}

	void testSubImmediateValueToRegister(byte instruction, byte& reg, byte startValue, byte value, byte expectedResult,
	                                     bool carryFlag, bool expectedHalfCarryFlag, bool expectedCarryFlag)
	{
		testOpImmediateValueToRegister(instruction, reg, startValue, value, expectedResult, carryFlag,
		                               expectedHalfCarryFlag, expectedCarryFlag, true);
	}

	void testSub8BitsRegisterTo8BitsRegister(byte instruction, byte& reg, byte& otherReg)
	{
		testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0x00, false, false, false);
		testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0xFB, false, true, true);
		testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x10, otherReg, 0x05, 0x0B, false, true, false);
		testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x05, 0xFA, false, false, false);
		testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x01, 0xFF, false, true, true);
	}

	void testSub8BitsRegisterAndCarryTo8BitsRegister(byte instruction, byte& reg, byte& otherReg)
	{
		testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0x00, false, false, false);
		testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0xFF, true, true, true);
		testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0xFB, false, true, true);
		testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0xFA, true, true, true);
		testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x10, otherReg, 0x00, 0x0F, true, true, false);
		testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x10, otherReg, 0x05, 0x0B, false, true, false);
		testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x05, 0xFA, false, false, false);
		testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x00, 0xFE, true, false, false);
	}

	void testAndOperationWithRegister(byte instruction, byte startValue, byte& reg, byte value, byte expectedResult)
	{
		testOp8BitsRegisterTo8BitsRegister(instruction, cpu.a, startValue, reg, value, expectedResult, true, true,
		                                   false, false);
	}

	void testAndOperationWithRegister(byte instruction, byte& reg)
	{
		testAndOperationWithRegister(instruction, 0x00, reg, 0x00, 0x00);
		testAndOperationWithRegister(instruction, 0x0F, reg, 0x00, 0x00);
		testAndOperationWithRegister(instruction, 0x00, reg, 0x0F, 0x00);
		testAndOperationWithRegister(instruction, 0x0F, reg, 0x0F, 0x0F);
		testAndOperationWithRegister(instruction, 0xF0, reg, 0xF0, 0xF0);
		testAndOperationWithRegister(instruction, 0x01, reg, 0x01, 0x01);
		testAndOperationWithRegister(instruction, 0x10, reg, 0x10, 0x10);
	}

	void testAndOperationWithImmediateValue(byte instruction, byte startValue, byte value, byte expectedResult)
	{
		testOpImmediateValueToRegister(instruction, cpu.a, startValue, value, expectedResult, true, true, false, false);
	}

	void testAndOperationWithMemory(byte instruction, byte startValue, byte regAddrMsb, byte regAddrLsb, byte value,
	                                byte expectedResult)
	{
		testOpFromMemoryToRegister(instruction, cpu.a, startValue, regAddrMsb, regAddrLsb, value, expectedResult, true,
		                           true, false, false);
	}

	void testXorOperationWithRegister(byte instruction, byte startValue, byte& reg, byte value, byte expectedResult)
	{
		testOp8BitsRegisterTo8BitsRegister(instruction, cpu.a, startValue, reg, value, expectedResult, true, false,
		                                   false, false);
	}

	void testXorOperationWithRegister(byte instruction, byte& reg)
	{
		testXorOperationWithRegister(instruction, 0x00, reg, 0x00, 0x00);
		testXorOperationWithRegister(instruction, 0x0F, reg, 0x00, 0x0F);
		testXorOperationWithRegister(instruction, 0x00, reg, 0x0F, 0x0F);
		testXorOperationWithRegister(instruction, 0x0F, reg, 0x0F, 0x00);
		testXorOperationWithRegister(instruction, 0xF0, reg, 0xF0, 0x00);
		testXorOperationWithRegister(instruction, 0x01, reg, 0x01, 0x00);
		testXorOperationWithRegister(instruction, 0x10, reg, 0x10, 0x00);
		testXorOperationWithRegister(instruction, 0x01, reg, 0x00, 0x01);
		testXorOperationWithRegister(instruction, 0x00, reg, 0x01, 0x01);
		testXorOperationWithRegister(instruction, 0xFF, reg, 0xFF, 0x00);
	}

	void testXorOperationWithImmediateValue(byte instruction, byte startValue, byte value, byte expectedResult)
	{
		testOpImmediateValueToRegister(instruction, cpu.a, startValue, value, expectedResult, true, false, false,
		                               false);
	}

	void testXorOperationWithMemory(byte instruction, byte startValue, byte regAddrMsb, byte regAddrLsb, byte value,
	                                byte expectedResult)
	{
		testOpFromMemoryToRegister(instruction, cpu.a, startValue, regAddrMsb, regAddrLsb, value, expectedResult, true,
		                           false, false, false);
	}

	void testOrOperationWithRegister(byte instruction, byte startValue, byte& reg, byte value, byte expectedResult)
	{
		testOp8BitsRegisterTo8BitsRegister(instruction, cpu.a, startValue, reg, value, expectedResult, true, false,
		                                   false, false);
	}

	void testOrOperationWithRegister(byte instruction, byte& reg)
	{
		testOrOperationWithRegister(instruction, 0x00, reg, 0x00, 0x00);
		testOrOperationWithRegister(instruction, 0x0F, reg, 0x00, 0x0F);
		testOrOperationWithRegister(instruction, 0x00, reg, 0x0F, 0x0F);
		testOrOperationWithRegister(instruction, 0x0F, reg, 0x0F, 0x0F);
		testOrOperationWithRegister(instruction, 0xF0, reg, 0xF0, 0xF0);
		testOrOperationWithRegister(instruction, 0x01, reg, 0x01, 0x01);
		testOrOperationWithRegister(instruction, 0x10, reg, 0x10, 0x10);
		testOrOperationWithRegister(instruction, 0x01, reg, 0x00, 0x01);
		testOrOperationWithRegister(instruction, 0x00, reg, 0x01, 0x01);
		testOrOperationWithRegister(instruction, 0xFF, reg, 0xFF, 0xFF);
	}

	void testOrOperationWithImmediateValue(byte instruction, byte startValue, byte value, byte expectedResult)
	{
		testOpImmediateValueToRegister(instruction, cpu.a, startValue, value, expectedResult, true, false, false,
		                               false);
	}

	void testOrOperationWithMemory(byte instruction, byte startValue, byte regAddrMsb, byte regAddrLsb, byte value,
	                               byte expectedResult)
	{
		testOpFromMemoryToRegister(instruction, cpu.a, startValue, regAddrMsb, regAddrLsb, value, expectedResult, true,
		                           false, false, false);
	}

	void testCompareOperationWithRegister(byte instruction, byte startValue, byte& reg, byte value, int expectedFlags)
	{
		cpu.pc = 0x00;
		cpu.a = startValue;
		reg = value;
		mmu.write(cpu.pc, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 1);
		ASSERT_TRUE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_EQ(cpu.isFlagSet(CPU::ZERO), (expectedFlags & CPU::ZERO) > 0);
		ASSERT_EQ(cpu.isFlagSet(CPU::CARRY), (expectedFlags & CPU::CARRY) > 0);
		ASSERT_EQ(cpu.isFlagSet(CPU::HALF_CARRY), (expectedFlags & CPU::HALF_CARRY) > 0);
		ASSERT_EQ(cpu.pc, 1);
	}

	void testCompareOperationWithRegister(byte instruction, byte& reg)
	{
		testCompareOperationWithRegister(instruction, 0x00, reg, 0x00, CPU::ZERO);
		testCompareOperationWithRegister(instruction, 0x0F, reg, 0x00, 0);
		testCompareOperationWithRegister(instruction, 0x00, reg, 0x0F, CPU::CARRY | CPU::HALF_CARRY);
		testCompareOperationWithRegister(instruction, 0xFF, reg, 0xFF, CPU::ZERO);
		testCompareOperationWithRegister(instruction, 0x09, reg, 0x08, 0);
		testCompareOperationWithRegister(instruction, 0x08, reg, 0x09, CPU::CARRY | CPU::HALF_CARRY);
	}

	void testCompareOperationWithImmediateValue(byte instruction, byte startValue, byte value, int expectedFlags)
	{
		cpu.pc = 0x00;
		cpu.a = startValue;
		mmu.write(cpu.pc, instruction);
		mmu.write(cpu.pc + 1, value);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_TRUE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_EQ(cpu.isFlagSet(CPU::ZERO), (expectedFlags & CPU::ZERO) > 0);
		ASSERT_EQ(cpu.isFlagSet(CPU::CARRY), (expectedFlags & CPU::CARRY) > 0);
		ASSERT_EQ(cpu.isFlagSet(CPU::HALF_CARRY), (expectedFlags & CPU::HALF_CARRY) > 0);
		ASSERT_EQ(cpu.pc, 2);
	}

	void testCompareOperationWithMemory(byte instruction, byte startValue, byte regAddrMsb, byte regAddrLsb, byte value,
	                                    int expectedFlags)
	{
		cpu.pc = 0x00;
		cpu.a = startValue;
		mmu.write(createAddrFromHighAndLowBytes(regAddrMsb, regAddrLsb), value);
		mmu.write(cpu.pc, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_TRUE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_EQ(cpu.isFlagSet(CPU::ZERO), (expectedFlags & CPU::ZERO) > 0);
		ASSERT_EQ(cpu.isFlagSet(CPU::CARRY), (expectedFlags & CPU::CARRY) > 0);
		ASSERT_EQ(cpu.isFlagSet(CPU::HALF_CARRY), (expectedFlags & CPU::HALF_CARRY) > 0);
		ASSERT_EQ(cpu.pc, 1);
	}

	void testReturnConditional(int instruction, bool expectReturn) {
        uint16_t callerAddr = 0x1234;
        mmu.writeWord(cpu.pc, callerAddr);
        cpu.pc += 2;
        mmu.write(cpu.pc, instruction);
        int ticks = cpu.fetchDecodeAndExecute();
		if (expectReturn) {
            ASSERT_EQ(ticks, 5);
            ASSERT_EQ(cpu.pc, callerAddr);
        }
		else {
            ASSERT_EQ(ticks, 2);
            ASSERT_EQ(cpu.pc, 0x03);
		}
	}
};

TEST_F(CpuTest, RegistersValueAtInitAreCorrect)
{
	ASSERT_EQ(cpu.a, 0x00);
	ASSERT_EQ(cpu.b, 0x00);
	ASSERT_EQ(cpu.c, 0x00);
	ASSERT_EQ(cpu.d, 0x00);
	ASSERT_EQ(cpu.e, 0x00);
	ASSERT_EQ(cpu.h, 0x00);
	ASSERT_EQ(cpu.l, 0x00);
}

TEST_F(CpuTest, FlagValueAtInitAreCorrect)
{
	ASSERT_EQ(cpu.f, 0x00);
}

TEST_F(CpuTest, CheckCpuStateAtInit)
{
	ASSERT_EQ(cpu.tick, 0);
	ASSERT_EQ(cpu.pc, 0);
	ASSERT_EQ(cpu.sp, 0);
	ASSERT_EQ(cpu.halted, false);
	ASSERT_EQ(cpu.interrupts, false);
}

TEST_F(CpuTest, FlagAreUnsetByDefault)
{
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
}

TEST_F(CpuTest, FlagZeroCanBeSet)
{
	assertFlagCanBeSetAndUnset(CPU::CpuFlags::ZERO);
}

TEST_F(CpuTest, FlagSubstractionCanBeSet)
{
	assertFlagCanBeSetAndUnset(CPU::CpuFlags::SUBSTRACTION);
}

TEST_F(CpuTest, FlagCarryCanBeSet)
{
	assertFlagCanBeSetAndUnset(CPU::CpuFlags::CARRY);
}

TEST_F(CpuTest, FlagHalfCarryCanBeSet)
{
	assertFlagCanBeSetAndUnset(CPU::CpuFlags::HALF_CARRY);
}

TEST_F(CpuInstructionTest, InstructionNoop)
{
	performInstructionAndAssertTicksAndFlag(standardInstructions::NOP, 1, 0x00);
	ASSERT_EQ(cpu.pc, 0x01);
}

TEST_F(CpuInstructionTest, InstructionLoad16BitsValueInRegisters)
{
	testLoad16BitsValueInRegisters(standardInstructions::LD_BC_nn, 0x0012, cpu.b, cpu.c);
	testLoad16BitsValueInRegisters(standardInstructions::LD_BC_nn, 0x1200, cpu.b, cpu.c);
	testLoad16BitsValueInRegisters(standardInstructions::LD_BC_nn, 0x1234, cpu.b, cpu.c);

	testLoad16BitsValueInRegisters(standardInstructions::LD_DE_nn, 0x0012, cpu.d, cpu.e);
	testLoad16BitsValueInRegisters(standardInstructions::LD_DE_nn, 0x1200, cpu.d, cpu.e);
	testLoad16BitsValueInRegisters(standardInstructions::LD_DE_nn, 0x1234, cpu.d, cpu.e);

	testLoad16BitsValueInRegisters(standardInstructions::LD_HL_nn, 0x0012, cpu.h, cpu.l);
	testLoad16BitsValueInRegisters(standardInstructions::LD_HL_nn, 0x1200, cpu.h, cpu.l);
	testLoad16BitsValueInRegisters(standardInstructions::LD_HL_nn, 0x1234, cpu.h, cpu.l);

	testLoad16BitsValueInRegisters(standardInstructions::LD_SP_nn, 0x0012, cpu.sp);
	testLoad16BitsValueInRegisters(standardInstructions::LD_SP_nn, 0x1200, cpu.sp);
	testLoad16BitsValueInRegisters(standardInstructions::LD_SP_nn, 0x1234, cpu.sp);
}

TEST_F(CpuInstructionTest, InstructionIncrementRegister)
{
	testIncrement8BitsRegisters(standardInstructions::INC_BC, cpu.b, cpu.c);
	testIncrement8BitsRegisters(standardInstructions::INC_DE, cpu.d, cpu.e);
	testIncrement8BitsRegisters(standardInstructions::INC_HL, cpu.h, cpu.l);
	testIncrement16BitsRegister(standardInstructions::INC_SP, cpu.sp);
	testIncrement8BitsMemoryValue(standardInstructions::INC_HLm, cpu.h, cpu.l);
}

TEST_F(CpuInstructionTest, InstructionDecrementRegister)
{
	testDecrement8BitsRegisters(standardInstructions::DEC_BC, cpu.b, cpu.c);
	testDecrement8BitsRegisters(standardInstructions::DEC_DE, cpu.d, cpu.e);
	testDecrement8BitsRegisters(standardInstructions::DEC_HL, cpu.h, cpu.l);
	testDecrement16BitsRegister(standardInstructions::DEC_SP, cpu.sp);
	testDecrement8BitsMemoryValue(standardInstructions::DEC_HLm, cpu.h, cpu.l);
}

TEST_F(CpuInstructionTest, InstructionRotateLeftCircular)
{
	testRotateLeftCircular({standardInstructions::RLC_A}, cpu.a, 1, false);
	testRotateLeftCircular({standardInstructions::EXT_OPS, extendedInstructions::RLC_A}, cpu.a, 2);
	testRotateLeftCircular({standardInstructions::EXT_OPS, extendedInstructions::RLC_B}, cpu.b, 2);
	testRotateLeftCircular({standardInstructions::EXT_OPS, extendedInstructions::RLC_C}, cpu.c, 2);
	testRotateLeftCircular({standardInstructions::EXT_OPS, extendedInstructions::RLC_D}, cpu.d, 2);
	testRotateLeftCircular({standardInstructions::EXT_OPS, extendedInstructions::RLC_E}, cpu.e, 2);
	testRotateLeftCircular({standardInstructions::EXT_OPS, extendedInstructions::RLC_H}, cpu.h, 2);
	testRotateLeftCircular({standardInstructions::EXT_OPS, extendedInstructions::RLC_L}, cpu.l, 2);
}

TEST_F(CpuInstructionTest, InstructionRotateLeftCircularFromMemory)
{
	testRotateLeftCircularFromMemory({standardInstructions::EXT_OPS, extendedInstructions::RLC_HLm}, cpu.h, cpu.l, 4);
}

TEST_F(CpuInstructionTest, InstructionRotateLeft)
{
    testRotateLeft({standardInstructions::RL_A}, cpu.a, 1, false);
	testRotateLeft({standardInstructions::EXT_OPS, extendedInstructions::RL_A}, cpu.a, 2);
	testRotateLeft({standardInstructions::EXT_OPS, extendedInstructions::RL_B}, cpu.b, 2);
	testRotateLeft({standardInstructions::EXT_OPS, extendedInstructions::RL_C}, cpu.c, 2);
	testRotateLeft({standardInstructions::EXT_OPS, extendedInstructions::RL_D}, cpu.d, 2);
	testRotateLeft({standardInstructions::EXT_OPS, extendedInstructions::RL_E}, cpu.e, 2);
	testRotateLeft({standardInstructions::EXT_OPS, extendedInstructions::RL_H}, cpu.h, 2);
	testRotateLeft({standardInstructions::EXT_OPS, extendedInstructions::RL_L}, cpu.l, 2);
}

TEST_F(CpuInstructionTest, InstructionRotateLeftFromMemory)
{
	testRotateLeftFromMemory({standardInstructions::EXT_OPS, extendedInstructions::RL_HLm}, cpu.h, cpu.l, 4);
}

TEST_F(CpuInstructionTest, InstructionLoad16BitsRegisterAtImmediateAddr)
{
	uint16_t expectedValue = 0x559A;
	cpu.sp = expectedValue;
	cpu.pc = 0x00;
	uint16_t addr = 0x5C8;
	mmu.write(cpu.pc, standardInstructions::LD_nnm_SP);
	mmu.writeWord(cpu.pc + 1, addr);
	int ticks = cpu.fetchDecodeAndExecute();
	ASSERT_EQ(ticks, 5);
	ASSERT_EQ(cpu.getFlag(), 0x00);
	uint16_t value = mmu.readWord(addr);
	ASSERT_EQ(value, expectedValue);
	ASSERT_EQ(cpu.pc, 3);
}

TEST_F(CpuInstructionTest, InstructionAddTwo8BitsRegisterToTwo8BitsRegister)
{
	std::vector<uint16_t> startValues = {0x0000, 0x00FF, 0xFFFF, 0x0FFF, 0xF000};
	std::vector<uint16_t> addValues = {0x0012, 0x0012, 0x0050, 0x0012, 0xF000};
    std::vector<int> expectedFlags = {0, 0, CPU::CARRY | CPU::HALF_CARRY, CPU::HALF_CARRY, CPU::CARRY};
    addTwo8BitsRegisterToTwo8BitsRegister(standardInstructions::ADD_HL_BC, cpu.h, cpu.l, cpu.b, cpu.c, startValues,
	                                      addValues, expectedFlags);
	addTwo8BitsRegisterToTwo8BitsRegister(standardInstructions::ADD_HL_DE, cpu.h, cpu.l, cpu.d, cpu.e, startValues,
	                                      addValues, expectedFlags);
}

TEST_F(CpuInstructionTest, InstructionAdd16BitsRegisterToTwo8BitsRegister)
{
    std::vector<uint16_t> startValues = {0x0000, 0x00FF, 0xFFFF, 0x0FFF, 0xF000};
    std::vector<uint16_t> addValues = {0x0012, 0x0012, 0x0050, 0x0012, 0xF000};
    std::vector<int> expectedFlags = {0, 0, CPU::CARRY | CPU::HALF_CARRY, CPU::HALF_CARRY, CPU::CARRY};
	add16BitsRegisterToTwo8BitsRegister(standardInstructions::ADD_HL_SP, cpu.h, cpu.l, cpu.sp, startValues, addValues, expectedFlags);
}

TEST_F(CpuInstructionTest, InstructionAdd8BitsRegisterToSameRegisters)
{
	std::vector<uint16_t> startValues = {0x0012, 0x00FF, 0x0FFF, 0xF000};
	std::vector<uint16_t> addValues = {0x0012, 0x00FF, 0x0FFF, 0xF000};
    std::vector<int> expectedFlags = {0, 0, CPU::HALF_CARRY, CPU::CARRY};
    addTwo8BitsRegisterToTwo8BitsRegister(standardInstructions::ADD_HL_HL, cpu.h, cpu.l, cpu.h, cpu.l, startValues,
	                                      addValues, expectedFlags);
}

TEST_F(CpuInstructionTest, InstructionRotateRightCircular)
{
	testRotateRightCircular({standardInstructions::RRC_A}, cpu.a, 1, false);
	testRotateRightCircular({standardInstructions::EXT_OPS, extendedInstructions::RRC_A}, cpu.a, 2);
	testRotateRightCircular({standardInstructions::EXT_OPS, extendedInstructions::RRC_B}, cpu.b, 2);
	testRotateRightCircular({standardInstructions::EXT_OPS, extendedInstructions::RRC_C}, cpu.c, 2);
	testRotateRightCircular({standardInstructions::EXT_OPS, extendedInstructions::RRC_D}, cpu.d, 2);
	testRotateRightCircular({standardInstructions::EXT_OPS, extendedInstructions::RRC_E}, cpu.e, 2);
	testRotateRightCircular({standardInstructions::EXT_OPS, extendedInstructions::RRC_H}, cpu.h, 2);
	testRotateRightCircular({standardInstructions::EXT_OPS, extendedInstructions::RRC_L}, cpu.l, 2);
}

TEST_F(CpuInstructionTest, InstructionRotateRightCircularFromMemory)
{
	testRotateRightCircularFromMemory({standardInstructions::EXT_OPS, extendedInstructions::RRC_HLm}, cpu.h, cpu.l, 4);
}

TEST_F(CpuInstructionTest, InstructionRotateRight)
{
    testRotateRight({standardInstructions::RR_A}, cpu.a, 1, false);
	testRotateRight({standardInstructions::EXT_OPS, extendedInstructions::RR_A}, cpu.a, 2);
	testRotateRight({standardInstructions::EXT_OPS, extendedInstructions::RR_B}, cpu.b, 2);
	testRotateRight({standardInstructions::EXT_OPS, extendedInstructions::RR_C}, cpu.c, 2);
	testRotateRight({standardInstructions::EXT_OPS, extendedInstructions::RR_D}, cpu.d, 2);
	testRotateRight({standardInstructions::EXT_OPS, extendedInstructions::RR_E}, cpu.e, 2);
	testRotateRight({standardInstructions::EXT_OPS, extendedInstructions::RR_H}, cpu.h, 2);
	testRotateRight({standardInstructions::EXT_OPS, extendedInstructions::RR_L}, cpu.l, 2);
}

TEST_F(CpuInstructionTest, InstructionRotateRightFromMemory)
{
	testRotateRightFromMemory({standardInstructions::EXT_OPS, extendedInstructions::RR_HLm}, cpu.h, cpu.l, 4);
}

TEST_F(CpuInstructionTest, InstructionLoad16BitsImmediateValueInRegister)
{
	cpu.pc = 0x00;
	uint16_t value = 0x1234;
	mmu.write(cpu.pc, standardInstructions::LD_SP_nn);
	mmu.writeWord(cpu.pc + 1, value);
	int ticks = cpu.fetchDecodeAndExecute();
	ASSERT_EQ(ticks, 3);
	ASSERT_EQ(cpu.getFlag(), 0x00);
	ASSERT_EQ(cpu.pc, 3);
	ASSERT_EQ(cpu.sp, value);
}

TEST_F(CpuInstructionTest, InstructionAdd8BitsRegisterTogether)
{
	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_B, cpu.a, cpu.b);
	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_C, cpu.a, cpu.c);
	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_D, cpu.a, cpu.d);
	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_E, cpu.a, cpu.e);
	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_H, cpu.a, cpu.h);
	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_L, cpu.a, cpu.l);

	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_A, cpu.a, 0x00, cpu.a, 0x00, 0x00, false, false,
	                                    false);
	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_A, cpu.a, 0x0F, cpu.a, 0x0F, 0x1E, false, true,
	                                    false);
	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_A, cpu.a, 0xFF, cpu.a, 0xFF, 0xFE, false, true,
	                                    true);
    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_A, cpu.a, 0xF0, cpu.a, 0xF0, 0xE0, false, false,
                                        true);
}

TEST_F(CpuInstructionTest, InstructionAddValueFromMemoryToRegister)
{
	cpu.h = 0x12;
	cpu.l = 0x23;
	testAddValueFromMemoryToRegister(standardInstructions::ADD_A_HLm, cpu.a, 0x00, cpu.h, cpu.l, 0x00, 0x00, false,
	                                 false, false);
	testAddValueFromMemoryToRegister(standardInstructions::ADD_A_HLm, cpu.a, 0x00, cpu.h, cpu.l, 0x05, 0x05, false,
	                                 false, false);
	testAddValueFromMemoryToRegister(standardInstructions::ADD_A_HLm, cpu.a, 0x0F, cpu.h, cpu.l, 0x05, 0x14, false,
	                                 true, false);
	testAddValueFromMemoryToRegister(standardInstructions::ADD_A_HLm, cpu.a, 0xFF, cpu.h, cpu.l, 0x05, 0x04, false,
	                                 true, true);
	testAddValueFromMemoryToRegister(standardInstructions::ADD_A_HLm, cpu.a, 0xFF, cpu.h, cpu.l, 0x01, 0x00, false,
	                                 true, true);
    testAddValueFromMemoryToRegister(standardInstructions::ADD_A_HLm, cpu.a, 0xF0, cpu.h, cpu.l, 0xF0, 0xE0, false,
                                     false, true);
}

TEST_F(CpuInstructionTest, InstructionAddImmediateValueToRegister)
{
	testAddImmediateValueToRegister(standardInstructions::ADD_A_n, cpu.a, 0x00, 0x00, 0x00, false, false, false);
	testAddImmediateValueToRegister(standardInstructions::ADD_A_n, cpu.a, 0x00, 0x05, 0x05, false, false, false);
	testAddImmediateValueToRegister(standardInstructions::ADD_A_n, cpu.a, 0x0F, 0x05, 0x14, false, true, false);
	testAddImmediateValueToRegister(standardInstructions::ADD_A_n, cpu.a, 0xFF, 0x05, 0x04, false, true, true);
	testAddImmediateValueToRegister(standardInstructions::ADD_A_n, cpu.a, 0xFF, 0x01, 0x00, false, true, true);
    testAddImmediateValueToRegister(standardInstructions::ADD_A_n, cpu.a, 0xF0, 0xF0, 0xE0, false, false, true);
}

TEST_F(CpuInstructionTest, InstructionAdd8BitsRegisterAndCarryToAnother)
{
	testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_B, cpu.a, cpu.b);
	testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_C, cpu.a, cpu.c);
	testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_D, cpu.a, cpu.d);
	testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_E, cpu.a, cpu.e);
	testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_H, cpu.a, cpu.h);
	testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_L, cpu.a, cpu.l);

	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.a, 0x00, cpu.a, 0x00, 0x00, false, false,
	                                    false);
	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.a, 0x00, cpu.a, 0x00, 0x01, true, false,
	                                    false);

	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.a, 0x0F, cpu.a, 0x0F, 0x1E, false, true,
	                                    false);
	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.a, 0x0F, cpu.a, 0x0F, 0x1F, true, true,
	                                    false);

	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.a, 0xFF, cpu.a, 0xFF, 0xFE, false, true,
	                                    true);
	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.a, 0xFF, cpu.a, 0xFF, 0xFF, true, true,
	                                    true);

    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.a, 0xF0, cpu.a, 0xF0, 0xE0, false, false,
                                        true);
    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.a, 0xF0, cpu.a, 0xF0, 0xE1, true, false,
                                        true);
}

TEST_F(CpuInstructionTest, InstructionAddValueFromMemoryAndCarryToRegister)
{
	cpu.h = 0x12;
	cpu.l = 0x23;
	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0x00, cpu.h, cpu.l, 0x00, 0x00, false,
	                                 false, false);
	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0x00, cpu.h, cpu.l, 0x00, 0x01, true,
	                                 false, false);
	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0x00, cpu.h, cpu.l, 0x05, 0x05, false,
	                                 false, false);
	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0x0F, cpu.h, cpu.l, 0x05, 0x14, false,
	                                 true, false);
	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0x0F, cpu.h, cpu.l, 0x05, 0x15, true, true,
	                                 false);

	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0xFF, cpu.h, cpu.l, 0x05, 0x04, false,
	                                 true, true);
	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0xFF, cpu.h, cpu.l, 0x05, 0x05, true,
	                                 true, true);

	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0xFF, cpu.h, cpu.l, 0x01, 0x00, false,
	                                 true, true);
	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0xFF, cpu.h, cpu.l, 0x00, 0x00, true,
	                                 true, true);

    testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0xF0, cpu.h, cpu.l, 0xF0, 0xE0, false,
                                     false, true);
    testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0xF0, cpu.h, cpu.l, 0xF0, 0xE1, true,
                                     false, true);
}

TEST_F(CpuInstructionTest, InstructionAddImmediateValueAndCarryToRegister)
{
	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0x00, 0x00, 0x00, false, false, false);
	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0x00, 0x00, 0x01, true, false, false);

	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0x00, 0x05, 0x05, false, false, false);
	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0x00, 0x05, 0x06, true, false, false);

	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0x0F, 0x05, 0x14, false, true, false);
	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0x0F, 0x05, 0x15, true, true, false);

	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0xFF, 0x05, 0x04, false, true, true);
	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0xFF, 0x05, 0x05, true, true, true);

	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0xFF, 0x01, 0x00, false, true, true);
	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0xFF, 0x00, 0x00, true, true, true);

    testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0xF0, 0xF0, 0xE0, false, false, true);
    testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0xF0, 0xEF, 0xE0, true, true, true);
}

TEST_F(CpuInstructionTest, InstructionSub8BitsRegisterTogether)
{
	testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_B, cpu.a, cpu.b);
	testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_C, cpu.a, cpu.c);
	testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_D, cpu.a, cpu.d);
	testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_E, cpu.a, cpu.e);
	testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_H, cpu.a, cpu.h);
	testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_L, cpu.a, cpu.l);

	testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_A, cpu.a, 0x00, cpu.a, 0x00, 0x00, false, false,
	                                    false);
	testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_A, cpu.a, 0xFF, cpu.a, 0xFF, 0x00, false, false,
	                                    false);
	testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_A, cpu.a, 0x10, cpu.a, 0x10, 0x00, false, false,
	                                    false);
}

TEST_F(CpuInstructionTest, InstructionSubValueFromMemoryToRegister)
{
	cpu.h = 0x12;
	cpu.l = 0x23;
	testSubValueFromMemoryToRegister(standardInstructions::SUB_A_HLm, cpu.a, 0x00, cpu.h, cpu.l, 0x00, 0x00, false,
	                                 false, false);
	testSubValueFromMemoryToRegister(standardInstructions::SUB_A_HLm, cpu.a, 0x00, cpu.h, cpu.l, 0x05, 0xFB, false,
	                                 true, true);
	testSubValueFromMemoryToRegister(standardInstructions::SUB_A_HLm, cpu.a, 0x10, cpu.h, cpu.l, 0x05, 0x0B, false,
	                                 true, false);
	testSubValueFromMemoryToRegister(standardInstructions::SUB_A_HLm, cpu.a, 0xFF, cpu.h, cpu.l, 0x05, 0xFA, false,
	                                 false, false);
	testSubValueFromMemoryToRegister(standardInstructions::SUB_A_HLm, cpu.a, 0x00, cpu.h, cpu.l, 0x01, 0xFF, false,
	                                 true, true);
}

TEST_F(CpuInstructionTest, InstructionSubImmediateValueToRegister)
{
	testSubImmediateValueToRegister(standardInstructions::SUB_A_n, cpu.a, 0x00, 0x00, 0x00, false, false, false);
	testSubImmediateValueToRegister(standardInstructions::SUB_A_n, cpu.a, 0x00, 0x05, 0xFB, false, true, true);
	testSubImmediateValueToRegister(standardInstructions::SUB_A_n, cpu.a, 0x10, 0x05, 0x0B, false, true, false);
	testSubImmediateValueToRegister(standardInstructions::SUB_A_n, cpu.a, 0xFF, 0x05, 0xFA, false, false, false);
	testSubImmediateValueToRegister(standardInstructions::SUB_A_n, cpu.a, 0x00, 0x01, 0xFF, false, true, true);
}

TEST_F(CpuInstructionTest, InstructionSub8BitsRegisterAndCarryToAnother)
{
	testSub8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::SBC_A_B, cpu.a, cpu.b);
	testSub8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::SBC_A_C, cpu.a, cpu.c);
	testSub8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::SBC_A_D, cpu.a, cpu.d);
	testSub8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::SBC_A_E, cpu.a, cpu.e);
	testSub8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::SBC_A_H, cpu.a, cpu.h);
	testSub8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::SBC_A_L, cpu.a, cpu.l);

	testSub8BitsRegisterTo8BitsRegister(standardInstructions::SBC_A_A, cpu.a, 0x00, cpu.a, 0x00, 0x00, false, false,
	                                    false);
	testSub8BitsRegisterTo8BitsRegister(standardInstructions::SBC_A_A, cpu.a, 0x00, cpu.a, 0x00, 0xFF, true, true,
	                                    true);

	testSub8BitsRegisterTo8BitsRegister(standardInstructions::SBC_A_A, cpu.a, 0x10, cpu.a, 0x10, 0x00, false, false,
	                                    false);
	testSub8BitsRegisterTo8BitsRegister(standardInstructions::SBC_A_A, cpu.a, 0x10, cpu.a, 0x10, 0xFF, true, true,
	                                    true);
}

TEST_F(CpuInstructionTest, InstructionAndBetweenRegisterAndAccumulator)
{
	testAndOperationWithRegister(standardInstructions::AND_B, cpu.b);
	testAndOperationWithRegister(standardInstructions::AND_C, cpu.c);
	testAndOperationWithRegister(standardInstructions::AND_D, cpu.d);
	testAndOperationWithRegister(standardInstructions::AND_E, cpu.e);
	testAndOperationWithRegister(standardInstructions::AND_H, cpu.h);
	testAndOperationWithRegister(standardInstructions::AND_L, cpu.l);

	testAndOperationWithRegister(standardInstructions::AND_A, 0x00, cpu.a, 0x00, 0x00);
	testAndOperationWithRegister(standardInstructions::AND_A, 0x0F, cpu.a, 0x0F, 0x0F);
	testAndOperationWithRegister(standardInstructions::AND_A, 0xF0, cpu.a, 0xF0, 0xF0);
	testAndOperationWithRegister(standardInstructions::AND_A, 0x01, cpu.a, 0x01, 0x01);
	testAndOperationWithRegister(standardInstructions::AND_A, 0x10, cpu.a, 0x10, 0x10);
}

TEST_F(CpuInstructionTest, InstructionAndBetweenRegisterAndImmediateValue)
{
	testAndOperationWithImmediateValue(standardInstructions::AND_n, 0x00, 0x00, 0x00);
	testAndOperationWithImmediateValue(standardInstructions::AND_n, 0x0F, 0x00, 0x00);
	testAndOperationWithImmediateValue(standardInstructions::AND_n, 0x00, 0x0F, 0x00);
	testAndOperationWithImmediateValue(standardInstructions::AND_n, 0x0F, 0x0F, 0x0F);
	testAndOperationWithImmediateValue(standardInstructions::AND_n, 0xF0, 0xF0, 0xF0);
	testAndOperationWithImmediateValue(standardInstructions::AND_n, 0x01, 0x01, 0x01);
	testAndOperationWithImmediateValue(standardInstructions::AND_n, 0x10, 0x10, 0x10);
}

TEST_F(CpuInstructionTest, InstructionAndBetweenRegisterAndMemory)
{
	cpu.h = 0x12;
	cpu.l = 0x23;
	testAndOperationWithMemory(standardInstructions::AND_HLm, 0x00, cpu.h, cpu.l, 0x00, 0x00);
	testAndOperationWithMemory(standardInstructions::AND_HLm, 0x0F, cpu.h, cpu.l, 0x00, 0x00);
	testAndOperationWithMemory(standardInstructions::AND_HLm, 0x00, cpu.h, cpu.l, 0x0F, 0x00);
	testAndOperationWithMemory(standardInstructions::AND_HLm, 0x0F, cpu.h, cpu.l, 0x0F, 0x0F);
	testAndOperationWithMemory(standardInstructions::AND_HLm, 0xF0, cpu.h, cpu.l, 0xF0, 0xF0);
	testAndOperationWithMemory(standardInstructions::AND_HLm, 0x01, cpu.h, cpu.l, 0x01, 0x01);
	testAndOperationWithMemory(standardInstructions::AND_HLm, 0x10, cpu.h, cpu.l, 0x10, 0x10);
}

TEST_F(CpuInstructionTest, InstructionXorBetweenRegisterAndAccumulator)
{
	testXorOperationWithRegister(standardInstructions::XOR_B, cpu.b);
	testXorOperationWithRegister(standardInstructions::XOR_C, cpu.c);
	testXorOperationWithRegister(standardInstructions::XOR_D, cpu.d);
	testXorOperationWithRegister(standardInstructions::XOR_E, cpu.e);
	testXorOperationWithRegister(standardInstructions::XOR_H, cpu.h);
	testXorOperationWithRegister(standardInstructions::XOR_L, cpu.l);

	testXorOperationWithRegister(standardInstructions::XOR_A, 0x00, cpu.a, 0x00, 0x00);
	testXorOperationWithRegister(standardInstructions::XOR_A, 0x0F, cpu.a, 0x0F, 0x00);
	testXorOperationWithRegister(standardInstructions::XOR_A, 0xF0, cpu.a, 0xF0, 0x00);
	testXorOperationWithRegister(standardInstructions::XOR_A, 0x01, cpu.a, 0x01, 0x00);
	testXorOperationWithRegister(standardInstructions::XOR_A, 0x10, cpu.a, 0x10, 0x00);
	testXorOperationWithRegister(standardInstructions::XOR_A, 0xFF, cpu.a, 0xFF, 0x00);
}

TEST_F(CpuInstructionTest, InstructionXorBetweenRegisterAndImmediateValue)
{
	testXorOperationWithImmediateValue(standardInstructions::XOR_n, 0x00, 0x00, 0x00);
	testXorOperationWithImmediateValue(standardInstructions::XOR_n, 0x0F, 0x00, 0x0F);
	testXorOperationWithImmediateValue(standardInstructions::XOR_n, 0x00, 0x0F, 0x0F);
	testXorOperationWithImmediateValue(standardInstructions::XOR_n, 0x0F, 0x0F, 0x00);
	testXorOperationWithImmediateValue(standardInstructions::XOR_n, 0xF0, 0xF0, 0x00);
	testXorOperationWithImmediateValue(standardInstructions::XOR_n, 0x01, 0x01, 0x00);
	testXorOperationWithImmediateValue(standardInstructions::XOR_n, 0x10, 0x10, 0x00);
	testXorOperationWithImmediateValue(standardInstructions::XOR_n, 0x01, 0x00, 0x01);
	testXorOperationWithImmediateValue(standardInstructions::XOR_n, 0x00, 0x01, 0x01);
	testXorOperationWithImmediateValue(standardInstructions::XOR_n, 0xFF, 0xFF, 0x00);
}

TEST_F(CpuInstructionTest, InstructionXorBetweenRegisterAndMemory)
{
	cpu.h = 0x12;
	cpu.l = 0x23;
	testXorOperationWithMemory(standardInstructions::XOR_HLm, 0x00, cpu.h, cpu.l, 0x00, 0x00);
	testXorOperationWithMemory(standardInstructions::XOR_HLm, 0x0F, cpu.h, cpu.l, 0x00, 0x0F);
	testXorOperationWithMemory(standardInstructions::XOR_HLm, 0x00, cpu.h, cpu.l, 0x0F, 0x0F);
	testXorOperationWithMemory(standardInstructions::XOR_HLm, 0x0F, cpu.h, cpu.l, 0x0F, 0x00);
	testXorOperationWithMemory(standardInstructions::XOR_HLm, 0xF0, cpu.h, cpu.l, 0xF0, 0x00);
	testXorOperationWithMemory(standardInstructions::XOR_HLm, 0x01, cpu.h, cpu.l, 0x01, 0x00);
	testXorOperationWithMemory(standardInstructions::XOR_HLm, 0x10, cpu.h, cpu.l, 0x10, 0x00);
	testXorOperationWithMemory(standardInstructions::XOR_HLm, 0x01, cpu.h, cpu.l, 0x00, 0x01);
	testXorOperationWithMemory(standardInstructions::XOR_HLm, 0x00, cpu.h, cpu.l, 0x01, 0x01);
	testXorOperationWithMemory(standardInstructions::XOR_HLm, 0xFF, cpu.h, cpu.l, 0xFF, 0x00);
}

TEST_F(CpuInstructionTest, InstructionOrBetweenRegisterAndAccumulator)
{
	testOrOperationWithRegister(standardInstructions::OR_B, cpu.b);
	testOrOperationWithRegister(standardInstructions::OR_C, cpu.c);
	testOrOperationWithRegister(standardInstructions::OR_D, cpu.d);
	testOrOperationWithRegister(standardInstructions::OR_E, cpu.e);
	testOrOperationWithRegister(standardInstructions::OR_H, cpu.h);
	testOrOperationWithRegister(standardInstructions::OR_L, cpu.l);

	testOrOperationWithRegister(standardInstructions::OR_A, 0x00, cpu.a, 0x00, 0x00);
	testOrOperationWithRegister(standardInstructions::OR_A, 0x0F, cpu.a, 0x0F, 0x0F);
	testOrOperationWithRegister(standardInstructions::OR_A, 0xF0, cpu.a, 0xF0, 0xF0);
	testOrOperationWithRegister(standardInstructions::OR_A, 0x01, cpu.a, 0x01, 0x01);
	testOrOperationWithRegister(standardInstructions::OR_A, 0x10, cpu.a, 0x10, 0x10);
	testOrOperationWithRegister(standardInstructions::OR_A, 0xFF, cpu.a, 0xFF, 0xFF);
}

TEST_F(CpuInstructionTest, InstructionOrBetweenRegisterAndImmediateValue)
{
	testOrOperationWithImmediateValue(standardInstructions::OR_n, 0x00, 0x00, 0x00);
	testOrOperationWithImmediateValue(standardInstructions::OR_n, 0x0F, 0x00, 0x0F);
	testOrOperationWithImmediateValue(standardInstructions::OR_n, 0x00, 0x0F, 0x0F);
	testOrOperationWithImmediateValue(standardInstructions::OR_n, 0x0F, 0x0F, 0x0F);
	testOrOperationWithImmediateValue(standardInstructions::OR_n, 0xF0, 0xF0, 0xF0);
	testOrOperationWithImmediateValue(standardInstructions::OR_n, 0x01, 0x01, 0x01);
	testOrOperationWithImmediateValue(standardInstructions::OR_n, 0x10, 0x10, 0x10);
	testOrOperationWithImmediateValue(standardInstructions::OR_n, 0x01, 0x00, 0x01);
	testOrOperationWithImmediateValue(standardInstructions::OR_n, 0x00, 0x01, 0x01);
	testOrOperationWithImmediateValue(standardInstructions::OR_n, 0xFF, 0xFF, 0xFF);
}

TEST_F(CpuInstructionTest, InstructionOrBetweenRegisterAndMemory)
{
	cpu.h = 0x12;
	cpu.l = 0x23;
	testOrOperationWithMemory(standardInstructions::OR_HLm, 0x00, cpu.h, cpu.l, 0x00, 0x00);
	testOrOperationWithMemory(standardInstructions::OR_HLm, 0x0F, cpu.h, cpu.l, 0x00, 0x0F);
	testOrOperationWithMemory(standardInstructions::OR_HLm, 0x00, cpu.h, cpu.l, 0x0F, 0x0F);
	testOrOperationWithMemory(standardInstructions::OR_HLm, 0x0F, cpu.h, cpu.l, 0x0F, 0x0F);
	testOrOperationWithMemory(standardInstructions::OR_HLm, 0xF0, cpu.h, cpu.l, 0xF0, 0xF0);
	testOrOperationWithMemory(standardInstructions::OR_HLm, 0x01, cpu.h, cpu.l, 0x01, 0x01);
	testOrOperationWithMemory(standardInstructions::OR_HLm, 0x10, cpu.h, cpu.l, 0x10, 0x10);
	testOrOperationWithMemory(standardInstructions::OR_HLm, 0x01, cpu.h, cpu.l, 0x00, 0x01);
	testOrOperationWithMemory(standardInstructions::OR_HLm, 0x00, cpu.h, cpu.l, 0x01, 0x01);
	testOrOperationWithMemory(standardInstructions::OR_HLm, 0xFF, cpu.h, cpu.l, 0xFF, 0xFF);
}

TEST_F(CpuInstructionTest, InstructionCompareAccumulatorAndRegister)
{
	testCompareOperationWithRegister(standardInstructions::CP_B, cpu.b);
	testCompareOperationWithRegister(standardInstructions::CP_C, cpu.c);
	testCompareOperationWithRegister(standardInstructions::CP_D, cpu.d);
	testCompareOperationWithRegister(standardInstructions::CP_E, cpu.e);
	testCompareOperationWithRegister(standardInstructions::CP_H, cpu.h);
	testCompareOperationWithRegister(standardInstructions::CP_L, cpu.l);

	testCompareOperationWithRegister(standardInstructions::CP_A, 0x00, cpu.a, 0x00, CPU::ZERO);
	testCompareOperationWithRegister(standardInstructions::CP_A, 0xF0, cpu.a, 0xF0, CPU::ZERO);
	testCompareOperationWithRegister(standardInstructions::CP_A, 0x0F, cpu.a, 0x0F, CPU::ZERO);
	testCompareOperationWithRegister(standardInstructions::CP_A, 0xFF, cpu.a, 0xFF, CPU::ZERO);
}

TEST_F(CpuInstructionTest, InstructionCompareWithImmediateValue)
{
    testCompareOperationWithImmediateValue(standardInstructions::CP_n, 0x00, 0x00, CPU::ZERO);
    testCompareOperationWithImmediateValue(standardInstructions::CP_n, 0x0F, 0x00, 0);
    testCompareOperationWithImmediateValue(standardInstructions::CP_n, 0x00, 0x0F, CPU::CARRY | CPU::HALF_CARRY);
    testCompareOperationWithImmediateValue(standardInstructions::CP_n, 0xFF, 0xFF, CPU::ZERO);
    testCompareOperationWithImmediateValue(standardInstructions::CP_n, 0x09, 0x08, 0);
    testCompareOperationWithImmediateValue(standardInstructions::CP_n, 0x08, 0x09, CPU::CARRY | CPU::HALF_CARRY);

}

TEST_F(CpuInstructionTest, InstructionCompareWithMemory)
{
    cpu.h = 0x12;
    cpu.l = 0x23;
    testCompareOperationWithMemory(standardInstructions::CP_HLm, 0x00, cpu.h, cpu.l, 0x00, CPU::ZERO);
    testCompareOperationWithMemory(standardInstructions::CP_HLm, 0x0F, cpu.h, cpu.l, 0x00, 0);
    testCompareOperationWithMemory(standardInstructions::CP_HLm, 0x00, cpu.h, cpu.l, 0x0F, CPU::CARRY | CPU::HALF_CARRY);
    testCompareOperationWithMemory(standardInstructions::CP_HLm, 0xFF, cpu.h, cpu.l, 0xFF, CPU::ZERO);
    testCompareOperationWithMemory(standardInstructions::CP_HLm, 0x09, cpu.h, cpu.l, 0x08, 0);
    testCompareOperationWithMemory(standardInstructions::CP_HLm, 0x08, cpu.h, cpu.l, 0x09, CPU::CARRY | CPU::HALF_CARRY);
}

TEST_F(CpuInstructionTest, InstructionReturn)
{
	uint16_t callerAddr = 0x1234;
    mmu.writeWord(cpu.pc, callerAddr);
	cpu.pc += 2;
	mmu.write(cpu.pc, standardInstructions::RET);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 4);
    ASSERT_EQ(cpu.getFlag(), 0x00);
	ASSERT_EQ(cpu.pc, callerAddr);
}

TEST_F(CpuInstructionTest, InstructionReturnConditionalCarryTrue)
{
	cpu.setFlag(CPU::CARRY);
    testReturnConditional(standardInstructions::RET_C, true);
	ASSERT_EQ(cpu.getFlag(), CPU::CARRY);
}

TEST_F(CpuInstructionTest, InstructionReturnConditionalCarryFalse)
{
    testReturnConditional(standardInstructions::RET_C, false);
    ASSERT_EQ(cpu.getFlag(), 0x00);
}

TEST_F(CpuInstructionTest, InstructionReturnConditionalZeroTrue)
{
    cpu.setFlag(CPU::ZERO);
    testReturnConditional(standardInstructions::RET_Z, true);
    ASSERT_EQ(cpu.getFlag(), CPU::ZERO);
}

TEST_F(CpuInstructionTest, InstructionReturnConditionalZeroFalse)
{
    testReturnConditional(standardInstructions::RET_Z, false);
    ASSERT_EQ(cpu.getFlag(), 0x00);
}

TEST_F(CpuInstructionTest, InstructionReturnConditionalNotCarryTrue)
{
    testReturnConditional(standardInstructions::RET_NC, true);
    ASSERT_EQ(cpu.getFlag(), 0x00);
}

TEST_F(CpuInstructionTest, InstructionReturnConditionalNotCarryFalse)
{
	cpu.setFlag(CPU::CARRY);
    testReturnConditional(standardInstructions::RET_NC, false);
    ASSERT_EQ(cpu.getFlag(), CPU::CARRY);
}

TEST_F(CpuInstructionTest, InstructionReturnConditionalNotZeroTrue)
{
    testReturnConditional(standardInstructions::RET_NZ, true);
    ASSERT_EQ(cpu.getFlag(), 0x00);
}

TEST_F(CpuInstructionTest, InstructionReturnConditionalNotZeroFalse)
{
    cpu.setFlag(CPU::ZERO);
    testReturnConditional(standardInstructions::RET_NZ, false);
    ASSERT_EQ(cpu.getFlag(), CPU::ZERO);
}

TEST_F(CpuInstructionTest, InstructionReturnAfterInterrupt)
{
	cpu.interrupts = true;
    uint16_t callerAddr = 0x1234;
    mmu.writeWord(cpu.pc, callerAddr);
    cpu.pc += 2;
    mmu.write(cpu.pc, standardInstructions::RETI);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 4);
    ASSERT_EQ(cpu.getFlag(), 0x00);
    ASSERT_EQ(cpu.pc, callerAddr);
	ASSERT_FALSE(cpu.interrupts);
}

TEST_F(CpuTest, GetSetStackPointerShouldChangeValue)
{
	uint16_t value = 0xCAFE;
	ASSERT_EQ(cpu.getStackPointer(), 0x00);
	cpu.setStackPointer(value);
	ASSERT_EQ(cpu.getStackPointer(), value);
}

TEST_F(CpuTest, GetSetProgramCounterShouldChangeValue)
{
    uint16_t value = 0xCAFE;
    ASSERT_EQ(cpu.getProgramCounter(), 0x00);
    cpu.setProgramCounter(value);
    ASSERT_EQ(cpu.getProgramCounter(), value);
}

TEST_F(CpuTest, GetSetRegisterAShouldChangeValue)
{
    byte value = 0x12;
    ASSERT_EQ(cpu.getRegisterA(), 0x00);
    cpu.setRegisterA(value);
    ASSERT_EQ(cpu.getRegisterA(), value);
}

TEST_F(CpuTest, GetSetRegisterBShouldChangeValue)
{
    byte value = 0x12;
    ASSERT_EQ(cpu.getRegisterB(), 0x00);
    cpu.setRegisterB(value);
    ASSERT_EQ(cpu.getRegisterB(), value);
}

TEST_F(CpuTest, GetSetRegisterCShouldChangeValue)
{
    byte value = 0x12;
    ASSERT_EQ(cpu.getRegisterC(), 0x00);
    cpu.setRegisterC(value);
    ASSERT_EQ(cpu.getRegisterC(), value);
}

TEST_F(CpuTest, GetSetRegisterDShouldChangeValue)
{
    byte value = 0x12;
    ASSERT_EQ(cpu.getRegisterD(), 0x00);
    cpu.setRegisterD(value);
    ASSERT_EQ(cpu.getRegisterD(), value);
}

TEST_F(CpuTest, GetSetRegisterEShouldChangeValue)
{
    byte value = 0x12;
    ASSERT_EQ(cpu.getRegisterE(), 0x00);
    cpu.setRegisterE(value);
    ASSERT_EQ(cpu.getRegisterE(), value);
}

TEST_F(CpuTest, GetSetRegisterHShouldChangeValue)
{
    byte value = 0x12;
    ASSERT_EQ(cpu.getRegisterH(), 0x00);
    cpu.setRegisterH(value);
    ASSERT_EQ(cpu.getRegisterH(), value);
}

TEST_F(CpuTest, GetSetRegisterLShouldChangeValue)
{
    byte value = 0x12;
    ASSERT_EQ(cpu.getRegisterL(), 0x00);
    cpu.setRegisterL(value);
    ASSERT_EQ(cpu.getRegisterL(), value);
}