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

	void testLoad8BitsValueInMemoryAddr(byte instruction, byte& msb, byte& lsb, byte& value)
	{
		cpu.pc = 0x00;
		value = 42;
		msb = 0x05;
		lsb = 0x60;
		mmu.write(cpu.pc, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_EQ(cpu.getFlag(), 0x00);
		ASSERT_EQ(mmu.read(createAddrFromHighAndLowBytes(msb, lsb)), value);
		ASSERT_EQ(cpu.pc, 0x01);
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

	void testIncrement8BitRegister(byte instruction, const byte& reg)
	{
		cpu.pc = 0x00;
		uint16_t expected_pc = 1;
		for (int i = 1; i <= UINT8_MAX + 1; ++i)
		{
			byte expected_value = i;
			mmu.write(cpu.pc, instruction);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, 1);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), expected_value == 0x10);
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expected_value == 0);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
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

	void testDecrement8BitRegister(byte instruction, const byte& reg)
	{
		cpu.pc = 0x00;
		uint16_t expected_pc = 1;
		for (int i = UINT8_MAX; i >= 0; --i)
		{
			byte expected_value = i;
			mmu.write(cpu.pc, instruction);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, 1);
			ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), expected_value == 0x08);
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expected_value == 0);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
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
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), expected_value == 0x08);
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expected_value == 0);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
			ASSERT_EQ(mmu.read(createAddrFromHighAndLowBytes(msbRegister, lsbRegister)), expected_value);
			ASSERT_EQ(cpu.pc, expected_pc);
			expected_pc++;
		}
	}

	void testLoadImmediateValue(byte instruction, byte& reg)
	{
		cpu.pc = 0x00;
		uint16_t expected_pc = 0;
		for (int i = 0; i < UINT8_MAX; ++i)
		{
			mmu.write(cpu.pc, instruction);
			mmu.write(cpu.pc + 1, i);
			int ticks = cpu.fetchDecodeAndExecute();
			expected_pc += 2;
			ASSERT_EQ(ticks, 2);
			ASSERT_EQ(cpu.getFlag(), 0x00);
			ASSERT_EQ(cpu.pc, expected_pc);
			ASSERT_EQ(reg, i);
		}
	}

	void testRotateLeftCircular(const std::vector<byte>& instructions, byte& reg, int expectedTicks)
	{
		reg = 0b00000001;
		cpu.pc = 0x00;
		for (int i = 0; i < 9; ++i)
		{
			for (int j = 0; j < instructions.size(); ++j)
			{
				mmu.write(cpu.pc + j, instructions[j]);
			}
			byte registerValue = reg;
			byte expectedValue = (registerValue << 1) | (registerValue >> 7);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, expectedTicks);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expectedValue == 0);
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
			for (int j = 0; j < instructions.size(); ++j)
			{
				mmu.write(cpu.pc + j, instructions[j]);
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

	void testRotateLeft(const std::vector<byte>& instructions, byte& reg, int expectedTicks)
	{
		reg = 0b00000001;
		cpu.pc = 0x00;
		for (int i = 0; i < 9; ++i)
		{
			for (int j = 0; j < instructions.size(); ++j)
			{
				mmu.write(cpu.pc + j, instructions[j]);
			}
			byte registerValue = reg;
			bool isCarryFlagSet = cpu.isFlagSet(CPU::CpuFlags::CARRY);
			byte expectedValue = (registerValue << 1) | isCarryFlagSet;
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, expectedTicks);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expectedValue == 0);
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
			for (int j = 0; j < instructions.size(); ++j)
			{
				mmu.write(cpu.pc + j, instructions[j]);
			}
			byte registerValue = mmu.read(addr);
			bool isCarryFlagSet = cpu.isFlagSet(CPU::CpuFlags::CARRY);
			byte expectedValue = (registerValue << 1) | isCarryFlagSet;
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
	                                           std::vector<uint16_t>& addValues)
	{
		for (int i = 0; i < startValues.size(); ++i)
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
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (expectedValue > 0xFFFF));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), (startValue <= 0xFFF && expectedValue > 0xFFF));
		}
	}

	void add16BitsRegisterToTwo8BitsRegister(byte instruction, byte& resultRegMsb, byte& resultRegLsb,
	                                         uint16_t& valueReg, std::vector<uint16_t>& startValues,
	                                         std::vector<uint16_t>& addValues)
	{
		for (int i = 0; i < startValues.size(); ++i)
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
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (expectedValue > 0xFFFF));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), (startValue <= 0xFFF && expectedValue > 0xFFF));
		}
	}

	void testLoadValueFromMemoryInto8BitsRegister(byte instruction, byte& reg, byte& addrRegMsb, byte& addrRegLsb)
	{
		cpu.pc = 0x00;
		std::vector<uint16_t> addr = {0x0042, 0x0F00, 0x0F42};
		for (int i = 0; i < addr.size(); ++i)
		{
			addrRegMsb = addr[i] >> 8;
			addrRegLsb = addr[i] & 0xFF;
			mmu.write(addr[i], i + 1);
			mmu.write(cpu.pc, instruction);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, 2);
			ASSERT_EQ(cpu.getFlag(), 0x00);
			ASSERT_EQ(reg, i + 1);
			ASSERT_EQ(cpu.pc, i + 1);
		}
	}

	void testRotateRightCircular(const std::vector<byte>& instructions, byte& reg, int expectedTicks)
	{
		reg = 0b10000000;
		cpu.pc = 0x00;
		for (int i = 0; i < 9; ++i)
		{
			for (int j = 0; j < instructions.size(); ++j)
			{
				mmu.write(cpu.pc + j, instructions[j]);
			}
			byte registerValue = reg;
			byte expectedValue = (registerValue << 7) | (registerValue >> 1);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, expectedTicks);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expectedValue == 0);
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
			for (int j = 0; j < instructions.size(); ++j)
			{
				mmu.write(cpu.pc + j, instructions[j]);
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

	void testRotateRight(const std::vector<byte>& instructions, byte& reg, int expectedTicks)
	{
		reg = 0b10000000;
		cpu.pc = 0x00;
		for (int i = 0; i < 9; ++i)
		{
			for (int j = 0; j < instructions.size(); ++j)
			{
				mmu.write(cpu.pc + j, instructions[j]);
			}
			byte registerValue = reg;
			bool isCarryFlagSet = cpu.isFlagSet(CPU::CpuFlags::CARRY);
			byte expectedValue = (isCarryFlagSet << 7) | (registerValue >> 1);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, expectedTicks);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expectedValue == 0);
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
			for (int j = 0; j < instructions.size(); ++j)
			{
				mmu.write(cpu.pc + j, instructions[j]);
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

	void testJumpConditional(byte instruction, bool condition)
	{
		auto flag = cpu.getFlag();
		cpu.pc = 0x00;
		uint16_t addr = 0x1234;
		mmu.write(cpu.pc, instruction);
		mmu.writeWord(cpu.pc + 1, addr);
		int ticks = cpu.fetchDecodeAndExecute();
		if (condition)
		{
			ASSERT_EQ(ticks, 4);
			ASSERT_EQ(cpu.pc, addr);
		}
		else
		{
			ASSERT_EQ(ticks, 3);
			ASSERT_EQ(cpu.pc, 3);
		}
		ASSERT_EQ(cpu.getFlag(), flag);
	}

	void testJumpRelative(int8_t offset)
	{
		uint16_t startAddr = 0x1234;
		cpu.pc = startAddr;
		mmu.write(cpu.pc, standardInstructions::JR_n);
		mmu.write(cpu.pc + 1, offset);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 3);
		ASSERT_EQ(cpu.getFlag(), 0x00);
		ASSERT_EQ(cpu.pc, startAddr + 2 + offset);
	}

	void testJumpRelativeConditional(byte instruction, int8_t offset, bool condition)
	{
		auto flag = cpu.getFlag();
		uint16_t startAddr = 0x1234;
		cpu.pc = startAddr;
		mmu.write(cpu.pc, instruction);
		mmu.writeWord(cpu.pc + 1, offset);
		int ticks = cpu.fetchDecodeAndExecute();
		if (condition)
		{
			ASSERT_EQ(ticks, 3);
			ASSERT_EQ(cpu.pc, startAddr + 2 + offset);
		}
		else
		{
			ASSERT_EQ(ticks, 2);
			ASSERT_EQ(cpu.pc, startAddr + 2);
		}
		ASSERT_EQ(cpu.getFlag(), flag);
	}

	void testLoadValueToMemoryAndIncreaseAddr(byte instruction, byte& regAddrMsb, byte& regAddrLsb, byte& valueReg)
	{
		cpu.pc = 0x00;
		uint16_t addr = (regAddrMsb << 8) | regAddrLsb;
		uint16_t expected_inc_addr = addr + 1;
		mmu.write(cpu.pc, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_EQ(cpu.getFlag(), 0x00);
		ASSERT_EQ(cpu.pc, 1);
		ASSERT_EQ(mmu.read(addr), valueReg);
		ASSERT_EQ(expected_inc_addr >> 8, regAddrMsb);
		ASSERT_EQ(expected_inc_addr & 0xFF, regAddrLsb);
	}

	void testLoadValueToMemoryAndDecreaseAddr(byte instruction, byte& regAddrMsb, byte& regAddrLsb, byte& valueReg)
	{
		cpu.pc = 0x00;
		uint16_t addr = (regAddrMsb << 8) | regAddrLsb;
		uint16_t expected_inc_addr = addr - 1;
		mmu.write(cpu.pc, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_EQ(cpu.getFlag(), 0x00);
		ASSERT_EQ(cpu.pc, 1);
		ASSERT_EQ(mmu.read(addr), valueReg);
		ASSERT_EQ(expected_inc_addr >> 8, regAddrMsb);
		ASSERT_EQ(expected_inc_addr & 0xFF, regAddrLsb);
	}

	void testLoadValueFromMemoryAndIncreaseAddr(byte instruction, byte& valueReg, byte& regAddrMsb, byte& regAddrLsb,
	                                            byte value)
	{
		cpu.pc = 0x00;
		uint16_t addr = (regAddrMsb << 8) | regAddrLsb;
		uint16_t expected_inc_addr = addr + 1;
		mmu.write(addr, value);
		mmu.write(cpu.pc, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_EQ(cpu.getFlag(), 0x00);
		ASSERT_EQ(cpu.pc, 1);
		ASSERT_EQ(valueReg, value);
		ASSERT_EQ(expected_inc_addr >> 8, regAddrMsb);
		ASSERT_EQ(expected_inc_addr & 0xFF, regAddrLsb);
	}

	void testDecimalAdjustAccumulator(byte instruction, bool carryStateBeforeExecution, byte registerValue,
	                                  bool halfCarryStateBeforeExecution, byte expectedRegisterValue,
	                                  bool carryStateAfterExecution)
	{
		cpu.pc = 0x00;
		cpu.a = registerValue;
		cpu.setCarryFlag(carryStateBeforeExecution);
		cpu.setHalfCarryFlag(halfCarryStateBeforeExecution);

		mmu.write(cpu.pc, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 1);
		ASSERT_EQ(cpu.isFlagSet(CPU::ZERO), cpu.a == 0);
		ASSERT_EQ(cpu.pc, 1);
		ASSERT_EQ(cpu.a, expectedRegisterValue);
		ASSERT_EQ(cpu.isFlagSet(CPU::CARRY), carryStateAfterExecution);
		ASSERT_EQ(cpu.isFlagSet(CPU::HALF_CARRY), false);
	}

	void testComplementRegister(byte instruction, byte& reg, byte value, byte expectedValue)
	{
		cpu.pc = 0x00;
		reg = value;
		mmu.write(cpu.pc, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 1);
		ASSERT_TRUE(cpu.isFlagSet(CPU::HALF_CARRY));
		ASSERT_TRUE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_EQ(reg, expectedValue);
		ASSERT_EQ(cpu.pc, 1);
	}

	void testLoadValueIntoRegister(byte instruction, byte& regDst, byte& regSrc, byte value)
	{
		cpu.pc = 0x00;
		regSrc = value;
		mmu.write(cpu.pc, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 1);
		ASSERT_EQ(cpu.getFlag(), 0);
		ASSERT_EQ(regDst, value);
		ASSERT_EQ(cpu.pc, 1);
	}

	void testArithmeticOp8BitsRegisterTo8BitsRegister(byte instruction, byte& reg, byte startValue, byte& regOther,
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
		ASSERT_EQ(cpu.isFlagSet(CPU::HALF_CARRY), expectedHalfCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::CARRY), expectedCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::SUBSTRACTION), expectedSubstractionFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::ZERO), expectedResult == 0);
		ASSERT_EQ(reg, expectedResult);
		ASSERT_EQ(cpu.pc, 1);
	}

	void testArithmeticOpFromMemoryToRegister(byte instruction, byte& reg, byte startValue, byte regAddrMsb,
	                                          byte regAddrLsb, byte value, byte expectedResult, bool carryFlag,
	                                          bool expectedHalfCarryFlag, bool expectedCarryFlag,
	                                          bool expectedSubstractionFlag)
	{
		cpu.pc = 0x00;
		reg = startValue;
		mmu.write(createAddrFromHighAndLowBytes(regAddrMsb, regAddrLsb), value);
		mmu.write(cpu.pc, instruction);
        cpu.setCarryFlag(carryFlag);
        int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_EQ(cpu.isFlagSet(CPU::HALF_CARRY), expectedHalfCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::CARRY), expectedCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::SUBSTRACTION), expectedSubstractionFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::ZERO), expectedResult == 0);
		ASSERT_EQ(reg, expectedResult);
		ASSERT_EQ(cpu.pc, 1);
	}

	void testArithmeticOpImmediateValueToRegister(byte instruction, byte& reg, byte startValue, byte value,
	                                              byte expectedResult, bool carryFlag, bool expectedHalfCarryFlag,
	                                              bool expectedCarryFlag, bool expectedSubstractionFlag)
	{
		cpu.pc = 0x00;
		reg = startValue;
		mmu.write(cpu.pc, instruction);
		mmu.write(cpu.pc + 1, value);
        cpu.setCarryFlag(carryFlag);
        int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_EQ(cpu.isFlagSet(CPU::HALF_CARRY), expectedHalfCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::CARRY), expectedCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::SUBSTRACTION), expectedSubstractionFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::ZERO), expectedResult == 0);
		ASSERT_EQ(reg, expectedResult);
		ASSERT_EQ(cpu.pc, 2);
	}

	void testAdd8BitsRegisterTo8BitsRegister(byte instruction, byte& reg, byte startValue, byte& otherReg, byte value,
	                                         byte expectedResult, bool carryFlag, bool expectedHalfCarryFlag,
	                                         bool expectedCarryFlag)
	{
		testArithmeticOp8BitsRegisterTo8BitsRegister(instruction, reg, startValue, otherReg, value, expectedResult,
		                                             carryFlag, expectedHalfCarryFlag, expectedCarryFlag, false);
	}

	void testAddValueFromMemoryToRegister(byte instruction, byte& reg, byte startValue, byte regAddrMsb,
	                                      byte regAddrLsb, byte value, byte expectedResult, bool carryFlag,
	                                      bool expectedHalfCarryFlag, bool expectedCarryFlag)
	{
		testArithmeticOpFromMemoryToRegister(instruction, reg, startValue, regAddrMsb, regAddrLsb, value,
		                                     expectedResult, carryFlag, expectedHalfCarryFlag, expectedCarryFlag, false);
	}

	void testAddImmediateValueToRegister(byte instruction, byte& reg, byte startValue, byte value,
	                                     byte expectedResult, bool carryFlag, bool expectedHalfCarryFlag,
	                                     bool expectedCarryFlag)
	{
		testArithmeticOpImmediateValueToRegister(instruction, reg, startValue, value, expectedResult, carryFlag,
		                                         expectedHalfCarryFlag, expectedCarryFlag, false);
	}

	void testAdd8BitsRegisterTo8BitsRegister(byte instruction, byte& reg, byte& otherReg)
	{
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0x00, false, false, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0x05, false, false, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x0F, otherReg, 0x05, 0x14, false, true, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x05, 0x04, false, false, true);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x01, 0x00, false, false, true);
	}

	void testAdd8BitsRegisterAndCarryTo8BitsRegister(byte instruction, byte& reg, byte& otherReg)
	{
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0x00, false, false, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0x01, true, false, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0x05, false, false, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0x06, true, false, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x0F, otherReg, 0x00, 0x10, true, true, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x0F, otherReg, 0x05, 0x14, false, true, false);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x05, 0x04, false, false, true);
		testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x00, 0x00, true, false, true);
	}

    void testSub8BitsRegisterTo8BitsRegister(byte instruction, byte& reg, byte startValue, byte& otherReg, byte value,
                                             byte expectedResult, bool carryFlag, bool expectedHalfCarryFlag,
                                             bool expectedCarryFlag)
    {
        testArithmeticOp8BitsRegisterTo8BitsRegister(instruction, reg, startValue, otherReg, value, expectedResult,
                                                     carryFlag, expectedHalfCarryFlag, expectedCarryFlag, true);
    }

    void testSubValueFromMemoryToRegister(byte instruction, byte& reg, byte startValue, byte regAddrMsb,
                                          byte regAddrLsb, byte value, byte expectedResult, bool carryFlag,
                                          bool expectedHalfCarryFlag, bool expectedCarryFlag)
    {
        testArithmeticOpFromMemoryToRegister(instruction, reg, startValue, regAddrMsb, regAddrLsb, value,
                                             expectedResult, carryFlag, expectedHalfCarryFlag, expectedCarryFlag, true);
    }

    void testSubImmediateValueToRegister(byte instruction, byte& reg, byte startValue, byte value,
                                         byte expectedResult, bool carryFlag, bool expectedHalfCarryFlag,
                                         bool expectedCarryFlag)
    {
        testArithmeticOpImmediateValueToRegister(instruction, reg, startValue, value, expectedResult, carryFlag,
                                                 expectedHalfCarryFlag, expectedCarryFlag, true);
    }

    void testSub8BitsRegisterTo8BitsRegister(byte instruction, byte& reg, byte& otherReg)
    {
        testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0x00, false, false, false);
        testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0xFB, false, false, true);
        testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x10, otherReg, 0x05, 0x0B, false, true, false);
        testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x05, 0xFA, false, false, false);
        testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x01, 0xFF, false, false, true);
    }

    void testSub8BitsRegisterAndCarryTo8BitsRegister(byte instruction, byte& reg, byte& otherReg)
    {
        testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0x00, false, false, false);
        testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0xFF, true, false, true);
        testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0xFB, false, false, true);
        testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0xFA, true, false, true);
        testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x10, otherReg, 0x00, 0x0F, true, true, false);
        testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x10, otherReg, 0x05, 0x0B, false, true, false);
        testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x05, 0xFA, false, false, false);
        testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x00, 0xFE, true, false, false);
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

