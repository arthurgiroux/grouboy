#include "cpu.hpp"
#include "instructions.hpp"
#include "utils.hpp"

#include <gtest/gtest.h>

using namespace utils;

class CpuInstructions16BitsArithmeticLogicalTest : public ::testing::Test
{
  protected:
	void assertAddImmediateValueToSPGaveExpectedResult(byte addValue, uint16_t expectedValue)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::ADD_SP_d);
		mmu.write(cpu.getProgramCounter() + 1, addValue);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 4);
		ASSERT_EQ(cpu.getProgramCounter(), 2);
		ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
		ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
		ASSERT_EQ(cpu.getStackPointer(), expectedValue);
	}

	void assertIncrementRegisterWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_EQ(cpu.getFlag(), 0x00);
		ASSERT_EQ(cpu.getProgramCounter(), 1);
	}

	void assertDecrementRegisterWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_EQ(cpu.getFlag(), 0x00);
		ASSERT_EQ(cpu.getProgramCounter(), 1);
	}

	void setExpectedTicks(int ticks)
	{
		expectedTicks = ticks;
	}

	void setExpectedFlags(int flags)
	{
		expectedFlags = flags;
	}

	void assertStandardInstructionWasExecutedCorrectly(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, expectedTicks);
		ASSERT_EQ(cpu.getFlag(), expectedFlags);
	}

	void assertExtendedInstructionWasExecutedCorrectly(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::EXT_OPS);
		mmu.write(cpu.getProgramCounter() + 1, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, expectedTicks);
		ASSERT_EQ(cpu.getFlag(), expectedFlags);
	}

	void assertAddBCToHLGivesExpectedResult(int startValue, int addValue, int expectedValue)
	{
		cpu.setRegisterH(getMsbFromWord(startValue));
		cpu.setRegisterL(getLsbFromWord(startValue));
		cpu.setRegisterB(getMsbFromWord(addValue));
		cpu.setRegisterC(getLsbFromWord(addValue));
		setExpectedTicks(2);
		assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_HL_BC);
		ASSERT_EQ(cpu.getRegisterH(), getMsbFromWord(expectedValue));
		ASSERT_EQ(cpu.getRegisterL(), getLsbFromWord(expectedValue));
	}

	void assertAddDEToHLGivesExpectedResult(int startValue, int addValue, int expectedValue)
	{
		cpu.setRegisterH(getMsbFromWord(startValue));
		cpu.setRegisterL(getLsbFromWord(startValue));
		cpu.setRegisterD(getMsbFromWord(addValue));
		cpu.setRegisterE(getLsbFromWord(addValue));
		setExpectedTicks(2);
		assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_HL_DE);
		ASSERT_EQ(cpu.getRegisterH(), getMsbFromWord(expectedValue));
		ASSERT_EQ(cpu.getRegisterL(), getLsbFromWord(expectedValue));
	}

	void assertAddSPToHLGivesExpectedResult(int startValue, int addValue, int expectedValue)
	{
		cpu.setRegisterH(getMsbFromWord(startValue));
		cpu.setRegisterL(getLsbFromWord(startValue));
		cpu.setStackPointer(addValue);
		setExpectedTicks(2);
		assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_HL_SP);
		ASSERT_EQ(cpu.getRegisterH(), getMsbFromWord(expectedValue));
		ASSERT_EQ(cpu.getRegisterL(), getLsbFromWord(expectedValue));
	}

	void assertAddHLToHLGivesExpectedResult(int value, int expectedValue)
	{
		cpu.setRegisterH(getMsbFromWord(value));
		cpu.setRegisterL(getLsbFromWord(value));
		setExpectedTicks(2);
		assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_HL_HL);
		ASSERT_EQ(cpu.getRegisterH(), getMsbFromWord(expectedValue));
		ASSERT_EQ(cpu.getRegisterL(), getLsbFromWord(expectedValue));
	}

	MMU mmu;
	CPU cpu = CPU(mmu);
	int expectedTicks = 1;
	int expectedFlags = 0;
};

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddImmediateValue0ToSPDoesntChangeResult)
{
	cpu.setStackPointer(0x1234);
	assertAddImmediateValueToSPGaveExpectedResult(0x00, 0x1234);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddImmediateValueToSPChangeResult)
{
	cpu.setStackPointer(0x1234);
	assertAddImmediateValueToSPGaveExpectedResult(0x16, 0x124A);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddImmediateValueToMaxSPGivesOverflow)
{
	cpu.setStackPointer(0xFFFF);
	assertAddImmediateValueToSPGaveExpectedResult(0x7F, 0x007E);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddImmediateValueToMidSPRaisesHalfCarry)
{
	cpu.setStackPointer(0x00FF);
	assertAddImmediateValueToSPGaveExpectedResult(0x01, 0x0100);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

#pragma region Increment Registers
TEST_F(CpuInstructions16BitsArithmeticLogicalTest, IncrementRegisterBCFrom0ShouldGive1)
{
	byte startMsb = 0x00;
	byte startLsb = 0x00;
	byte expectedMsb = 0x00;
	byte expectedLsb = 0x01;
	cpu.setRegisterB(startMsb);
	cpu.setRegisterC(startLsb);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_BC);
	ASSERT_EQ(cpu.getRegisterB(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterC(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, IncrementRegisterBCFromMaxShouldGive0)
{
	byte startMsb = 0xFF;
	byte startLsb = 0xFF;
	byte expectedMsb = 0x00;
	byte expectedLsb = 0x00;
	cpu.setRegisterB(startMsb);
	cpu.setRegisterC(startLsb);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_BC);
	ASSERT_EQ(cpu.getRegisterB(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterC(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, IncrementRegisterBCFromMaxLsbShouldIncreaseMsb)
{
	byte startMsb = 0x00;
	byte startLsb = 0xFF;
	byte expectedMsb = 0x01;
	byte expectedLsb = 0x00;
	cpu.setRegisterB(startMsb);
	cpu.setRegisterC(startLsb);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_BC);
	ASSERT_EQ(cpu.getRegisterB(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterC(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, IncrementRegisterDEFrom0ShouldGive1)
{
	byte startMsb = 0x00;
	byte startLsb = 0x00;
	byte expectedMsb = 0x00;
	byte expectedLsb = 0x01;
	cpu.setRegisterD(startMsb);
	cpu.setRegisterE(startLsb);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_DE);
	ASSERT_EQ(cpu.getRegisterD(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterE(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, IncrementRegisterDEFromMaxShouldGive0)
{
	byte startMsb = 0xFF;
	byte startLsb = 0xFF;
	byte expectedMsb = 0x00;
	byte expectedLsb = 0x00;
	cpu.setRegisterD(startMsb);
	cpu.setRegisterE(startLsb);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_DE);
	ASSERT_EQ(cpu.getRegisterD(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterE(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, IncrementRegisterDEFromMaxLsbShouldIncreaseMsb)
{
	byte startMsb = 0x00;
	byte startLsb = 0xFF;
	byte expectedMsb = 0x01;
	byte expectedLsb = 0x00;
	cpu.setRegisterD(startMsb);
	cpu.setRegisterE(startLsb);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_DE);
	ASSERT_EQ(cpu.getRegisterD(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterE(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, IncrementRegisterHLFrom0ShouldGive1)
{
	byte startMsb = 0x00;
	byte startLsb = 0x00;
	byte expectedMsb = 0x00;
	byte expectedLsb = 0x01;
	cpu.setRegisterH(startMsb);
	cpu.setRegisterL(startLsb);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_HL);
	ASSERT_EQ(cpu.getRegisterH(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterL(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, IncrementRegisterHLFromMaxShouldGive0)
{
	byte startMsb = 0xFF;
	byte startLsb = 0xFF;
	byte expectedMsb = 0x00;
	byte expectedLsb = 0x00;
	cpu.setRegisterH(startMsb);
	cpu.setRegisterL(startLsb);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_HL);
	ASSERT_EQ(cpu.getRegisterH(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterL(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, IncrementRegisterHLFromMaxLsbShouldIncreaseMsb)
{
	byte startMsb = 0x00;
	byte startLsb = 0xFF;
	byte expectedMsb = 0x01;
	byte expectedLsb = 0x00;
	cpu.setRegisterH(startMsb);
	cpu.setRegisterL(startLsb);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_HL);
	ASSERT_EQ(cpu.getRegisterH(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterL(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, IncrementRegisterSPFrom0ShouldGive1)
{
	uint16_t startValue = 0x0000;
	uint16_t expectedValue = 0x0001;
	cpu.setStackPointer(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_SP);
	ASSERT_EQ(cpu.getStackPointer(), expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, IncrementRegisterSPFromMaxShouldGive0)
{
	uint16_t startValue = 0xFFFF;
	uint16_t expectedValue = 0x0000;
	cpu.setStackPointer(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_SP);
	ASSERT_EQ(cpu.getStackPointer(), expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, IncrementRegisterSPFromMaxLsbShouldIncreaseMsb)
{
	uint16_t startValue = 0x00FF;
	uint16_t expectedValue = 0x0100;
	cpu.setStackPointer(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_SP);
	ASSERT_EQ(cpu.getStackPointer(), expectedValue);
}
#pragma endregion

#pragma region Decrement Registers
TEST_F(CpuInstructions16BitsArithmeticLogicalTest, DecrementRegisterBCFrom1ShouldGive0)
{
	byte startMsb = 0x00;
	byte startLsb = 0x01;
	byte expectedMsb = 0x00;
	byte expectedLsb = 0x00;
	cpu.setRegisterB(startMsb);
	cpu.setRegisterC(startLsb);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_BC);
	ASSERT_EQ(cpu.getRegisterB(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterC(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, DecrementRegisterBCFrom0ShouldGiveMax)
{
	byte startMsb = 0x00;
	byte startLsb = 0x00;
	byte expectedMsb = 0xFF;
	byte expectedLsb = 0xFF;
	cpu.setRegisterB(startMsb);
	cpu.setRegisterC(startLsb);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_BC);
	ASSERT_EQ(cpu.getRegisterB(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterC(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, DecrementRegisterBCFromMinMsbShouldDecreaseLsb)
{
	byte startMsb = 0x01;
	byte startLsb = 0x00;
	byte expectedMsb = 0x00;
	byte expectedLsb = 0xFF;
	cpu.setRegisterB(startMsb);
	cpu.setRegisterC(startLsb);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_BC);
	ASSERT_EQ(cpu.getRegisterB(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterC(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, DecrementRegisterDEFrom1ShouldGive0)
{
	byte startMsb = 0x00;
	byte startLsb = 0x01;
	byte expectedMsb = 0x00;
	byte expectedLsb = 0x00;
	cpu.setRegisterD(startMsb);
	cpu.setRegisterE(startLsb);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_DE);
	ASSERT_EQ(cpu.getRegisterD(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterE(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, DecrementRegisterDEFrom0ShouldGiveMax)
{
	byte startMsb = 0x00;
	byte startLsb = 0x00;
	byte expectedMsb = 0xFF;
	byte expectedLsb = 0xFF;
	cpu.setRegisterD(startMsb);
	cpu.setRegisterE(startLsb);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_DE);
	ASSERT_EQ(cpu.getRegisterD(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterE(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, DecrementRegisterDEFromMinMsbShouldDecreaseLsb)
{
	byte startMsb = 0x01;
	byte startLsb = 0x00;
	byte expectedMsb = 0x00;
	byte expectedLsb = 0xFF;
	cpu.setRegisterD(startMsb);
	cpu.setRegisterE(startLsb);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_DE);
	ASSERT_EQ(cpu.getRegisterD(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterE(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, DecrementRegisterHLFrom1ShouldGive0)
{
	byte startMsb = 0x00;
	byte startLsb = 0x01;
	byte expectedMsb = 0x00;
	byte expectedLsb = 0x00;
	cpu.setRegisterH(startMsb);
	cpu.setRegisterL(startLsb);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_HL);
	ASSERT_EQ(cpu.getRegisterH(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterL(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, DecrementRegisterHLFrom0ShouldGiveMax)
{
	byte startMsb = 0x00;
	byte startLsb = 0x00;
	byte expectedMsb = 0xFF;
	byte expectedLsb = 0xFF;
	cpu.setRegisterH(startMsb);
	cpu.setRegisterL(startLsb);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_HL);
	ASSERT_EQ(cpu.getRegisterH(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterL(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, DecrementRegisterHLFromMinMsbShouldDecreaseLsb)
{
	byte startMsb = 0x01;
	byte startLsb = 0x00;
	byte expectedMsb = 0x00;
	byte expectedLsb = 0xFF;
	cpu.setRegisterH(startMsb);
	cpu.setRegisterL(startLsb);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_HL);
	ASSERT_EQ(cpu.getRegisterH(), expectedMsb);
	ASSERT_EQ(cpu.getRegisterL(), expectedLsb);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, DecrementRegisterSPFrom1ShouldGive0)
{
	uint16_t startValue = 0x0001;
	uint16_t expectedValue = 0x0000;
	cpu.setStackPointer(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_SP);
	ASSERT_EQ(cpu.getStackPointer(), expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, DecrementRegisterSPFrom0ShouldGiveMax)
{
	uint16_t startValue = 0x0000;
	uint16_t expectedValue = 0xFFFF;
	cpu.setStackPointer(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_SP);
	ASSERT_EQ(cpu.getStackPointer(), expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, DecrementRegisterSPFromMinMsbShouldDecreaseLsb)
{
	uint16_t startValue = 0x0100;
	uint16_t expectedValue = 0x00FF;
	cpu.setStackPointer(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_SP);
	ASSERT_EQ(cpu.getStackPointer(), expectedValue);
}
#pragma endregion

#pragma region Add Registers
TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddRegisterBCToHLSetsValuesCorrectly)
{
	uint16_t startValue = 0x0000;
	uint16_t addValue = 0x0012;
	uint16_t expectedValue = 0x0012;
	int expectedFlag = CPU::CpuFlags::NONE;
	setExpectedFlags(expectedFlag);
	assertAddBCToHLGivesExpectedResult(startValue, addValue, expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddRegisterBCToHLSetsValuesCorrectlyAndSetsHalfCarryFlag)
{
	uint16_t startValue = 0x0FFF;
	uint16_t addValue = 0x0012;
	uint16_t expectedValue = 0x1011;
	int expectedFlag = CPU::CpuFlags::HALF_CARRY;
	setExpectedFlags(expectedFlag);
	assertAddBCToHLGivesExpectedResult(startValue, addValue, expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddRegisterBCToHLSetsValuesCorrectlyAndSetsCarryFlag)
{
	uint16_t startValue = 0xF000;
	uint16_t addValue = 0xF000;
	uint16_t expectedValue = 0xE000;
	int expectedFlag = CPU::CpuFlags::CARRY;
	setExpectedFlags(expectedFlag);
	assertAddBCToHLGivesExpectedResult(startValue, addValue, expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddRegisterBCToHLSetsValuesCorrectlyAndSetsCarryAndHalfCarryFlag)
{
	uint16_t startValue = 0xFFFF;
	uint16_t addValue = 0x0050;
	uint16_t expectedValue = 0x004F;
	int expectedFlag = CPU::CpuFlags::CARRY | CPU::CpuFlags::HALF_CARRY;
	setExpectedFlags(expectedFlag);
	assertAddBCToHLGivesExpectedResult(startValue, addValue, expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddRegisterDEToHLSetsValuesCorrectly)
{
	uint16_t startValue = 0x0000;
	uint16_t addValue = 0x0012;
	uint16_t expectedValue = 0x0012;
	int expectedFlag = CPU::CpuFlags::NONE;
	setExpectedFlags(expectedFlag);
	assertAddDEToHLGivesExpectedResult(startValue, addValue, expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddRegisterDEToHLSetsValuesCorrectlyAndSetsHalfCarryFlag)
{
	uint16_t startValue = 0x0FFF;
	uint16_t addValue = 0x0012;
	uint16_t expectedValue = 0x1011;
	int expectedFlag = CPU::CpuFlags::HALF_CARRY;
	setExpectedFlags(expectedFlag);
	assertAddDEToHLGivesExpectedResult(startValue, addValue, expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddRegisterDEToHLSetsValuesCorrectlyAndSetsCarryFlag)
{
	uint16_t startValue = 0xF000;
	uint16_t addValue = 0xF000;
	uint16_t expectedValue = 0xE000;
	int expectedFlag = CPU::CpuFlags::CARRY;
	setExpectedFlags(expectedFlag);
	assertAddDEToHLGivesExpectedResult(startValue, addValue, expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddRegisterDEToHLSetsValuesCorrectlyAndSetsCarryAndHalfCarryFlag)
{
	uint16_t startValue = 0xFFFF;
	uint16_t addValue = 0x0050;
	uint16_t expectedValue = 0x004F;
	int expectedFlag = CPU::CpuFlags::CARRY | CPU::CpuFlags::HALF_CARRY;
	setExpectedFlags(expectedFlag);
	assertAddDEToHLGivesExpectedResult(startValue, addValue, expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddRegisterSPToHLSetsValuesCorrectly)
{
	uint16_t startValue = 0x0000;
	uint16_t addValue = 0x0012;
	uint16_t expectedValue = 0x0012;
	int expectedFlag = CPU::CpuFlags::NONE;
	setExpectedFlags(expectedFlag);
	assertAddSPToHLGivesExpectedResult(startValue, addValue, expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddRegisterSPToHLSetsValuesCorrectlyAndSetsHalfCarryFlag)
{
	uint16_t startValue = 0x0FFF;
	uint16_t addValue = 0x0012;
	uint16_t expectedValue = 0x1011;
	int expectedFlag = CPU::CpuFlags::HALF_CARRY;
	setExpectedFlags(expectedFlag);
	assertAddSPToHLGivesExpectedResult(startValue, addValue, expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddRegisterSPToHLSetsValuesCorrectlyAndSetsCarryFlag)
{
	uint16_t startValue = 0xF000;
	uint16_t addValue = 0xF000;
	uint16_t expectedValue = 0xE000;
	int expectedFlag = CPU::CpuFlags::CARRY;
	setExpectedFlags(expectedFlag);
	assertAddSPToHLGivesExpectedResult(startValue, addValue, expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddRegisterSPToHLSetsValuesCorrectlyAndSetsCarryAndHalfCarryFlag)
{
	uint16_t startValue = 0xFFFF;
	uint16_t addValue = 0x0050;
	uint16_t expectedValue = 0x004F;
	int expectedFlag = CPU::CpuFlags::CARRY | CPU::CpuFlags::HALF_CARRY;
	setExpectedFlags(expectedFlag);
	assertAddSPToHLGivesExpectedResult(startValue, addValue, expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddRegisterHLToHLSetsValuesCorrectly)
{
	uint16_t value = 0x0012;
	uint16_t expectedValue = 0x0024;
	int expectedFlag = CPU::CpuFlags::NONE;
	setExpectedFlags(expectedFlag);
	assertAddHLToHLGivesExpectedResult(value, expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddRegisterHLToHLSetsValuesCorrectlyAndSetsHalfCarryFlag)
{
	uint16_t value = 0x0FFF;
	uint16_t expectedValue = 0x1FFE;
	int expectedFlag = CPU::CpuFlags::HALF_CARRY;
	setExpectedFlags(expectedFlag);
	assertAddHLToHLGivesExpectedResult(value, expectedValue);
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddRegisterHLToHLSetsValuesCorrectlyAndSetsCarryFlag)
{
	uint16_t value = 0xF000;
	uint16_t expectedValue = 0xE000;
	int expectedFlag = CPU::CpuFlags::CARRY;
	setExpectedFlags(expectedFlag);
	assertAddHLToHLGivesExpectedResult(value, expectedValue);
}
#pragma endregion