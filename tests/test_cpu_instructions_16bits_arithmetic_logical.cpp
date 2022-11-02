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

	MMU mmu;
	CPU cpu = CPU(mmu);
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