TEST_F(CpuInstructionTest, InstructionLoad8BitsValueInMemoryAddr)
{
	testLoad8BitsValueInMemoryAddr(standardInstructions::LD_BCm_A, cpu.b, cpu.c, cpu.a);
	testLoad8BitsValueInMemoryAddr(standardInstructions::LD_DEm_A, cpu.d, cpu.e, cpu.a);
	testLoad8BitsValueInMemoryAddr(standardInstructions::LD_HLm_B, cpu.h, cpu.l, cpu.b);
	testLoad8BitsValueInMemoryAddr(standardInstructions::LD_HLm_C, cpu.h, cpu.l, cpu.c);
	testLoad8BitsValueInMemoryAddr(standardInstructions::LD_HLm_D, cpu.h, cpu.l, cpu.d);
	testLoad8BitsValueInMemoryAddr(standardInstructions::LD_HLm_E, cpu.h, cpu.l, cpu.e);
	testLoad8BitsValueInMemoryAddr(standardInstructions::LD_HLm_H, cpu.h, cpu.l, cpu.h);
	testLoad8BitsValueInMemoryAddr(standardInstructions::LD_HLm_L, cpu.h, cpu.l, cpu.l);
	testLoad8BitsValueInMemoryAddr(standardInstructions::LD_HLm_A, cpu.h, cpu.l, cpu.a);
}

