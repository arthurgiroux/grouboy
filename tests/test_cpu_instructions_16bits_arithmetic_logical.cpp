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
	assertAddImmediateValueToSPGaveExpectedResult(0x16, 0x1256);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions16BitsArithmeticLogicalTest, AddImmediateValueToMaxSPGivesOverflow)
{
	cpu.setStackPointer(0xFFFF);
	assertAddImmediateValueToSPGaveExpectedResult(0xFF, 0x00FE);
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