TEST_F(CpuInstructionTest, InstructionIncrementRegister)
{
	testIncrement8BitsRegisters(standardInstructions::INC_BC, cpu.b, cpu.c);
	testIncrement8BitsRegisters(standardInstructions::INC_DE, cpu.d, cpu.e);
	testIncrement8BitsRegisters(standardInstructions::INC_HL, cpu.h, cpu.l);
	testIncrement16BitsRegister(standardInstructions::INC_SP, cpu.sp);
	testIncrement8BitRegister(standardInstructions::INC_B, cpu.b);
	testIncrement8BitRegister(standardInstructions::INC_C, cpu.c);
	testIncrement8BitRegister(standardInstructions::INC_D, cpu.d);
	testIncrement8BitRegister(standardInstructions::INC_E, cpu.e);
	testIncrement8BitRegister(standardInstructions::INC_H, cpu.h);
	testIncrement8BitRegister(standardInstructions::INC_L, cpu.l);
	testIncrement8BitRegister(standardInstructions::INC_A, cpu.a);
	testIncrement8BitsMemoryValue(standardInstructions::INC_HLm, cpu.h, cpu.l);
}

TEST_F(CpuInstructionTest, InstructionDecrementRegister)
{
	testDecrement8BitsRegisters(standardInstructions::DEC_BC, cpu.b, cpu.c);
	testDecrement8BitsRegisters(standardInstructions::DEC_DE, cpu.d, cpu.e);
	testDecrement8BitsRegisters(standardInstructions::DEC_HL, cpu.h, cpu.l);
	testDecrement16BitsRegister(standardInstructions::DEC_SP, cpu.sp);
	testDecrement8BitRegister(standardInstructions::DEC_B, cpu.b);
	testDecrement8BitRegister(standardInstructions::DEC_C, cpu.c);
	testDecrement8BitRegister(standardInstructions::DEC_D, cpu.d);
	testDecrement8BitRegister(standardInstructions::DEC_E, cpu.e);
	testDecrement8BitRegister(standardInstructions::DEC_H, cpu.h);
	testDecrement8BitRegister(standardInstructions::DEC_L, cpu.l);
	testDecrement8BitRegister(standardInstructions::DEC_A, cpu.a);
	testDecrement8BitsMemoryValue(standardInstructions::DEC_HLm, cpu.h, cpu.l);
}

TEST_F(CpuInstructionTest, InstructionLoadImmediateValue)
{
	testLoadImmediateValue(standardInstructions::LD_A_n, cpu.a);
	testLoadImmediateValue(standardInstructions::LD_B_n, cpu.b);
	testLoadImmediateValue(standardInstructions::LD_C_n, cpu.c);
	testLoadImmediateValue(standardInstructions::LD_D_n, cpu.d);
	testLoadImmediateValue(standardInstructions::LD_E_n, cpu.e);
	testLoadImmediateValue(standardInstructions::LD_H_n, cpu.h);
	testLoadImmediateValue(standardInstructions::LD_L_n, cpu.l);
}

TEST_F(CpuInstructionTest, InstructionRotateLeftCircular)
{
	testRotateLeftCircular({standardInstructions::RLC_A}, cpu.a, 1);
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

TEST_F(CpuInstructionTest, InstructionLoad8BitsRegisterAtImmediateAddr)
{
	byte expectedValue = 0x9A;
	cpu.a = expectedValue;
	cpu.pc = 0x00;
	uint16_t addr = 0x5C8;
	mmu.write(cpu.pc, standardInstructions::LD_nnm_A);
	mmu.writeWord(cpu.pc + 1, addr);
	int ticks = cpu.fetchDecodeAndExecute();
	ASSERT_EQ(ticks, 4);
	ASSERT_EQ(cpu.getFlag(), 0x00);
	uint16_t value = mmu.readWord(addr);
	ASSERT_EQ(value, expectedValue);
	ASSERT_EQ(cpu.pc, 3);
}

TEST_F(CpuInstructionTest, InstructionAddTwo8BitsRegisterToTwo8BitsRegister)
{
	std::vector<uint16_t> startValues = {0x0000, 0x00FF, 0xFFFF, 0x0FFF, 0x0FFF};
	std::vector<uint16_t> addValues = {0x0012, 0x0012, 0x0050, 0x0012, 0x0000};
	addTwo8BitsRegisterToTwo8BitsRegister(standardInstructions::ADD_HL_BC, cpu.h, cpu.l, cpu.b, cpu.c, startValues,
	                                      addValues);
	addTwo8BitsRegisterToTwo8BitsRegister(standardInstructions::ADD_HL_DE, cpu.h, cpu.l, cpu.d, cpu.e, startValues,
	                                      addValues);
}

TEST_F(CpuInstructionTest, InstructionAdd16BitsRegisterToTwo8BitsRegister)
{
	std::vector<uint16_t> startValues = {0x0000, 0x00FF, 0xFFFF, 0x0FFF, 0x0FFF};
	std::vector<uint16_t> addValues = {0x0012, 0x0012, 0x0050, 0x0012, 0x0000};
	add16BitsRegisterToTwo8BitsRegister(standardInstructions::ADD_HL_SP, cpu.h, cpu.l, cpu.sp, startValues, addValues);
}

TEST_F(CpuInstructionTest, InstructionAdd8BitsRegisterToSameRegisters)
{
	std::vector<uint16_t> startValues = {0x0012, 0x00FF, 0x0FFF};
	std::vector<uint16_t> addValues = {0x0012, 0x00FF, 0x0FFF};
	addTwo8BitsRegisterToTwo8BitsRegister(standardInstructions::ADD_HL_HL, cpu.h, cpu.l, cpu.h, cpu.l, startValues,
	                                      addValues);
}

TEST_F(CpuInstructionTest, InstructionLoadValueFromMemoryInto8BitsRegister)
{
	testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_B_HLm, cpu.b, cpu.h, cpu.l);
	testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_D_HLm, cpu.d, cpu.h, cpu.l);
	testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_H_HLm, cpu.h, cpu.h, cpu.l);
	testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_A_BCm, cpu.a, cpu.b, cpu.c);
	testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_A_DEm, cpu.a, cpu.d, cpu.e);
	testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_C_HLm, cpu.c, cpu.h, cpu.l);
	testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_E_HLm, cpu.e, cpu.h, cpu.l);
	testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_L_HLm, cpu.l, cpu.h, cpu.l);
	testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_A_HLm, cpu.a, cpu.h, cpu.l);
}

TEST_F(CpuInstructionTest, InstructionRotateRightCircular)
{
	testRotateRightCircular({standardInstructions::RRC_A}, cpu.a, 1);
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

TEST_F(CpuInstructionTest, InstructionJump)
{
	cpu.pc = 0x00;
	uint16_t addr = 0x1234;
	mmu.write(cpu.pc, standardInstructions::JP_nn);
	mmu.writeWord(cpu.pc + 1, addr);
	int ticks = cpu.fetchDecodeAndExecute();
	ASSERT_EQ(ticks, 4);
	ASSERT_EQ(cpu.getFlag(), 0x00);
	ASSERT_EQ(cpu.pc, addr);
}

TEST_F(CpuInstructionTest, InstructionJumpConditional)
{
	cpu.resetFlags();
	testJumpConditional(standardInstructions::JP_C_nn, false);
	cpu.setFlag(CPU::CARRY);
	testJumpConditional(standardInstructions::JP_C_nn, true);

	cpu.resetFlags();
	testJumpConditional(standardInstructions::JP_NC_nn, true);
	cpu.setFlag(CPU::CARRY);
	testJumpConditional(standardInstructions::JP_NC_nn, false);

	cpu.resetFlags();
	testJumpConditional(standardInstructions::JP_Z_nn, false);
	cpu.setFlag(CPU::ZERO);
	testJumpConditional(standardInstructions::JP_Z_nn, true);

	cpu.resetFlags();
	testJumpConditional(standardInstructions::JP_NZ_nn, true);
	cpu.setFlag(CPU::ZERO);
	testJumpConditional(standardInstructions::JP_NZ_nn, false);
}

TEST_F(CpuInstructionTest, InstructionJumpRegister)
{
	cpu.pc = 0x00;
	cpu.h = 0x12;
	cpu.l = 0x34;
	uint16_t addr = 0x1234;
	mmu.write(cpu.pc, standardInstructions::JP_HLm);
	int ticks = cpu.fetchDecodeAndExecute();
	ASSERT_EQ(ticks, 1);
	ASSERT_EQ(cpu.getFlag(), 0x00);
	ASSERT_EQ(cpu.pc, addr);
}

TEST_F(CpuInstructionTest, InstructionJumpRelative)
{
	testJumpRelative(0);
	testJumpRelative(5);
	testJumpRelative(-5);
	testJumpRelative(127);
	testJumpRelative(-127);
}

TEST_F(CpuInstructionTest, InstructionJumpRelativeConditional)
{
	cpu.resetFlags();
	testJumpRelativeConditional(standardInstructions::JR_C_n, 0, false);
	testJumpRelativeConditional(standardInstructions::JR_C_n, 5, false);
	testJumpRelativeConditional(standardInstructions::JR_C_n, -5, false);
	testJumpRelativeConditional(standardInstructions::JR_C_n, 127, false);
	testJumpRelativeConditional(standardInstructions::JR_C_n, -127, false);
	cpu.setFlag(CPU::CARRY);
	testJumpRelativeConditional(standardInstructions::JR_C_n, 0, true);
	testJumpRelativeConditional(standardInstructions::JR_C_n, 5, true);
	testJumpRelativeConditional(standardInstructions::JR_C_n, -5, true);
	testJumpRelativeConditional(standardInstructions::JR_C_n, 127, true);
	testJumpRelativeConditional(standardInstructions::JR_C_n, -127, true);
	cpu.resetFlags();

	testJumpRelativeConditional(standardInstructions::JR_NC_n, 0, true);
	testJumpRelativeConditional(standardInstructions::JR_NC_n, 5, true);
	testJumpRelativeConditional(standardInstructions::JR_NC_n, -5, true);
	testJumpRelativeConditional(standardInstructions::JR_NC_n, 127, true);
	testJumpRelativeConditional(standardInstructions::JR_NC_n, -127, true);
	cpu.setFlag(CPU::CARRY);
	testJumpRelativeConditional(standardInstructions::JR_NC_n, 0, false);
	testJumpRelativeConditional(standardInstructions::JR_NC_n, 5, false);
	testJumpRelativeConditional(standardInstructions::JR_NC_n, -5, false);
	testJumpRelativeConditional(standardInstructions::JR_NC_n, 127, false);
	testJumpRelativeConditional(standardInstructions::JR_NC_n, -127, false);

	cpu.resetFlags();
	testJumpRelativeConditional(standardInstructions::JR_Z_n, 0, false);
	testJumpRelativeConditional(standardInstructions::JR_Z_n, 5, false);
	testJumpRelativeConditional(standardInstructions::JR_Z_n, -5, false);
	testJumpRelativeConditional(standardInstructions::JR_Z_n, 127, false);
	testJumpRelativeConditional(standardInstructions::JR_Z_n, -127, false);
	cpu.setFlag(CPU::ZERO);
	testJumpRelativeConditional(standardInstructions::JR_Z_n, 0, true);
	testJumpRelativeConditional(standardInstructions::JR_Z_n, 5, true);
	testJumpRelativeConditional(standardInstructions::JR_Z_n, -5, true);
	testJumpRelativeConditional(standardInstructions::JR_Z_n, 127, true);
	testJumpRelativeConditional(standardInstructions::JR_Z_n, -127, true);

	cpu.resetFlags();
	testJumpRelativeConditional(standardInstructions::JR_NZ_n, 0, true);
	testJumpRelativeConditional(standardInstructions::JR_NZ_n, 5, true);
	testJumpRelativeConditional(standardInstructions::JR_NZ_n, -5, true);
	testJumpRelativeConditional(standardInstructions::JR_NZ_n, 127, true);
	testJumpRelativeConditional(standardInstructions::JR_NZ_n, -127, true);
	cpu.setFlag(CPU::ZERO);
	testJumpRelativeConditional(standardInstructions::JR_NZ_n, 0, false);
	testJumpRelativeConditional(standardInstructions::JR_NZ_n, 5, false);
	testJumpRelativeConditional(standardInstructions::JR_NZ_n, -5, false);
	testJumpRelativeConditional(standardInstructions::JR_NZ_n, 127, false);
	testJumpRelativeConditional(standardInstructions::JR_NZ_n, -127, false);
}

TEST_F(CpuInstructionTest, InstructionLoadValueToMemoryAndIncreaseAddr)
{
	cpu.a = 42;
	cpu.h = 0x00;
	cpu.l = 0x01;
	testLoadValueToMemoryAndIncreaseAddr(standardInstructions::LD_HLm_I_A, cpu.h, cpu.l, cpu.a);
	cpu.a = 200;
	cpu.h = 0xFF;
	cpu.l = 0x00;
	testLoadValueToMemoryAndIncreaseAddr(standardInstructions::LD_HLm_I_A, cpu.h, cpu.l, cpu.a);
	cpu.a = 250;
	cpu.h = 0x00;
	cpu.l = 0xFF;
	testLoadValueToMemoryAndIncreaseAddr(standardInstructions::LD_HLm_I_A, cpu.h, cpu.l, cpu.a);
}

TEST_F(CpuInstructionTest, InstructionDecimalAdjustAccumulator)
{
	// Taken from GameBoy programmer manual
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0x00, false, 0x00, false);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0x09, false, 0x09, false);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0x90, false, 0x90, false);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0x99, false, 0x99, false);

	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0x0A, false, 0x0A + 0x06, false);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0x0F, false, 0x0F + 0x06, false);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0x9A, false, 0x9A + 0x06, false);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0x9F, false, 0x9F + 0x06, false);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0x9F, false, 0x9F + 0x06, false);

	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0x00, true, 0x00 + 0x06, false);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0x90, true, 0x90 + 0x06, false);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0x93, true, 0x93 + 0x06, false);

	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0xA0, false, static_cast<byte>(0xA0 + 0x60), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0xA9, false, static_cast<byte>(0xA9 + 0x60), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0xF0, false, static_cast<byte>(0xF0 + 0x60), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0xF9, false, static_cast<byte>(0xF9 + 0x60), true);

	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0xAA, false, static_cast<byte>(0xAA + 0x66), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0xAF, false, static_cast<byte>(0xAF + 0x66), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0xFA, false, static_cast<byte>(0xFA + 0x66), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0xFF, false, static_cast<byte>(0xFF + 0x66), true);

	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0xA0, true, static_cast<byte>(0xA0 + 0x66), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0xA3, true, static_cast<byte>(0xA3 + 0x66), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0xF0, true, static_cast<byte>(0xF0 + 0x66), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, false, 0xF3, true, static_cast<byte>(0xF3 + 0x66), true);

	testDecimalAdjustAccumulator(standardInstructions::DAA, true, 0x00, false, static_cast<byte>(0x00 + 0x60), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, true, 0x02, false, static_cast<byte>(0x02 + 0x60), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, true, 0x30, false, static_cast<byte>(0x30 + 0x60), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, true, 0x39, false, static_cast<byte>(0x39 + 0x60), true);

	testDecimalAdjustAccumulator(standardInstructions::DAA, true, 0x0A, false, static_cast<byte>(0x0A + 0x66), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, true, 0x0F, false, static_cast<byte>(0x0F + 0x66), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, true, 0x2A, false, static_cast<byte>(0x2A + 0x66), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, true, 0x2F, false, static_cast<byte>(0x2F + 0x66), true);

	testDecimalAdjustAccumulator(standardInstructions::DAA, true, 0x00, true, static_cast<byte>(0x00 + 0x66), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, true, 0x03, true, static_cast<byte>(0x03 + 0x66), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, true, 0x30, true, static_cast<byte>(0x30 + 0x66), true);
	testDecimalAdjustAccumulator(standardInstructions::DAA, true, 0x33, true, static_cast<byte>(0x33 + 0x66), true);
}

TEST_F(CpuInstructionTest, InstructionLoadValueFromMemoryAndIncreaseAddr)
{
	cpu.h = 0x00;
	cpu.l = 0x01;
	testLoadValueFromMemoryAndIncreaseAddr(standardInstructions::LD_A_HLm_I, cpu.a, cpu.h, cpu.l, 42);
	cpu.a = 200;
	cpu.h = 0xFF;
	cpu.l = 0x00;
	testLoadValueFromMemoryAndIncreaseAddr(standardInstructions::LD_A_HLm_I, cpu.a, cpu.h, cpu.l, 200);
	cpu.a = 250;
	cpu.h = 0x00;
	cpu.l = 0xFF;
	testLoadValueFromMemoryAndIncreaseAddr(standardInstructions::LD_A_HLm_I, cpu.a, cpu.h, cpu.l, 250);
}

TEST_F(CpuInstructionTest, InstructionComplementRegister)
{
	testComplementRegister(standardInstructions::CPL, cpu.a, 0x00, 0xFF);
	testComplementRegister(standardInstructions::CPL, cpu.a, 0xFF, 0x00);
	testComplementRegister(standardInstructions::CPL, cpu.a, 0x01, 0xFE);
	testComplementRegister(standardInstructions::CPL, cpu.a, 0x10, 0xEF);
	testComplementRegister(standardInstructions::CPL, cpu.a, 0x55, 0xAA);
	testComplementRegister(standardInstructions::CPL, cpu.a, 0xAA, 0x55);
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

TEST_F(CpuInstructionTest, InstructionLoad8BitsImmediateValueInMemory)
{
	cpu.pc = 0x00;
	byte value = 0x12;
	uint16_t addr = 0x3456;
	cpu.h = (addr >> 8);
	cpu.l = (addr & 0xFF);
	mmu.write(cpu.pc, standardInstructions::LD_HLm_n);
	mmu.writeWord(cpu.pc + 1, value);
	int ticks = cpu.fetchDecodeAndExecute();
	ASSERT_EQ(ticks, 3);
	ASSERT_EQ(cpu.getFlag(), 0x00);
	ASSERT_EQ(cpu.pc, 2);
	ASSERT_EQ(mmu.read(addr), value);
}

TEST_F(CpuInstructionTest, InstructionLoadValueIntoRegister)
{
	testLoadValueIntoRegister(standardInstructions::LD_B_B, cpu.b, cpu.b, 0x12);
	testLoadValueIntoRegister(standardInstructions::LD_B_C, cpu.b, cpu.c, 0x34);
	testLoadValueIntoRegister(standardInstructions::LD_B_D, cpu.b, cpu.d, 0x56);
	testLoadValueIntoRegister(standardInstructions::LD_B_E, cpu.b, cpu.e, 0x78);
	testLoadValueIntoRegister(standardInstructions::LD_B_H, cpu.b, cpu.h, 0x9A);
	testLoadValueIntoRegister(standardInstructions::LD_B_L, cpu.b, cpu.l, 0xBC);
	testLoadValueIntoRegister(standardInstructions::LD_B_A, cpu.b, cpu.a, 0xDE);
	testLoadValueIntoRegister(standardInstructions::LD_C_B, cpu.c, cpu.b, 0x12);
	testLoadValueIntoRegister(standardInstructions::LD_C_C, cpu.c, cpu.c, 0x23);
	testLoadValueIntoRegister(standardInstructions::LD_C_D, cpu.c, cpu.d, 0x45);
	testLoadValueIntoRegister(standardInstructions::LD_C_E, cpu.c, cpu.e, 0x67);
	testLoadValueIntoRegister(standardInstructions::LD_C_H, cpu.c, cpu.h, 0x89);
	testLoadValueIntoRegister(standardInstructions::LD_C_L, cpu.c, cpu.l, 0xAB);
	testLoadValueIntoRegister(standardInstructions::LD_C_A, cpu.c, cpu.a, 0x12);

	testLoadValueIntoRegister(standardInstructions::LD_D_B, cpu.d, cpu.b, 0x12);
	testLoadValueIntoRegister(standardInstructions::LD_D_C, cpu.d, cpu.c, 0x23);
	testLoadValueIntoRegister(standardInstructions::LD_D_D, cpu.d, cpu.d, 0x45);
	testLoadValueIntoRegister(standardInstructions::LD_D_E, cpu.d, cpu.e, 0x67);
	testLoadValueIntoRegister(standardInstructions::LD_D_H, cpu.d, cpu.h, 0x89);
	testLoadValueIntoRegister(standardInstructions::LD_D_L, cpu.d, cpu.l, 0xAB);
	testLoadValueIntoRegister(standardInstructions::LD_D_A, cpu.d, cpu.a, 0xCD);
	testLoadValueIntoRegister(standardInstructions::LD_E_B, cpu.e, cpu.b, 0xEF);
	testLoadValueIntoRegister(standardInstructions::LD_E_C, cpu.e, cpu.c, 0x12);
	testLoadValueIntoRegister(standardInstructions::LD_E_D, cpu.e, cpu.d, 0x34);
	testLoadValueIntoRegister(standardInstructions::LD_E_E, cpu.e, cpu.e, 0x56);
	testLoadValueIntoRegister(standardInstructions::LD_E_H, cpu.e, cpu.h, 0x78);
	testLoadValueIntoRegister(standardInstructions::LD_E_L, cpu.e, cpu.l, 0x9A);
	testLoadValueIntoRegister(standardInstructions::LD_E_A, cpu.e, cpu.a, 0xBC);

	testLoadValueIntoRegister(standardInstructions::LD_H_B, cpu.h, cpu.b, 0x12);
	testLoadValueIntoRegister(standardInstructions::LD_H_C, cpu.h, cpu.c, 0x23);
	testLoadValueIntoRegister(standardInstructions::LD_H_D, cpu.h, cpu.d, 0x34);
	testLoadValueIntoRegister(standardInstructions::LD_H_E, cpu.h, cpu.e, 0x56);
	testLoadValueIntoRegister(standardInstructions::LD_H_H, cpu.h, cpu.h, 0x78);
	testLoadValueIntoRegister(standardInstructions::LD_H_L, cpu.h, cpu.l, 0x9A);
	testLoadValueIntoRegister(standardInstructions::LD_H_A, cpu.h, cpu.a, 0xBC);
	testLoadValueIntoRegister(standardInstructions::LD_L_B, cpu.l, cpu.b, 0xDE);
	testLoadValueIntoRegister(standardInstructions::LD_L_C, cpu.l, cpu.c, 0x12);
	testLoadValueIntoRegister(standardInstructions::LD_L_D, cpu.l, cpu.d, 0x23);
	testLoadValueIntoRegister(standardInstructions::LD_L_E, cpu.l, cpu.e, 0x45);
	testLoadValueIntoRegister(standardInstructions::LD_L_H, cpu.l, cpu.h, 0x67);
	testLoadValueIntoRegister(standardInstructions::LD_L_L, cpu.l, cpu.l, 0x89);
	testLoadValueIntoRegister(standardInstructions::LD_L_A, cpu.l, cpu.a, 0xAB);

	testLoadValueIntoRegister(standardInstructions::LD_A_B, cpu.a, cpu.b, 0x12);
	testLoadValueIntoRegister(standardInstructions::LD_A_C, cpu.a, cpu.c, 0x34);
	testLoadValueIntoRegister(standardInstructions::LD_A_D, cpu.a, cpu.d, 0x56);
	testLoadValueIntoRegister(standardInstructions::LD_A_E, cpu.a, cpu.e, 0x78);
	testLoadValueIntoRegister(standardInstructions::LD_A_H, cpu.a, cpu.h, 0x9A);
	testLoadValueIntoRegister(standardInstructions::LD_A_L, cpu.a, cpu.l, 0xBC);
	testLoadValueIntoRegister(standardInstructions::LD_A_A, cpu.a, cpu.a, 0xDE);
}

TEST_F(CpuInstructionTest, InstructionSetCarryFlag)
{
	cpu.pc = 0x00;
	cpu.resetFlags();
	cpu.setFlag(CPU::HALF_CARRY);
	cpu.setFlag(CPU::SUBSTRACTION);
	mmu.write(cpu.pc, standardInstructions::SCF);
	int ticks = cpu.fetchDecodeAndExecute();
	ASSERT_EQ(ticks, 1);
	ASSERT_EQ(cpu.getFlag(), CPU::CARRY);
	ASSERT_EQ(cpu.pc, 1);
}

TEST_F(CpuInstructionTest, InstructionInvertsCarryFlag)
{
	cpu.pc = 0x00;
	cpu.resetFlags();
	cpu.setFlag(CPU::HALF_CARRY);
	cpu.setFlag(CPU::SUBSTRACTION);
	mmu.write(cpu.pc, standardInstructions::CCF);
	int ticks = cpu.fetchDecodeAndExecute();
	ASSERT_EQ(ticks, 1);
	ASSERT_EQ(cpu.getFlag(), CPU::CARRY);
	ASSERT_EQ(cpu.pc, 1);
	mmu.write(cpu.pc, standardInstructions::CCF);
	ticks = cpu.fetchDecodeAndExecute();
	ASSERT_EQ(cpu.getFlag(), 0x00);
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
	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_A, cpu.a, 0xFF, cpu.a, 0xFF, 0xFE, false, false,
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
	                                 false, true);
	testAddValueFromMemoryToRegister(standardInstructions::ADD_A_HLm, cpu.a, 0xFF, cpu.h, cpu.l, 0x01, 0x00, false,
	                                 false, true);
}

TEST_F(CpuInstructionTest, InstructionAddImmediateValueToRegister)
{
	testAddImmediateValueToRegister(standardInstructions::ADD_A_n, cpu.a, 0x00, 0x00, 0x00, false, false, false);
	testAddImmediateValueToRegister(standardInstructions::ADD_A_n, cpu.a, 0x00, 0x05, 0x05, false, false, false);
	testAddImmediateValueToRegister(standardInstructions::ADD_A_n, cpu.a, 0x0F, 0x05, 0x14, false, true, false);
	testAddImmediateValueToRegister(standardInstructions::ADD_A_n, cpu.a, 0xFF, 0x05, 0x04, false, false, true);
	testAddImmediateValueToRegister(standardInstructions::ADD_A_n, cpu.a, 0xFF, 0x01, 0x00, false, false, true);
}

TEST_F(CpuInstructionTest, InstructionAdd8BitsRegisterAndCarryToAnother)
{
	testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_B, cpu.a, cpu.b);
	testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_C, cpu.a, cpu.c);
	testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_D, cpu.a, cpu.d);
	testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_E, cpu.a, cpu.e);
	testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_H, cpu.a, cpu.h);
	testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_L, cpu.a, cpu.l);

//	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.a, 0x00, cpu.a, 0x00, 0x00, false, false,
//	                                    false);
//	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.a, 0x00, cpu.a, 0x00, 0x01, true, false,
//	                                    false);
//
//	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.a, 0x0F, cpu.a, 0x0F, 0x1E, false, true,
//	                                    false);
//	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.a, 0x0F, cpu.a, 0x0F, 0x10, true, true,
//	                                    false);
//
//	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.a, 0xFF, cpu.a, 0xFF, 0xFE, false, false,
//	                                    true);
//	testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.a, 0xFF, cpu.a, 0xFF, 0xFF, true, false,
//	                                    true);
}

TEST_F(CpuInstructionTest, InstructionAddValueFromMemoryAndCarryToRegister)
{
	cpu.h = 0x12;
	cpu.l = 0x23;
	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0x00, cpu.h, cpu.l, 0x00, 0x00,
	                                         false, false, false);
	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0x00, cpu.h, cpu.l, 0x00, 0x01, true,
	                                 false, false);
	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0x00, cpu.h, cpu.l, 0x05, 0x05,
	                                         false, false, false);
	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0x0F, cpu.h, cpu.l, 0x05, 0x14,
	                                         false, true, false);
	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0x0F, cpu.h, cpu.l, 0x05, 0x15,
	                                         true, true, false);

	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0xFF, cpu.h, cpu.l, 0x05, 0x04,
	                                         false, false, true);
	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0xFF, cpu.h, cpu.l, 0x05, 0x05,
	                                         true, false, true);

	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0xFF, cpu.h, cpu.l, 0x01, 0x00,
	                                         false, false, true);
	testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.a, 0xFF, cpu.h, cpu.l, 0x00, 0x00,
	                                         true, false, true);
}

TEST_F(CpuInstructionTest, InstructionAddImmediateValueAndCarryToRegister)
{
	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0x00, 0x00, 0x00, false, false, false);
	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0x00, 0x00, 0x01, true, false, false);

	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0x00, 0x05, 0x05, false, false, false);
	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0x00, 0x05, 0x06, true, false, false);

	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0x0F, 0x05, 0x14, false, true, false);
	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0x0F, 0x05, 0x15, true, true, false);

	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0xFF, 0x05, 0x04, false, false, true);
	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0xFF, 0x05, 0x05, true, false, true);

	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0xFF, 0x01, 0x00, false, false, true);
	testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.a, 0xFF, 0x00, 0x00, true, false, true);
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
    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_A, cpu.a, 0xFF, cpu.a, 0xFF, 0x00, false, true,
                                        false);
    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_A, cpu.a, 0x10, cpu.a, 0x10, 0x00, false, true,
                                        false);
}


TEST_F(CpuInstructionTest, InstructionSubValueFromMemoryToRegister)
{
    cpu.h = 0x12;
    cpu.l = 0x23;
    testSubValueFromMemoryToRegister(standardInstructions::SUB_A_HLm, cpu.a, 0x00, cpu.h, cpu.l, 0x00, 0x00, false,
                                     false, false);
    testSubValueFromMemoryToRegister(standardInstructions::SUB_A_HLm, cpu.a, 0x00, cpu.h, cpu.l, 0x05, 0xFB, false,
                                     false, true);
    testSubValueFromMemoryToRegister(standardInstructions::SUB_A_HLm, cpu.a, 0x10, cpu.h, cpu.l, 0x05, 0x0B, false,
                                     true, false);
    testSubValueFromMemoryToRegister(standardInstructions::SUB_A_HLm, cpu.a, 0xFF, cpu.h, cpu.l, 0x05, 0xFA, false,
                                     false, false);
    testSubValueFromMemoryToRegister(standardInstructions::SUB_A_HLm, cpu.a, 0x00, cpu.h, cpu.l, 0x01, 0xFF, false,
                                     false, true);
}

TEST_F(CpuInstructionTest, InstructionSubImmediateValueToRegister)
{
    testSubImmediateValueToRegister(standardInstructions::SUB_A_n, cpu.a, 0x00, 0x00, 0x00, false, false, false);
    testSubImmediateValueToRegister(standardInstructions::SUB_A_n, cpu.a, 0x00, 0x05, 0xFB, false, false, true);
    testSubImmediateValueToRegister(standardInstructions::SUB_A_n, cpu.a, 0x10, 0x05, 0x0B, false, true, false);
    testSubImmediateValueToRegister(standardInstructions::SUB_A_n, cpu.a, 0xFF, 0x05, 0xFA, false, false, false);
    testSubImmediateValueToRegister(standardInstructions::SUB_A_n, cpu.a, 0x00, 0x01, 0xFF, false, false, true);
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
    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SBC_A_A, cpu.a, 0x00, cpu.a, 0x00, 0xFF, true, false,
                                        true);

    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SBC_A_A, cpu.a, 0x10, cpu.a, 0x10, 0x00, false, true,
                                        false);
	// TODO: Check if half carry flag should be set in this situation
    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SBC_A_A, cpu.a, 0x10, cpu.a, 0x10, 0xFF, true, false,
                                        true);
}