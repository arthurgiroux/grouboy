#include "cpu.hpp"
#include "instructions.hpp"
#include "utils.hpp"

#include <gtest/gtest.h>

using namespace utils;

class CpuInstructions8BitsRotationShiftsBitTest : public ::testing::Test
{
  protected:
	void assertShiftLeftArithmeticRegisterWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::EXT_OPS);
		mmu.write(cpu.getProgramCounter() + 1, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_FALSE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_FALSE(cpu.isFlagSet(CPU::HALF_CARRY));
		ASSERT_EQ(cpu.getProgramCounter(), 2);
	}

	void assertShiftLeftArithmeticInMemoryWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::EXT_OPS);
		mmu.write(cpu.getProgramCounter() + 1, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 4);
		ASSERT_FALSE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_FALSE(cpu.isFlagSet(CPU::HALF_CARRY));
		ASSERT_EQ(cpu.getProgramCounter(), 2);
	}

	void assertShiftRightArithmeticRegisterWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::EXT_OPS);
		mmu.write(cpu.getProgramCounter() + 1, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_FALSE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_FALSE(cpu.isFlagSet(CPU::HALF_CARRY));
		ASSERT_EQ(cpu.getProgramCounter(), 2);
	}

	void assertShiftRightArithmeticInMemoryWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::EXT_OPS);
		mmu.write(cpu.getProgramCounter() + 1, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 4);
		ASSERT_FALSE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_FALSE(cpu.isFlagSet(CPU::HALF_CARRY));
		ASSERT_EQ(cpu.getProgramCounter(), 2);
	}

	void assertShiftRightLogicalRegisterWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::EXT_OPS);
		mmu.write(cpu.getProgramCounter() + 1, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_FALSE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_FALSE(cpu.isFlagSet(CPU::HALF_CARRY));
		ASSERT_EQ(cpu.getProgramCounter(), 2);
	}

	void assertShiftRightLogicalInMemoryWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::EXT_OPS);
		mmu.write(cpu.getProgramCounter() + 1, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 4);
		ASSERT_FALSE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_FALSE(cpu.isFlagSet(CPU::HALF_CARRY));
		ASSERT_EQ(cpu.getProgramCounter(), 2);
	}

	void assertSwapNibblesInRegisterWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::EXT_OPS);
		mmu.write(cpu.getProgramCounter() + 1, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_FALSE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_FALSE(cpu.isFlagSet(CPU::HALF_CARRY));
		ASSERT_FALSE(cpu.isFlagSet(CPU::CARRY));
		ASSERT_EQ(cpu.getProgramCounter(), 2);
	}

	void assertSwapNibblesInMemoryWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::EXT_OPS);
		mmu.write(cpu.getProgramCounter() + 1, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 4);
		ASSERT_FALSE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_FALSE(cpu.isFlagSet(CPU::HALF_CARRY));
		ASSERT_FALSE(cpu.isFlagSet(CPU::CARRY));
		ASSERT_EQ(cpu.getProgramCounter(), 2);
	}

	void assertIsBitSetWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::EXT_OPS);
		mmu.write(cpu.getProgramCounter() + 1, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_FALSE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_TRUE(cpu.isFlagSet(CPU::HALF_CARRY));
		ASSERT_FALSE(cpu.isFlagSet(CPU::CARRY));
		ASSERT_EQ(cpu.getProgramCounter(), 2);
	}

	void assertIsBitSetInMemoryWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::EXT_OPS);
		mmu.write(cpu.getProgramCounter() + 1, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 4);
		ASSERT_FALSE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_TRUE(cpu.isFlagSet(CPU::HALF_CARRY));
		ASSERT_FALSE(cpu.isFlagSet(CPU::CARRY));
		ASSERT_EQ(cpu.getProgramCounter(), 2);
	}

	void assertBitResetWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::EXT_OPS);
		mmu.write(cpu.getProgramCounter() + 1, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_FALSE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_FALSE(cpu.isFlagSet(CPU::HALF_CARRY));
		ASSERT_FALSE(cpu.isFlagSet(CPU::CARRY));
		ASSERT_FALSE(cpu.isFlagSet(CPU::ZERO));
		ASSERT_EQ(cpu.getProgramCounter(), 2);
	}

	void assertBitResetInMemoryWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::EXT_OPS);
		mmu.write(cpu.getProgramCounter() + 1, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 4);
		ASSERT_FALSE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_FALSE(cpu.isFlagSet(CPU::HALF_CARRY));
		ASSERT_FALSE(cpu.isFlagSet(CPU::CARRY));
		ASSERT_FALSE(cpu.isFlagSet(CPU::ZERO));
		ASSERT_EQ(cpu.getProgramCounter(), 2);
	}

	void assertBitSetWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::EXT_OPS);
		mmu.write(cpu.getProgramCounter() + 1, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_FALSE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_FALSE(cpu.isFlagSet(CPU::HALF_CARRY));
		ASSERT_FALSE(cpu.isFlagSet(CPU::CARRY));
		ASSERT_FALSE(cpu.isFlagSet(CPU::ZERO));
		ASSERT_EQ(cpu.getProgramCounter(), 2);
	}

	void assertBitSetInMemoryWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::EXT_OPS);
		mmu.write(cpu.getProgramCounter() + 1, instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 4);
		ASSERT_FALSE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_FALSE(cpu.isFlagSet(CPU::HALF_CARRY));
		ASSERT_FALSE(cpu.isFlagSet(CPU::CARRY));
		ASSERT_FALSE(cpu.isFlagSet(CPU::ZERO));
		ASSERT_EQ(cpu.getProgramCounter(), 2);
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

	MMU mmu;
	CPU cpu = CPU(mmu);
	int expectedTicks = 1;
	int expectedFlags = 0;
};

#pragma region Shift Left Arithmetic

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterAForZeroShouldDoNothing)
{
	cpu.setRegisterA(0x00);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_A);
	ASSERT_EQ(cpu.getRegisterA(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterAShouldShiftByOne)
{
	cpu.setRegisterA(0x01);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_A);
	ASSERT_EQ(cpu.getRegisterA(), 0x02);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterAShouldCarryLastBit)
{
	cpu.setRegisterA(0xFF);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_A);
	ASSERT_EQ(cpu.getRegisterA(), 0xFE);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterBForZeroShouldDoNothing)
{
	cpu.setRegisterB(0x00);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_B);
	ASSERT_EQ(cpu.getRegisterB(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterBShouldShiftByOne)
{
	cpu.setRegisterB(0x01);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_B);
	ASSERT_EQ(cpu.getRegisterB(), 0x02);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterBShouldCarryLastBit)
{
	cpu.setRegisterB(0xFF);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_B);
	ASSERT_EQ(cpu.getRegisterB(), 0xFE);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterCForZeroShouldDoNothing)
{
	cpu.setRegisterC(0x00);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_C);
	ASSERT_EQ(cpu.getRegisterC(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterCShouldShiftByOne)
{
	cpu.setRegisterC(0x01);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_C);
	ASSERT_EQ(cpu.getRegisterC(), 0x02);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterCShouldCarryLastBit)
{
	cpu.setRegisterC(0xFF);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_C);
	ASSERT_EQ(cpu.getRegisterC(), 0xFE);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterDForZeroShouldDoNothing)
{
	cpu.setRegisterD(0x00);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_D);
	ASSERT_EQ(cpu.getRegisterD(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterDShouldShiftByOne)
{
	cpu.setRegisterD(0x01);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_D);
	ASSERT_EQ(cpu.getRegisterD(), 0x02);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterDShouldCarryLastBit)
{
	cpu.setRegisterD(0xFF);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_D);
	ASSERT_EQ(cpu.getRegisterD(), 0xFE);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterEForZeroShouldDoNothing)
{
	cpu.setRegisterE(0x00);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_E);
	ASSERT_EQ(cpu.getRegisterE(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterEShouldShiftByOne)
{
	cpu.setRegisterE(0x01);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_E);
	ASSERT_EQ(cpu.getRegisterE(), 0x02);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterEShouldCarryLastBit)
{
	cpu.setRegisterE(0xFF);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_E);
	ASSERT_EQ(cpu.getRegisterE(), 0xFE);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterHForZeroShouldDoNothing)
{
	cpu.setRegisterH(0x00);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_H);
	ASSERT_EQ(cpu.getRegisterH(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterHShouldShiftByOne)
{
	cpu.setRegisterH(0x01);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_H);
	ASSERT_EQ(cpu.getRegisterH(), 0x02);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterHShouldCarryLastBit)
{
	cpu.setRegisterH(0xFF);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_H);
	ASSERT_EQ(cpu.getRegisterH(), 0xFE);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterLForZeroShouldDoNothing)
{
	cpu.setRegisterL(0x00);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_L);
	ASSERT_EQ(cpu.getRegisterL(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterLShouldShiftByOne)
{
	cpu.setRegisterL(0x01);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_L);
	ASSERT_EQ(cpu.getRegisterL(), 0x02);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticRegisterLShouldCarryLastBit)
{
	cpu.setRegisterL(0xFF);
	assertShiftLeftArithmeticRegisterWasPerformed(extendedInstructions::SLA_L);
	ASSERT_EQ(cpu.getRegisterL(), 0xFE);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticMemoryHLForZeroShouldDoNothing)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0x00);
	assertShiftLeftArithmeticInMemoryWasPerformed(extendedInstructions::SLA_HLm);
	ASSERT_EQ(mmu.read(addr), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticMemoryHLShouldShiftByOne)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0x01);
	assertShiftLeftArithmeticInMemoryWasPerformed(extendedInstructions::SLA_HLm);
	ASSERT_EQ(mmu.read(addr), 0x02);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftLeftArithmeticMemoryHLShouldCarryLastBit)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0xFF);
	assertShiftLeftArithmeticInMemoryWasPerformed(extendedInstructions::SLA_HLm);
	ASSERT_EQ(mmu.read(addr), 0xFE);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}
#pragma endregion

#pragma region Shift Right Logical

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterAForZeroShouldDoNothing)
{
	cpu.setRegisterA(0x00);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_A);
	ASSERT_EQ(cpu.getRegisterA(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterAShouldShiftByOne)
{
	cpu.setRegisterA(0x82);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_A);
	ASSERT_EQ(cpu.getRegisterA(), 0x41);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterAShouldCarryFirstBit)
{
	cpu.setRegisterA(0xF1);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_A);
	ASSERT_EQ(cpu.getRegisterA(), 0x78);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterBForZeroShouldDoNothing)
{
	cpu.setRegisterB(0x00);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_B);
	ASSERT_EQ(cpu.getRegisterB(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterBShouldShiftByOne)
{
	cpu.setRegisterB(0x82);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_B);
	ASSERT_EQ(cpu.getRegisterB(), 0x41);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterBShouldCarryFirstBit)
{
	cpu.setRegisterB(0xF1);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_B);
	ASSERT_EQ(cpu.getRegisterB(), 0x78);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterCForZeroShouldDoNothing)
{
	cpu.setRegisterC(0x00);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_C);
	ASSERT_EQ(cpu.getRegisterC(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterCShouldShiftByOne)
{
	cpu.setRegisterC(0x82);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_C);
	ASSERT_EQ(cpu.getRegisterC(), 0x41);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterCShouldCarryFirstBit)
{
	cpu.setRegisterC(0xF1);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_C);
	ASSERT_EQ(cpu.getRegisterC(), 0x78);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterDForZeroShouldDoNothing)
{
	cpu.setRegisterD(0x00);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_D);
	ASSERT_EQ(cpu.getRegisterD(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterDShouldShiftByOne)
{
	cpu.setRegisterD(0x82);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_D);
	ASSERT_EQ(cpu.getRegisterD(), 0x41);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterDShouldCarryFirstBit)
{
	cpu.setRegisterD(0xF1);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_D);
	ASSERT_EQ(cpu.getRegisterD(), 0x78);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterEForZeroShouldDoNothing)
{
	cpu.setRegisterE(0x00);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_E);
	ASSERT_EQ(cpu.getRegisterE(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterEShouldShiftByOne)
{
	cpu.setRegisterE(0x82);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_E);
	ASSERT_EQ(cpu.getRegisterE(), 0x41);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterEShouldCarryFirstBit)
{
	cpu.setRegisterE(0xF1);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_E);
	ASSERT_EQ(cpu.getRegisterE(), 0x78);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterHForZeroShouldDoNothing)
{
	cpu.setRegisterH(0x00);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_H);
	ASSERT_EQ(cpu.getRegisterH(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterHShouldShiftByOne)
{
	cpu.setRegisterH(0x82);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_H);
	ASSERT_EQ(cpu.getRegisterH(), 0x41);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterHShouldCarryFirstBit)
{
	cpu.setRegisterH(0xF1);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_H);
	ASSERT_EQ(cpu.getRegisterH(), 0x78);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterLForZeroShouldDoNothing)
{
	cpu.setRegisterL(0x00);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_L);
	ASSERT_EQ(cpu.getRegisterL(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterLShouldShiftByOne)
{
	cpu.setRegisterL(0x82);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_L);
	ASSERT_EQ(cpu.getRegisterL(), 0x41);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalRegisterLShouldCarryFirstBit)
{
	cpu.setRegisterL(0xF1);
	assertShiftRightLogicalRegisterWasPerformed(extendedInstructions::SRL_L);
	ASSERT_EQ(cpu.getRegisterL(), 0x78);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalMemoryHLForZeroShouldDoNothing)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0x00);
	assertShiftRightLogicalInMemoryWasPerformed(extendedInstructions::SRL_HLm);
	ASSERT_EQ(mmu.read(addr), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalMemoryHLShouldShiftByOne)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0x82);
	assertShiftRightLogicalInMemoryWasPerformed(extendedInstructions::SRL_HLm);
	ASSERT_EQ(mmu.read(addr), 0x41);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightLogicalMemoryHLShouldCarryFirstBit)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0xF1);
	assertShiftRightLogicalInMemoryWasPerformed(extendedInstructions::SRL_HLm);
	ASSERT_EQ(mmu.read(addr), 0x78);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}
#pragma endregion

#pragma region Shift Right Arithmetic

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterAForZeroShouldDoNothing)
{
	cpu.setRegisterA(0x00);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_A);
	ASSERT_EQ(cpu.getRegisterA(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterAShouldShiftByOne)
{
	cpu.setRegisterA(0x82);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_A);
	ASSERT_EQ(cpu.getRegisterA(), 0xC1);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterAShouldCarryFirstBit)
{
	cpu.setRegisterA(0xF1);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_A);
	ASSERT_EQ(cpu.getRegisterA(), 0xF8);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterBForZeroShouldDoNothing)
{
	cpu.setRegisterB(0x00);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_B);
	ASSERT_EQ(cpu.getRegisterB(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterBShouldShiftByOne)
{
	cpu.setRegisterB(0x82);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_B);
	ASSERT_EQ(cpu.getRegisterB(), 0xC1);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterBShouldCarryFirstBit)
{
	cpu.setRegisterB(0xF1);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_B);
	ASSERT_EQ(cpu.getRegisterB(), 0xF8);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterCForZeroShouldDoNothing)
{
	cpu.setRegisterC(0x00);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_C);
	ASSERT_EQ(cpu.getRegisterC(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterCShouldShiftByOne)
{
	cpu.setRegisterC(0x82);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_C);
	ASSERT_EQ(cpu.getRegisterC(), 0xC1);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterCShouldCarryFirstBit)
{
	cpu.setRegisterC(0xF1);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_C);
	ASSERT_EQ(cpu.getRegisterC(), 0xF8);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterDForZeroShouldDoNothing)
{
	cpu.setRegisterD(0x00);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_D);
	ASSERT_EQ(cpu.getRegisterD(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterDShouldShiftByOne)
{
	cpu.setRegisterD(0x82);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_D);
	ASSERT_EQ(cpu.getRegisterD(), 0xC1);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterDShouldCarryFirstBit)
{
	cpu.setRegisterD(0xF1);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_D);
	ASSERT_EQ(cpu.getRegisterD(), 0xF8);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterEForZeroShouldDoNothing)
{
	cpu.setRegisterE(0x00);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_E);
	ASSERT_EQ(cpu.getRegisterE(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterEShouldShiftByOne)
{
	cpu.setRegisterE(0x82);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_E);
	ASSERT_EQ(cpu.getRegisterE(), 0xC1);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterEShouldCarryFirstBit)
{
	cpu.setRegisterE(0xF1);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_E);
	ASSERT_EQ(cpu.getRegisterE(), 0xF8);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterHForZeroShouldDoNothing)
{
	cpu.setRegisterH(0x00);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_H);
	ASSERT_EQ(cpu.getRegisterH(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterHShouldShiftByOne)
{
	cpu.setRegisterH(0x82);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_H);
	ASSERT_EQ(cpu.getRegisterH(), 0xC1);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterHShouldCarryFirstBit)
{
	cpu.setRegisterH(0xF1);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_H);
	ASSERT_EQ(cpu.getRegisterH(), 0xF8);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterLForZeroShouldDoNothing)
{
	cpu.setRegisterL(0x00);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_L);
	ASSERT_EQ(cpu.getRegisterL(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterLShouldShiftByOne)
{
	cpu.setRegisterL(0x82);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_L);
	ASSERT_EQ(cpu.getRegisterL(), 0xC1);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterLShouldCarryFirstBit)
{
	cpu.setRegisterL(0xF1);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_L);
	ASSERT_EQ(cpu.getRegisterL(), 0xF8);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticMemoryHLForZeroShouldDoNothing)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0x00);
	assertShiftRightArithmeticInMemoryWasPerformed(extendedInstructions::SRA_HLm);
	ASSERT_EQ(mmu.read(addr), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticMemoryHLShouldShiftByOne)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0x82);
	assertShiftRightArithmeticInMemoryWasPerformed(extendedInstructions::SRA_HLm);
	ASSERT_EQ(mmu.read(addr), 0xC1);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticMemoryHLShouldCarryFirstBit)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0xF1);
	assertShiftRightArithmeticInMemoryWasPerformed(extendedInstructions::SRA_HLm);
	ASSERT_EQ(mmu.read(addr), 0xF8);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}
#pragma endregion

#pragma region Swap nibbles

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, SwapNibblesRegisterAForZeroShouldDoNothing)
{
	cpu.setRegisterA(0x00);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SWAP_A);
	ASSERT_EQ(cpu.getRegisterA(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, SwapNibblesRegisterAShouldSwapCorrectly)
{
	cpu.setRegisterA(0x12);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SWAP_A);
	ASSERT_EQ(cpu.getRegisterA(), 0x21);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, SwapNibblesRegisterBForZeroShouldDoNothing)
{
	cpu.setRegisterB(0x00);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SWAP_B);
	ASSERT_EQ(cpu.getRegisterB(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, SwapNibblesRegisterBShouldSwapCorrectly)
{
	cpu.setRegisterB(0x12);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SWAP_B);
	ASSERT_EQ(cpu.getRegisterB(), 0x21);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, SwapNibblesRegisterCForZeroShouldDoNothing)
{
	cpu.setRegisterC(0x00);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SWAP_C);
	ASSERT_EQ(cpu.getRegisterC(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, SwapNibblesRegisterCShouldSwapCorrectly)
{
	cpu.setRegisterC(0x12);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SWAP_C);
	ASSERT_EQ(cpu.getRegisterC(), 0x21);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, SwapNibblesRegisterDForZeroShouldDoNothing)
{
	cpu.setRegisterD(0x00);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SWAP_D);
	ASSERT_EQ(cpu.getRegisterD(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, SwapNibblesRegisterDShouldSwapCorrectly)
{
	cpu.setRegisterD(0x12);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SWAP_D);
	ASSERT_EQ(cpu.getRegisterD(), 0x21);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, SwapNibblesRegisterEForZeroShouldDoNothing)
{
	cpu.setRegisterE(0x00);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SWAP_E);
	ASSERT_EQ(cpu.getRegisterE(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, SwapNibblesRegisterEShouldSwapCorrectly)
{
	cpu.setRegisterE(0x12);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SWAP_E);
	ASSERT_EQ(cpu.getRegisterE(), 0x21);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, SwapNibblesRegisterHForZeroShouldDoNothing)
{
	cpu.setRegisterH(0x00);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SWAP_H);
	ASSERT_EQ(cpu.getRegisterH(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, SwapNibblesRegisterHShouldSwapCorrectly)
{
	cpu.setRegisterH(0x12);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SWAP_H);
	ASSERT_EQ(cpu.getRegisterH(), 0x21);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, SwapNibblesRegisterLForZeroShouldDoNothing)
{
	cpu.setRegisterL(0x00);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SWAP_L);
	ASSERT_EQ(cpu.getRegisterL(), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, SwapNibblesRegisterLShouldSwapCorrectly)
{
	cpu.setRegisterL(0x12);
	assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SWAP_L);
	ASSERT_EQ(cpu.getRegisterL(), 0x21);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, SwapNibblesInMemoryHLForZeroShouldDoNothing)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0x00);
	assertShiftRightArithmeticInMemoryWasPerformed(extendedInstructions::SWAP_HLm);
	ASSERT_EQ(mmu.read(addr), 0x00);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, SwapNibblesInMemoryHLShouldSwapCorrectly)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0x56);
	assertShiftRightArithmeticInMemoryWasPerformed(extendedInstructions::SWAP_HLm);
	ASSERT_EQ(mmu.read(addr), 0x65);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}
#pragma endregion

#pragma region Is bit set
TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegABit0WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterA(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_0_A);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegABit0WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterA(0b00000001);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_0_A);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegABit1WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterA(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_1_A);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegABit1WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterA(0b00000010);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_1_A);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegABit2WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterA(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_2_A);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegABit2WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterA(0b00000100);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_2_A);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegABit3WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterA(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_3_A);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegABit3WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterA(0b00001000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_3_A);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegABit4WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterA(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_4_A);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegABit4WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterA(0b00010000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_4_A);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegABit5WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterA(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_5_A);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegABit5WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterA(0b00100000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_5_A);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegABit6WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterA(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_6_A);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegABit6WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterA(0b01000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_6_A);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegABit7WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterA(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_7_A);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegABit7WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterA(0b10000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_7_A);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegBBit0WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterB(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_0_B);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegBBit0WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterB(0b00000001);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_0_B);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegBBit1WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterB(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_1_B);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegBBit1WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterB(0b00000010);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_1_B);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegBBit2WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterB(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_2_B);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegBBit2WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterB(0b00000100);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_2_B);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegBBit3WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterB(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_3_B);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegBBit3WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterB(0b00001000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_3_B);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegBBit4WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterB(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_4_B);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegBBit4WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterB(0b00010000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_4_B);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegBBit5WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterB(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_5_B);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegBBit5WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterB(0b00100000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_5_B);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegBBit6WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterB(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_6_B);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegBBit6WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterB(0b01000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_6_B);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegBBit7WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterB(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_7_B);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegBBit7WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterB(0b10000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_7_B);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegCBit0WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterC(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_0_C);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegCBit0WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterC(0b00000001);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_0_C);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegCBit1WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterC(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_1_C);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegCBit1WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterC(0b00000010);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_1_C);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegCBit2WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterC(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_2_C);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegCBit2WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterC(0b00000100);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_2_C);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegCBit3WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterC(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_3_C);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegCBit3WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterC(0b00001000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_3_C);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegCBit4WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterC(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_4_C);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegCBit4WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterC(0b00010000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_4_C);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegCBit5WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterC(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_5_C);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegCBit5WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterC(0b00100000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_5_C);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegCBit6WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterC(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_6_C);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegCBit6WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterC(0b01000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_6_C);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegCBit7WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterC(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_7_C);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegCBit7WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterC(0b10000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_7_C);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegDBit0WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterD(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_0_D);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegDBit0WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterD(0b00000001);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_0_D);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegDBit1WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterD(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_1_D);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegDBit1WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterD(0b00000010);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_1_D);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegDBit2WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterD(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_2_D);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegDBit2WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterD(0b00000100);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_2_D);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegDBit3WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterD(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_3_D);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegDBit3WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterD(0b00001000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_3_D);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegDBit4WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterD(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_4_D);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegDBit4WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterD(0b00010000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_4_D);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegDBit5WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterD(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_5_D);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegDBit5WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterD(0b00100000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_5_D);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegDBit6WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterD(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_6_D);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegDBit6WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterD(0b01000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_6_D);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegDBit7WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterD(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_7_D);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegDBit7WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterD(0b10000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_7_D);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegEBit0WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterE(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_0_E);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegEBit0WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterE(0b00000001);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_0_E);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegEBit1WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterE(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_1_E);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegEBit1WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterE(0b00000010);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_1_E);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegEBit2WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterE(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_2_E);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegEBit2WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterE(0b00000100);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_2_E);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegEBit3WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterE(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_3_E);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegEBit3WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterE(0b00001000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_3_E);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegEBit4WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterE(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_4_E);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegEBit4WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterE(0b00010000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_4_E);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegEBit5WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterE(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_5_E);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegEBit5WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterE(0b00100000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_5_E);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegEBit6WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterE(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_6_E);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegEBit6WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterE(0b01000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_6_E);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegEBit7WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterE(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_7_E);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegEBit7WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterE(0b10000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_7_E);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegHBit0WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterH(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_0_H);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegHBit0WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterH(0b00000001);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_0_H);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegHBit1WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterH(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_1_H);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegHBit1WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterH(0b00000010);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_1_H);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegHBit2WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterH(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_2_H);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegHBit2WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterH(0b00000100);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_2_H);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegHBit3WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterH(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_3_H);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegHBit3WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterH(0b00001000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_3_H);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegHBit4WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterH(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_4_H);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegHBit4WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterH(0b00010000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_4_H);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegHBit5WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterH(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_5_H);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegHBit5WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterH(0b00100000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_5_H);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegHBit6WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterH(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_6_H);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegHBit6WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterH(0b01000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_6_H);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegHBit7WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterH(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_7_H);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegHBit7WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterH(0b10000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_7_H);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegLBit0WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterL(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_0_L);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegLBit0WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterL(0b00000001);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_0_L);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegLBit1WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterL(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_1_L);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegLBit1WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterL(0b00000010);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_1_L);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegLBit2WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterL(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_2_L);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegLBit2WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterL(0b00000100);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_2_L);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegLBit3WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterL(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_3_L);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegLBit3WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterL(0b00001000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_3_L);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegLBit4WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterL(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_4_L);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegLBit4WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterL(0b00010000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_4_L);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegLBit5WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterL(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_5_L);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegLBit5WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterL(0b00100000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_5_L);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegLBit6WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterL(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_6_L);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegLBit6WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterL(0b01000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_6_L);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegLBit7WhenUnsetShouldRaiseZeroFlag)
{
	cpu.setRegisterL(0b00000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_7_L);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForRegLBit7WhenSetShouldNotRaiseZeroFlag)
{
	cpu.setRegisterL(0b10000000);
	assertIsBitSetWasPerformed(extendedInstructions::BIT_7_L);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForMemoryHLBit0WhenUnsetShouldRaiseZeroFlag)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000000);
	assertIsBitSetInMemoryWasPerformed(extendedInstructions::BIT_0_HLm);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForMemoryHLBit0WhenSetShouldNotRaiseZeroFlag)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000001);
	assertIsBitSetInMemoryWasPerformed(extendedInstructions::BIT_0_HLm);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForMemoryHLBit1WhenUnsetShouldRaiseZeroFlag)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000000);
	assertIsBitSetInMemoryWasPerformed(extendedInstructions::BIT_1_HLm);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForMemoryHLBit1WhenSetShouldNotRaiseZeroFlag)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000010);
	assertIsBitSetInMemoryWasPerformed(extendedInstructions::BIT_1_HLm);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForMemoryHLBit2WhenUnsetShouldRaiseZeroFlag)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000000);
	assertIsBitSetInMemoryWasPerformed(extendedInstructions::BIT_2_HLm);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForMemoryHLBit2WhenSetShouldNotRaiseZeroFlag)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000100);
	assertIsBitSetInMemoryWasPerformed(extendedInstructions::BIT_2_HLm);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForMemoryHLBit3WhenUnsetShouldRaiseZeroFlag)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000000);
	assertIsBitSetInMemoryWasPerformed(extendedInstructions::BIT_3_HLm);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForMemoryHLBit3WhenSetShouldNotRaiseZeroFlag)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00001000);
	assertIsBitSetInMemoryWasPerformed(extendedInstructions::BIT_3_HLm);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForMemoryHLBit4WhenUnsetShouldRaiseZeroFlag)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000000);
	assertIsBitSetInMemoryWasPerformed(extendedInstructions::BIT_4_HLm);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForMemoryHLBit4WhenSetShouldNotRaiseZeroFlag)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00010000);
	assertIsBitSetInMemoryWasPerformed(extendedInstructions::BIT_4_HLm);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForMemoryHLBit5WhenUnsetShouldRaiseZeroFlag)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000000);
	assertIsBitSetInMemoryWasPerformed(extendedInstructions::BIT_5_HLm);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForMemoryHLBit5WhenSetShouldNotRaiseZeroFlag)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00100000);
	assertIsBitSetInMemoryWasPerformed(extendedInstructions::BIT_5_HLm);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForMemoryHLBit6WhenUnsetShouldRaiseZeroFlag)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000000);
	assertIsBitSetInMemoryWasPerformed(extendedInstructions::BIT_6_HLm);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForMemoryHLBit6WhenSetShouldNotRaiseZeroFlag)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b01000000);
	assertIsBitSetInMemoryWasPerformed(extendedInstructions::BIT_6_HLm);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForMemoryHLBit7WhenUnsetShouldRaiseZeroFlag)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000000);
	assertIsBitSetInMemoryWasPerformed(extendedInstructions::BIT_7_HLm);
	ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, IsBitSetForMemoryHLBit7WhenSetShouldNotRaiseZeroFlag)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b10000000);
	assertIsBitSetInMemoryWasPerformed(extendedInstructions::BIT_7_HLm);
	ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}
#pragma endregion

#pragma region Bit Reset
TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegABit0ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterA(0b11111110);
	assertBitResetWasPerformed(extendedInstructions::RES_0_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b11111110);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegABit0ShouldResetWhenSet)
{
	cpu.setRegisterA(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_0_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b11111110);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegABit1ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterA(0b11111101);
	assertBitResetWasPerformed(extendedInstructions::RES_1_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b11111101);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegABit1ShouldResetWhenSet)
{
	cpu.setRegisterA(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_1_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b11111101);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegABit2ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterA(0b11111011);
	assertBitResetWasPerformed(extendedInstructions::RES_2_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b11111011);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegABit2ShouldResetWhenSet)
{
	cpu.setRegisterA(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_2_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b11111011);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegABit3ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterA(0b11110111);
	assertBitResetWasPerformed(extendedInstructions::RES_3_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b11110111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegABit3ShouldResetWhenSet)
{
	cpu.setRegisterA(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_3_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b11110111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegABit4ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterA(0b11101111);
	assertBitResetWasPerformed(extendedInstructions::RES_4_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b11101111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegABit4ShouldResetWhenSet)
{
	cpu.setRegisterA(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_4_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b11101111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegABit5ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterA(0b11011111);
	assertBitResetWasPerformed(extendedInstructions::RES_5_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b11011111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegABit5ShouldResetWhenSet)
{
	cpu.setRegisterA(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_5_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b11011111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegABit6ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterA(0b10111111);
	assertBitResetWasPerformed(extendedInstructions::RES_6_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b10111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegABit6ShouldResetWhenSet)
{
	cpu.setRegisterA(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_6_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b10111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegABit7ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterA(0b01111111);
	assertBitResetWasPerformed(extendedInstructions::RES_7_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b01111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegABit7ShouldResetWhenSet)
{
	cpu.setRegisterA(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_7_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b01111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegBBit0ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterB(0b11111110);
	assertBitResetWasPerformed(extendedInstructions::RES_0_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b11111110);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegBBit0ShouldResetWhenSet)
{
	cpu.setRegisterB(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_0_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b11111110);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegBBit1ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterB(0b11111101);
	assertBitResetWasPerformed(extendedInstructions::RES_1_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b11111101);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegBBit1ShouldResetWhenSet)
{
	cpu.setRegisterB(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_1_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b11111101);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegBBit2ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterB(0b11111011);
	assertBitResetWasPerformed(extendedInstructions::RES_2_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b11111011);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegBBit2ShouldResetWhenSet)
{
	cpu.setRegisterB(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_2_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b11111011);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegBBit3ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterB(0b11110111);
	assertBitResetWasPerformed(extendedInstructions::RES_3_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b11110111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegBBit3ShouldResetWhenSet)
{
	cpu.setRegisterB(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_3_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b11110111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegBBit4ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterB(0b11101111);
	assertBitResetWasPerformed(extendedInstructions::RES_4_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b11101111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegBBit4ShouldResetWhenSet)
{
	cpu.setRegisterB(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_4_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b11101111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegBBit5ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterB(0b11011111);
	assertBitResetWasPerformed(extendedInstructions::RES_5_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b11011111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegBBit5ShouldResetWhenSet)
{
	cpu.setRegisterB(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_5_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b11011111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegBBit6ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterB(0b10111111);
	assertBitResetWasPerformed(extendedInstructions::RES_6_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b10111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegBBit6ShouldResetWhenSet)
{
	cpu.setRegisterB(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_6_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b10111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegBBit7ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterB(0b01111111);
	assertBitResetWasPerformed(extendedInstructions::RES_7_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b01111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegBBit7ShouldResetWhenSet)
{
	cpu.setRegisterB(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_7_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b01111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegCBit0ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterC(0b11111110);
	assertBitResetWasPerformed(extendedInstructions::RES_0_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b11111110);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegCBit0ShouldResetWhenSet)
{
	cpu.setRegisterC(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_0_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b11111110);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegCBit1ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterC(0b11111101);
	assertBitResetWasPerformed(extendedInstructions::RES_1_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b11111101);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegCBit1ShouldResetWhenSet)
{
	cpu.setRegisterC(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_1_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b11111101);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegCBit2ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterC(0b11111011);
	assertBitResetWasPerformed(extendedInstructions::RES_2_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b11111011);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegCBit2ShouldResetWhenSet)
{
	cpu.setRegisterC(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_2_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b11111011);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegCBit3ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterC(0b11110111);
	assertBitResetWasPerformed(extendedInstructions::RES_3_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b11110111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegCBit3ShouldResetWhenSet)
{
	cpu.setRegisterC(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_3_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b11110111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegCBit4ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterC(0b11101111);
	assertBitResetWasPerformed(extendedInstructions::RES_4_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b11101111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegCBit4ShouldResetWhenSet)
{
	cpu.setRegisterC(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_4_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b11101111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegCBit5ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterC(0b11011111);
	assertBitResetWasPerformed(extendedInstructions::RES_5_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b11011111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegCBit5ShouldResetWhenSet)
{
	cpu.setRegisterC(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_5_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b11011111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegCBit6ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterC(0b10111111);
	assertBitResetWasPerformed(extendedInstructions::RES_6_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b10111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegCBit6ShouldResetWhenSet)
{
	cpu.setRegisterC(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_6_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b10111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegCBit7ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterC(0b01111111);
	assertBitResetWasPerformed(extendedInstructions::RES_7_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b01111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegCBit7ShouldResetWhenSet)
{
	cpu.setRegisterC(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_7_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b01111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegDBit0ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterD(0b11111110);
	assertBitResetWasPerformed(extendedInstructions::RES_0_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b11111110);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegDBit0ShouldResetWhenSet)
{
	cpu.setRegisterD(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_0_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b11111110);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegDBit1ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterD(0b11111101);
	assertBitResetWasPerformed(extendedInstructions::RES_1_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b11111101);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegDBit1ShouldResetWhenSet)
{
	cpu.setRegisterD(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_1_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b11111101);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegDBit2ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterD(0b11111011);
	assertBitResetWasPerformed(extendedInstructions::RES_2_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b11111011);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegDBit2ShouldResetWhenSet)
{
	cpu.setRegisterD(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_2_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b11111011);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegDBit3ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterD(0b11110111);
	assertBitResetWasPerformed(extendedInstructions::RES_3_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b11110111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegDBit3ShouldResetWhenSet)
{
	cpu.setRegisterD(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_3_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b11110111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegDBit4ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterD(0b11101111);
	assertBitResetWasPerformed(extendedInstructions::RES_4_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b11101111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegDBit4ShouldResetWhenSet)
{
	cpu.setRegisterD(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_4_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b11101111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegDBit5ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterD(0b11011111);
	assertBitResetWasPerformed(extendedInstructions::RES_5_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b11011111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegDBit5ShouldResetWhenSet)
{
	cpu.setRegisterD(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_5_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b11011111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegDBit6ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterD(0b10111111);
	assertBitResetWasPerformed(extendedInstructions::RES_6_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b10111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegDBit6ShouldResetWhenSet)
{
	cpu.setRegisterD(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_6_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b10111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegDBit7ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterD(0b01111111);
	assertBitResetWasPerformed(extendedInstructions::RES_7_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b01111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegDBit7ShouldResetWhenSet)
{
	cpu.setRegisterD(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_7_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b01111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegEBit0ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterE(0b11111110);
	assertBitResetWasPerformed(extendedInstructions::RES_0_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b11111110);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegEBit0ShouldResetWhenSet)
{
	cpu.setRegisterE(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_0_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b11111110);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegEBit1ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterE(0b11111101);
	assertBitResetWasPerformed(extendedInstructions::RES_1_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b11111101);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegEBit1ShouldResetWhenSet)
{
	cpu.setRegisterE(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_1_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b11111101);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegEBit2ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterE(0b11111011);
	assertBitResetWasPerformed(extendedInstructions::RES_2_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b11111011);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegEBit2ShouldResetWhenSet)
{
	cpu.setRegisterE(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_2_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b11111011);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegEBit3ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterE(0b11110111);
	assertBitResetWasPerformed(extendedInstructions::RES_3_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b11110111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegEBit3ShouldResetWhenSet)
{
	cpu.setRegisterE(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_3_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b11110111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegEBit4ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterE(0b11101111);
	assertBitResetWasPerformed(extendedInstructions::RES_4_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b11101111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegEBit4ShouldResetWhenSet)
{
	cpu.setRegisterE(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_4_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b11101111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegEBit5ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterE(0b11011111);
	assertBitResetWasPerformed(extendedInstructions::RES_5_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b11011111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegEBit5ShouldResetWhenSet)
{
	cpu.setRegisterE(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_5_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b11011111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegEBit6ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterE(0b10111111);
	assertBitResetWasPerformed(extendedInstructions::RES_6_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b10111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegEBit6ShouldResetWhenSet)
{
	cpu.setRegisterE(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_6_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b10111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegEBit7ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterE(0b01111111);
	assertBitResetWasPerformed(extendedInstructions::RES_7_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b01111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegEBit7ShouldResetWhenSet)
{
	cpu.setRegisterE(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_7_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b01111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegHBit0ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterH(0b11111110);
	assertBitResetWasPerformed(extendedInstructions::RES_0_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b11111110);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegHBit0ShouldResetWhenSet)
{
	cpu.setRegisterH(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_0_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b11111110);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegHBit1ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterH(0b11111101);
	assertBitResetWasPerformed(extendedInstructions::RES_1_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b11111101);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegHBit1ShouldResetWhenSet)
{
	cpu.setRegisterH(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_1_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b11111101);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegHBit2ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterH(0b11111011);
	assertBitResetWasPerformed(extendedInstructions::RES_2_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b11111011);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegHBit2ShouldResetWhenSet)
{
	cpu.setRegisterH(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_2_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b11111011);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegHBit3ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterH(0b11110111);
	assertBitResetWasPerformed(extendedInstructions::RES_3_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b11110111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegHBit3ShouldResetWhenSet)
{
	cpu.setRegisterH(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_3_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b11110111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegHBit4ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterH(0b11101111);
	assertBitResetWasPerformed(extendedInstructions::RES_4_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b11101111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegHBit4ShouldResetWhenSet)
{
	cpu.setRegisterH(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_4_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b11101111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegHBit5ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterH(0b11011111);
	assertBitResetWasPerformed(extendedInstructions::RES_5_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b11011111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegHBit5ShouldResetWhenSet)
{
	cpu.setRegisterH(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_5_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b11011111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegHBit6ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterH(0b10111111);
	assertBitResetWasPerformed(extendedInstructions::RES_6_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b10111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegHBit6ShouldResetWhenSet)
{
	cpu.setRegisterH(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_6_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b10111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegHBit7ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterH(0b01111111);
	assertBitResetWasPerformed(extendedInstructions::RES_7_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b01111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegHBit7ShouldResetWhenSet)
{
	cpu.setRegisterH(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_7_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b01111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegLBit0ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterL(0b11111110);
	assertBitResetWasPerformed(extendedInstructions::RES_0_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b11111110);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegLBit0ShouldResetWhenSet)
{
	cpu.setRegisterL(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_0_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b11111110);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegLBit1ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterL(0b11111101);
	assertBitResetWasPerformed(extendedInstructions::RES_1_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b11111101);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegLBit1ShouldResetWhenSet)
{
	cpu.setRegisterL(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_1_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b11111101);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegLBit2ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterL(0b11111011);
	assertBitResetWasPerformed(extendedInstructions::RES_2_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b11111011);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegLBit2ShouldResetWhenSet)
{
	cpu.setRegisterL(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_2_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b11111011);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegLBit3ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterL(0b11110111);
	assertBitResetWasPerformed(extendedInstructions::RES_3_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b11110111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegLBit3ShouldResetWhenSet)
{
	cpu.setRegisterL(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_3_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b11110111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegLBit4ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterL(0b11101111);
	assertBitResetWasPerformed(extendedInstructions::RES_4_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b11101111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegLBit4ShouldResetWhenSet)
{
	cpu.setRegisterL(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_4_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b11101111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegLBit5ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterL(0b11011111);
	assertBitResetWasPerformed(extendedInstructions::RES_5_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b11011111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegLBit5ShouldResetWhenSet)
{
	cpu.setRegisterL(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_5_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b11011111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegLBit6ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterL(0b10111111);
	assertBitResetWasPerformed(extendedInstructions::RES_6_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b10111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegLBit6ShouldResetWhenSet)
{
	cpu.setRegisterL(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_6_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b10111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegLBit7ShouldNotChangeBitWhenUnset)
{
	cpu.setRegisterL(0b01111111);
	assertBitResetWasPerformed(extendedInstructions::RES_7_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b01111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForRegLBit7ShouldResetWhenSet)
{
	cpu.setRegisterL(0b11111111);
	assertBitResetWasPerformed(extendedInstructions::RES_7_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b01111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForMemoryHLBit0ShouldNotChangeBitWhenUnset)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b11111110);
	assertBitResetInMemoryWasPerformed(extendedInstructions::RES_0_HLm);
	ASSERT_EQ(mmu.read(addr), 0b11111110);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForMemoryHLBit0ShouldResetWhenSet)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b11111111);
	assertBitResetInMemoryWasPerformed(extendedInstructions::RES_0_HLm);
	ASSERT_EQ(mmu.read(addr), 0b11111110);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForMemoryHLBit1ShouldNotChangeBitWhenUnset)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b11111101);
	assertBitResetInMemoryWasPerformed(extendedInstructions::RES_1_HLm);
	ASSERT_EQ(mmu.read(addr), 0b11111101);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForMemoryHLBit1ShouldResetWhenSet)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b11111111);
	assertBitResetInMemoryWasPerformed(extendedInstructions::RES_1_HLm);
	ASSERT_EQ(mmu.read(addr), 0b11111101);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForMemoryHLBit2ShouldNotChangeBitWhenUnset)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b11111011);
	assertBitResetInMemoryWasPerformed(extendedInstructions::RES_2_HLm);
	ASSERT_EQ(mmu.read(addr), 0b11111011);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForMemoryHLBit2ShouldResetWhenSet)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b11111111);
	assertBitResetInMemoryWasPerformed(extendedInstructions::RES_2_HLm);
	ASSERT_EQ(mmu.read(addr), 0b11111011);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForMemoryHLBit3ShouldNotChangeBitWhenUnset)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b11110111);
	assertBitResetInMemoryWasPerformed(extendedInstructions::RES_3_HLm);
	ASSERT_EQ(mmu.read(addr), 0b11110111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForMemoryHLBit3ShouldResetWhenSet)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b11111111);
	assertBitResetInMemoryWasPerformed(extendedInstructions::RES_3_HLm);
	ASSERT_EQ(mmu.read(addr), 0b11110111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForMemoryHLBit4ShouldNotChangeBitWhenUnset)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b11101111);
	assertBitResetInMemoryWasPerformed(extendedInstructions::RES_4_HLm);
	ASSERT_EQ(mmu.read(addr), 0b11101111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForMemoryHLBit4ShouldResetWhenSet)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b11111111);
	assertBitResetInMemoryWasPerformed(extendedInstructions::RES_4_HLm);
	ASSERT_EQ(mmu.read(addr), 0b11101111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForMemoryHLBit5ShouldNotChangeBitWhenUnset)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b11011111);
	assertBitResetInMemoryWasPerformed(extendedInstructions::RES_5_HLm);
	ASSERT_EQ(mmu.read(addr), 0b11011111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForMemoryHLBit5ShouldResetWhenSet)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b11111111);
	assertBitResetInMemoryWasPerformed(extendedInstructions::RES_5_HLm);
	ASSERT_EQ(mmu.read(addr), 0b11011111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForMemoryHLBit6ShouldNotChangeBitWhenUnset)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b10111111);
	assertBitResetInMemoryWasPerformed(extendedInstructions::RES_6_HLm);
	ASSERT_EQ(mmu.read(addr), 0b10111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForMemoryHLBit6ShouldResetWhenSet)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b11111111);
	assertBitResetInMemoryWasPerformed(extendedInstructions::RES_6_HLm);
	ASSERT_EQ(mmu.read(addr), 0b10111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForMemoryHLBit7ShouldNotChangeBitWhenUnset)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b01111111);
	assertBitResetInMemoryWasPerformed(extendedInstructions::RES_7_HLm);
	ASSERT_EQ(mmu.read(addr), 0b01111111);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitResetForMemoryHLBit7ShouldResetWhenSet)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b11111111);
	assertBitResetInMemoryWasPerformed(extendedInstructions::RES_7_HLm);
	ASSERT_EQ(mmu.read(addr), 0b01111111);
}
#pragma endregion

#pragma region Bit Set
TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegABit0ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterA(0b00000001);
	assertBitSetWasPerformed(extendedInstructions::SET_0_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegABit0ShouldSetBitWhenUnset)
{
	cpu.setRegisterA(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_0_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegABit1ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterA(0b00000010);
	assertBitSetWasPerformed(extendedInstructions::SET_1_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegABit1ShouldSetBitWhenUnset)
{
	cpu.setRegisterA(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_1_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegABit2ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterA(0b00000100);
	assertBitSetWasPerformed(extendedInstructions::SET_2_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00000100);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegABit2ShouldSetBitWhenUnset)
{
	cpu.setRegisterA(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_2_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00000100);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegABit3ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterA(0b00001000);
	assertBitSetWasPerformed(extendedInstructions::SET_3_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00001000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegABit3ShouldSetBitWhenUnset)
{
	cpu.setRegisterA(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_3_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00001000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegABit4ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterA(0b00010000);
	assertBitSetWasPerformed(extendedInstructions::SET_4_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00010000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegABit4ShouldSetBitWhenUnset)
{
	cpu.setRegisterA(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_4_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00010000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegABit5ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterA(0b00100000);
	assertBitSetWasPerformed(extendedInstructions::SET_5_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00100000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegABit5ShouldSetBitWhenUnset)
{
	cpu.setRegisterA(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_5_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00100000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegABit6ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterA(0b01000000);
	assertBitSetWasPerformed(extendedInstructions::SET_6_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegABit6ShouldSetBitWhenUnset)
{
	cpu.setRegisterA(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_6_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegABit7ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterA(0b10000000);
	assertBitSetWasPerformed(extendedInstructions::SET_7_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegABit7ShouldSetBitWhenUnset)
{
	cpu.setRegisterA(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_7_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegBBit0ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterB(0b00000001);
	assertBitSetWasPerformed(extendedInstructions::SET_0_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegBBit0ShouldSetBitWhenUnset)
{
	cpu.setRegisterB(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_0_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegBBit1ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterB(0b00000010);
	assertBitSetWasPerformed(extendedInstructions::SET_1_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegBBit1ShouldSetBitWhenUnset)
{
	cpu.setRegisterB(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_1_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegBBit2ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterB(0b00000100);
	assertBitSetWasPerformed(extendedInstructions::SET_2_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b00000100);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegBBit2ShouldSetBitWhenUnset)
{
	cpu.setRegisterB(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_2_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b00000100);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegBBit3ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterB(0b00001000);
	assertBitSetWasPerformed(extendedInstructions::SET_3_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b00001000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegBBit3ShouldSetBitWhenUnset)
{
	cpu.setRegisterB(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_3_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b00001000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegBBit4ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterB(0b00010000);
	assertBitSetWasPerformed(extendedInstructions::SET_4_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b00010000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegBBit4ShouldSetBitWhenUnset)
{
	cpu.setRegisterB(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_4_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b00010000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegBBit5ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterB(0b00100000);
	assertBitSetWasPerformed(extendedInstructions::SET_5_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b00100000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegBBit5ShouldSetBitWhenUnset)
{
	cpu.setRegisterB(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_5_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b00100000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegBBit6ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterB(0b01000000);
	assertBitSetWasPerformed(extendedInstructions::SET_6_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegBBit6ShouldSetBitWhenUnset)
{
	cpu.setRegisterB(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_6_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegBBit7ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterB(0b10000000);
	assertBitSetWasPerformed(extendedInstructions::SET_7_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegBBit7ShouldSetBitWhenUnset)
{
	cpu.setRegisterB(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_7_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegCBit0ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterC(0b00000001);
	assertBitSetWasPerformed(extendedInstructions::SET_0_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegCBit0ShouldSetBitWhenUnset)
{
	cpu.setRegisterC(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_0_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegCBit1ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterC(0b00000010);
	assertBitSetWasPerformed(extendedInstructions::SET_1_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegCBit1ShouldSetBitWhenUnset)
{
	cpu.setRegisterC(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_1_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegCBit2ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterC(0b00000100);
	assertBitSetWasPerformed(extendedInstructions::SET_2_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b00000100);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegCBit2ShouldSetBitWhenUnset)
{
	cpu.setRegisterC(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_2_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b00000100);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegCBit3ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterC(0b00001000);
	assertBitSetWasPerformed(extendedInstructions::SET_3_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b00001000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegCBit3ShouldSetBitWhenUnset)
{
	cpu.setRegisterC(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_3_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b00001000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegCBit4ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterC(0b00010000);
	assertBitSetWasPerformed(extendedInstructions::SET_4_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b00010000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegCBit4ShouldSetBitWhenUnset)
{
	cpu.setRegisterC(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_4_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b00010000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegCBit5ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterC(0b00100000);
	assertBitSetWasPerformed(extendedInstructions::SET_5_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b00100000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegCBit5ShouldSetBitWhenUnset)
{
	cpu.setRegisterC(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_5_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b00100000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegCBit6ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterC(0b01000000);
	assertBitSetWasPerformed(extendedInstructions::SET_6_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegCBit6ShouldSetBitWhenUnset)
{
	cpu.setRegisterC(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_6_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegCBit7ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterC(0b10000000);
	assertBitSetWasPerformed(extendedInstructions::SET_7_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegCBit7ShouldSetBitWhenUnset)
{
	cpu.setRegisterC(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_7_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegDBit0ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterD(0b00000001);
	assertBitSetWasPerformed(extendedInstructions::SET_0_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegDBit0ShouldSetBitWhenUnset)
{
	cpu.setRegisterD(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_0_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegDBit1ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterD(0b00000010);
	assertBitSetWasPerformed(extendedInstructions::SET_1_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegDBit1ShouldSetBitWhenUnset)
{
	cpu.setRegisterD(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_1_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegDBit2ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterD(0b00000100);
	assertBitSetWasPerformed(extendedInstructions::SET_2_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b00000100);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegDBit2ShouldSetBitWhenUnset)
{
	cpu.setRegisterD(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_2_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b00000100);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegDBit3ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterD(0b00001000);
	assertBitSetWasPerformed(extendedInstructions::SET_3_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b00001000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegDBit3ShouldSetBitWhenUnset)
{
	cpu.setRegisterD(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_3_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b00001000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegDBit4ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterD(0b00010000);
	assertBitSetWasPerformed(extendedInstructions::SET_4_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b00010000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegDBit4ShouldSetBitWhenUnset)
{
	cpu.setRegisterD(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_4_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b00010000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegDBit5ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterD(0b00100000);
	assertBitSetWasPerformed(extendedInstructions::SET_5_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b00100000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegDBit5ShouldSetBitWhenUnset)
{
	cpu.setRegisterD(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_5_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b00100000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegDBit6ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterD(0b01000000);
	assertBitSetWasPerformed(extendedInstructions::SET_6_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegDBit6ShouldSetBitWhenUnset)
{
	cpu.setRegisterD(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_6_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegDBit7ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterD(0b10000000);
	assertBitSetWasPerformed(extendedInstructions::SET_7_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegDBit7ShouldSetBitWhenUnset)
{
	cpu.setRegisterD(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_7_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegEBit0ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterE(0b00000001);
	assertBitSetWasPerformed(extendedInstructions::SET_0_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegEBit0ShouldSetBitWhenUnset)
{
	cpu.setRegisterE(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_0_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegEBit1ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterE(0b00000010);
	assertBitSetWasPerformed(extendedInstructions::SET_1_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegEBit1ShouldSetBitWhenUnset)
{
	cpu.setRegisterE(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_1_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegEBit2ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterE(0b00000100);
	assertBitSetWasPerformed(extendedInstructions::SET_2_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b00000100);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegEBit2ShouldSetBitWhenUnset)
{
	cpu.setRegisterE(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_2_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b00000100);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegEBit3ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterE(0b00001000);
	assertBitSetWasPerformed(extendedInstructions::SET_3_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b00001000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegEBit3ShouldSetBitWhenUnset)
{
	cpu.setRegisterE(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_3_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b00001000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegEBit4ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterE(0b00010000);
	assertBitSetWasPerformed(extendedInstructions::SET_4_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b00010000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegEBit4ShouldSetBitWhenUnset)
{
	cpu.setRegisterE(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_4_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b00010000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegEBit5ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterE(0b00100000);
	assertBitSetWasPerformed(extendedInstructions::SET_5_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b00100000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegEBit5ShouldSetBitWhenUnset)
{
	cpu.setRegisterE(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_5_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b00100000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegEBit6ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterE(0b01000000);
	assertBitSetWasPerformed(extendedInstructions::SET_6_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegEBit6ShouldSetBitWhenUnset)
{
	cpu.setRegisterE(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_6_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegEBit7ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterE(0b10000000);
	assertBitSetWasPerformed(extendedInstructions::SET_7_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegEBit7ShouldSetBitWhenUnset)
{
	cpu.setRegisterE(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_7_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegHBit0ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterH(0b00000001);
	assertBitSetWasPerformed(extendedInstructions::SET_0_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegHBit0ShouldSetBitWhenUnset)
{
	cpu.setRegisterH(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_0_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegHBit1ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterH(0b00000010);
	assertBitSetWasPerformed(extendedInstructions::SET_1_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegHBit1ShouldSetBitWhenUnset)
{
	cpu.setRegisterH(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_1_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegHBit2ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterH(0b00000100);
	assertBitSetWasPerformed(extendedInstructions::SET_2_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b00000100);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegHBit2ShouldSetBitWhenUnset)
{
	cpu.setRegisterH(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_2_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b00000100);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegHBit3ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterH(0b00001000);
	assertBitSetWasPerformed(extendedInstructions::SET_3_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b00001000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegHBit3ShouldSetBitWhenUnset)
{
	cpu.setRegisterH(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_3_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b00001000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegHBit4ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterH(0b00010000);
	assertBitSetWasPerformed(extendedInstructions::SET_4_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b00010000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegHBit4ShouldSetBitWhenUnset)
{
	cpu.setRegisterH(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_4_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b00010000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegHBit5ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterH(0b00100000);
	assertBitSetWasPerformed(extendedInstructions::SET_5_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b00100000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegHBit5ShouldSetBitWhenUnset)
{
	cpu.setRegisterH(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_5_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b00100000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegHBit6ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterH(0b01000000);
	assertBitSetWasPerformed(extendedInstructions::SET_6_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegHBit6ShouldSetBitWhenUnset)
{
	cpu.setRegisterH(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_6_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegHBit7ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterH(0b10000000);
	assertBitSetWasPerformed(extendedInstructions::SET_7_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegHBit7ShouldSetBitWhenUnset)
{
	cpu.setRegisterH(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_7_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegLBit0ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterL(0b00000001);
	assertBitSetWasPerformed(extendedInstructions::SET_0_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegLBit0ShouldSetBitWhenUnset)
{
	cpu.setRegisterL(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_0_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegLBit1ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterL(0b00000010);
	assertBitSetWasPerformed(extendedInstructions::SET_1_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegLBit1ShouldSetBitWhenUnset)
{
	cpu.setRegisterL(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_1_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegLBit2ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterL(0b00000100);
	assertBitSetWasPerformed(extendedInstructions::SET_2_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b00000100);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegLBit2ShouldSetBitWhenUnset)
{
	cpu.setRegisterL(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_2_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b00000100);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegLBit3ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterL(0b00001000);
	assertBitSetWasPerformed(extendedInstructions::SET_3_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b00001000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegLBit3ShouldSetBitWhenUnset)
{
	cpu.setRegisterL(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_3_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b00001000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegLBit4ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterL(0b00010000);
	assertBitSetWasPerformed(extendedInstructions::SET_4_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b00010000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegLBit4ShouldSetBitWhenUnset)
{
	cpu.setRegisterL(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_4_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b00010000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegLBit5ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterL(0b00100000);
	assertBitSetWasPerformed(extendedInstructions::SET_5_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b00100000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegLBit5ShouldSetBitWhenUnset)
{
	cpu.setRegisterL(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_5_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b00100000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegLBit6ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterL(0b01000000);
	assertBitSetWasPerformed(extendedInstructions::SET_6_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegLBit6ShouldSetBitWhenUnset)
{
	cpu.setRegisterL(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_6_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegLBit7ShouldNotChangeBitWhenSet)
{
	cpu.setRegisterL(0b10000000);
	assertBitSetWasPerformed(extendedInstructions::SET_7_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForRegLBit7ShouldSetBitWhenUnset)
{
	cpu.setRegisterL(0b00000000);
	assertBitSetWasPerformed(extendedInstructions::SET_7_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForMemoryHLBit0ShouldNotChangeBitWhenSet)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000001);
	assertBitSetInMemoryWasPerformed(extendedInstructions::SET_0_HLm);
	ASSERT_EQ(mmu.read(addr), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForMemoryHLBit0ShouldSetBitWhenUnset)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000000);
	assertBitSetInMemoryWasPerformed(extendedInstructions::SET_0_HLm);
	ASSERT_EQ(mmu.read(addr), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForMemoryHLBit1ShouldNotChangeBitWhenSet)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000010);
	assertBitSetInMemoryWasPerformed(extendedInstructions::SET_1_HLm);
	ASSERT_EQ(mmu.read(addr), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForMemoryHLBit1ShouldSetBitWhenUnset)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000000);
	assertBitSetInMemoryWasPerformed(extendedInstructions::SET_1_HLm);
	ASSERT_EQ(mmu.read(addr), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForMemoryHLBit2ShouldNotChangeBitWhenSet)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000100);
	assertBitSetInMemoryWasPerformed(extendedInstructions::SET_2_HLm);
	ASSERT_EQ(mmu.read(addr), 0b00000100);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForMemoryHLBit2ShouldSetBitWhenUnset)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000000);
	assertBitSetInMemoryWasPerformed(extendedInstructions::SET_2_HLm);
	ASSERT_EQ(mmu.read(addr), 0b00000100);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForMemoryHLBit3ShouldNotChangeBitWhenSet)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00001000);
	assertBitSetInMemoryWasPerformed(extendedInstructions::SET_3_HLm);
	ASSERT_EQ(mmu.read(addr), 0b00001000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForMemoryHLBit3ShouldSetBitWhenUnset)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000000);
	assertBitSetInMemoryWasPerformed(extendedInstructions::SET_3_HLm);
	ASSERT_EQ(mmu.read(addr), 0b00001000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForMemoryHLBit4ShouldNotChangeBitWhenSet)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00010000);
	assertBitSetInMemoryWasPerformed(extendedInstructions::SET_4_HLm);
	ASSERT_EQ(mmu.read(addr), 0b00010000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForMemoryHLBit4ShouldSetBitWhenUnset)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000000);
	assertBitSetInMemoryWasPerformed(extendedInstructions::SET_4_HLm);
	ASSERT_EQ(mmu.read(addr), 0b00010000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForMemoryHLBit5ShouldNotChangeBitWhenSet)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00100000);
	assertBitSetInMemoryWasPerformed(extendedInstructions::SET_5_HLm);
	ASSERT_EQ(mmu.read(addr), 0b00100000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForMemoryHLBit5ShouldSetBitWhenUnset)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000000);
	assertBitSetInMemoryWasPerformed(extendedInstructions::SET_5_HLm);
	ASSERT_EQ(mmu.read(addr), 0b00100000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForMemoryHLBit6ShouldNotChangeBitWhenSet)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b01000000);
	assertBitSetInMemoryWasPerformed(extendedInstructions::SET_6_HLm);
	ASSERT_EQ(mmu.read(addr), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForMemoryHLBit6ShouldSetBitWhenUnset)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000000);
	assertBitSetInMemoryWasPerformed(extendedInstructions::SET_6_HLm);
	ASSERT_EQ(mmu.read(addr), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForMemoryHLBit7ShouldNotChangeBitWhenSet)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b10000000);
	assertBitSetInMemoryWasPerformed(extendedInstructions::SET_7_HLm);
	ASSERT_EQ(mmu.read(addr), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, BitSetForMemoryHLBit7ShouldSetBitWhenUnset)
{
	uint16_t addr = 0x1234;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, 0b00000000);
	assertBitSetInMemoryWasPerformed(extendedInstructions::SET_7_HLm);
	ASSERT_EQ(mmu.read(addr), 0b10000000);
}
#pragma endregion

#pragma region Left Circular Rotations
TEST_F(CpuInstructions8BitsRotationShiftsBitTest, RotateLeftCircularRegisterAForValue0ShouldNotChangeValue)
{
	cpu.setRegisterA(0x00);
	setExpectedTicks(1);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertStandardInstructionWasExecutedCorrectly(standardInstructions::RLC_A);
	ASSERT_EQ(cpu.getRegisterA(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, RotateLeftCircularRegisterAForValue1ShouldGive2)
{
	cpu.setRegisterA(0b00000001);
	setExpectedTicks(1);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertStandardInstructionWasExecutedCorrectly(standardInstructions::RLC_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, RotateLeftCircularRegisterAForMaxBitShouldSetCarryAndLoopAround)
{
	cpu.setRegisterA(0b10000000);
	setExpectedTicks(1);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertStandardInstructionWasExecutedCorrectly(standardInstructions::RLC_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftCircularRegisterAForValue0ShouldNotChangeValue)
{
	cpu.setRegisterA(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_A);
	ASSERT_EQ(cpu.getRegisterA(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftCircularRegisterAForValue1ShouldGive2)
{
	cpu.setRegisterA(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest,
       ExtendedRotateLeftCircularRegisterAForMaxBitShouldSetCarryAndLoopAround)
{
	cpu.setRegisterA(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftCircularRegisterBForValue0ShouldNotChangeValue)
{
	cpu.setRegisterB(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_B);
	ASSERT_EQ(cpu.getRegisterB(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftCircularRegisterBForValue1ShouldGive2)
{
	cpu.setRegisterB(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest,
       ExtendedRotateLeftCircularRegisterBForMaxBitShouldSetCarryAndLoopAround)
{
	cpu.setRegisterB(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftCircularRegisterCForValue0ShouldNotChangeValue)
{
	cpu.setRegisterC(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_C);
	ASSERT_EQ(cpu.getRegisterC(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftCircularRegisterCForValue1ShouldGive2)
{
	cpu.setRegisterC(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest,
       ExtendedRotateLeftCircularRegisterCForMaxBitShouldSetCarryAndLoopAround)
{
	cpu.setRegisterC(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftCircularRegisterDForValue0ShouldNotChangeValue)
{
	cpu.setRegisterD(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_D);
	ASSERT_EQ(cpu.getRegisterD(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftCircularRegisterDForValue1ShouldGive2)
{
	cpu.setRegisterD(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest,
       ExtendedRotateLeftCircularRegisterDForMaxBitShouldSetCarryAndLoopAround)
{
	cpu.setRegisterD(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftCircularRegisterEForValue0ShouldNotChangeValue)
{
	cpu.setRegisterE(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_E);
	ASSERT_EQ(cpu.getRegisterE(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftCircularRegisterEForValue1ShouldGive2)
{
	cpu.setRegisterE(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest,
       ExtendedRotateLeftCircularRegisterEForMaxBitShouldSetCarryAndLoopAround)
{
	cpu.setRegisterE(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftCircularRegisterHForValue0ShouldNotChangeValue)
{
	cpu.setRegisterH(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_H);
	ASSERT_EQ(cpu.getRegisterH(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftCircularRegisterHForValue1ShouldGive2)
{
	cpu.setRegisterH(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest,
       ExtendedRotateLeftCircularRegisterHForMaxBitShouldSetCarryAndLoopAround)
{
	cpu.setRegisterH(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftCircularRegisterLForValue0ShouldNotChangeValue)
{
	cpu.setRegisterL(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_L);
	ASSERT_EQ(cpu.getRegisterL(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftCircularRegisterLForValue1ShouldGive2)
{
	cpu.setRegisterL(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest,
       ExtendedRotateLeftCircularRegisterLForMaxBitShouldSetCarryAndLoopAround)
{
	cpu.setRegisterL(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b00000001);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftCircularInMemoryForValue0ShouldNotChangeValue)
{
	byte startValue = 0x00;
	byte expectedValue = 0x00;
	uint16_t addr = 0x1234;
	mmu.write(addr, startValue);
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	setExpectedTicks(4);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_HLm);
	ASSERT_EQ(mmu.read(addr), expectedValue);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftCircularInMemoryForValue1ShouldGive2)
{
	byte startValue = 0b00000001;
	byte expectedValue = 0b00000010;
	uint16_t addr = 0x1234;
	mmu.write(addr, startValue);
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	setExpectedTicks(4);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_HLm);
	ASSERT_EQ(mmu.read(addr), expectedValue);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest,
       ExtendedRotateLeftCircularInMemoryForMaxBitShouldSetCarryAndLoopAround)
{
	byte startValue = 0b10000000;
	byte expectedValue = 0b00000001;
	uint16_t addr = 0x1234;
	mmu.write(addr, startValue);
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	setExpectedTicks(4);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RLC_HLm);
	ASSERT_EQ(mmu.read(addr), expectedValue);
}
#pragma endregion

#pragma region Left Rotations
TEST_F(CpuInstructions8BitsRotationShiftsBitTest, RotateLeftRegisterAForValue0ShouldNotChangeValue)
{
	cpu.setRegisterA(0x00);
	setExpectedTicks(1);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertStandardInstructionWasExecutedCorrectly(standardInstructions::RL_A);
	ASSERT_EQ(cpu.getRegisterA(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, RotateLeftRegisterAForValue1ShouldGive2)
{
	cpu.setRegisterA(0b00000001);
	setExpectedTicks(1);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertStandardInstructionWasExecutedCorrectly(standardInstructions::RL_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, RotateLeftRegisterAForMaxBitShouldSetCarry)
{
	cpu.setRegisterA(0b10000000);
	setExpectedTicks(1);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertStandardInstructionWasExecutedCorrectly(standardInstructions::RL_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterAForValue0ShouldNotChangeValue)
{
	cpu.setRegisterA(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_A);
	ASSERT_EQ(cpu.getRegisterA(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterAForValue1ShouldGive2)
{
	cpu.setRegisterA(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterAForMaxBitShouldSetCarry)
{
	cpu.setRegisterA(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY | CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterBForValue0ShouldNotChangeValue)
{
	cpu.setRegisterB(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_B);
	ASSERT_EQ(cpu.getRegisterB(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterBForValue1ShouldGive2)
{
	cpu.setRegisterB(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterBForMaxBitShouldSetCarry)
{
	cpu.setRegisterB(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY | CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b00000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterCForValue0ShouldNotChangeValue)
{
	cpu.setRegisterC(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_C);
	ASSERT_EQ(cpu.getRegisterC(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterCForValue1ShouldGive2)
{
	cpu.setRegisterC(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterCForMaxBitShouldSetCarry)
{
	cpu.setRegisterC(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY | CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b00000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterDForValue0ShouldNotChangeValue)
{
	cpu.setRegisterD(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_D);
	ASSERT_EQ(cpu.getRegisterD(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterDForValue1ShouldGive2)
{
	cpu.setRegisterD(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterDForMaxBitShouldSetCarry)
{
	cpu.setRegisterD(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY | CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b00000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterEForValue0ShouldNotChangeValue)
{
	cpu.setRegisterE(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_E);
	ASSERT_EQ(cpu.getRegisterE(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterEForValue1ShouldGive2)
{
	cpu.setRegisterE(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterEForMaxBitShouldSetCarry)
{
	cpu.setRegisterE(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY | CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b00000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterHForValue0ShouldNotChangeValue)
{
	cpu.setRegisterH(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_H);
	ASSERT_EQ(cpu.getRegisterH(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterHForValue1ShouldGive2)
{
	cpu.setRegisterH(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterHForMaxBitShouldSetCarry)
{
	cpu.setRegisterH(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY | CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b00000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterLForValue0ShouldNotChangeValue)
{
	cpu.setRegisterL(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_L);
	ASSERT_EQ(cpu.getRegisterL(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterLForValue1ShouldGive2)
{
	cpu.setRegisterL(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b00000010);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftRegisterLForMaxBitShouldSetCarry)
{
	cpu.setRegisterL(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY | CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b00000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftInMemoryForValue0ShouldNotChangeValue)
{
	byte startValue = 0x00;
	byte expectedValue = 0x00;
	uint16_t addr = 0x1234;
	mmu.write(addr, startValue);
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	setExpectedTicks(4);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_HLm);
	ASSERT_EQ(mmu.read(addr), expectedValue);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftInMemoryForValue1ShouldGive2)
{
	byte startValue = 0b00000001;
	byte expectedValue = 0b00000010;
	uint16_t addr = 0x1234;
	mmu.write(addr, startValue);
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	setExpectedTicks(4);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_HLm);
	ASSERT_EQ(mmu.read(addr), expectedValue);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateLeftInMemoryForMaxBitShouldSetCarry)
{
	byte startValue = 0b10000000;
	byte expectedValue = 0b00000000;
	uint16_t addr = 0x1234;
	mmu.write(addr, startValue);
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	setExpectedTicks(4);
	setExpectedFlags(CPU::CpuFlags::CARRY | CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RL_HLm);
	ASSERT_EQ(mmu.read(addr), expectedValue);
}
#pragma endregion

#pragma region Right Circular Rotations
TEST_F(CpuInstructions8BitsRotationShiftsBitTest, RotateRightCircularRegisterAForValue0ShouldNotChangeValue)
{
	cpu.setRegisterA(0x00);
	setExpectedTicks(1);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertStandardInstructionWasExecutedCorrectly(standardInstructions::RRC_A);
	ASSERT_EQ(cpu.getRegisterA(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, RotateRightCircularRegisterAForValueMaxBitShouldShiftProperly)
{
	cpu.setRegisterA(0b10000000);
	setExpectedTicks(1);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertStandardInstructionWasExecutedCorrectly(standardInstructions::RRC_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, RotateRightCircularRegisterAForMinBitShouldSetCarryAndLoopAround)
{
	cpu.setRegisterA(0b00000001);
	setExpectedTicks(1);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertStandardInstructionWasExecutedCorrectly(standardInstructions::RRC_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightCircularRegisterAForValue0ShouldNotChangeValue)
{
	cpu.setRegisterA(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_A);
	ASSERT_EQ(cpu.getRegisterA(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightCircularRegisterAForValueMaxBitShouldShiftProperly)
{
	cpu.setRegisterA(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest,
       ExtendedRotateRightCircularRegisterAForMinBitShouldSetCarryAndLoopAround)
{
	cpu.setRegisterA(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightCircularRegisterBForValue0ShouldNotChangeValue)
{
	cpu.setRegisterB(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_B);
	ASSERT_EQ(cpu.getRegisterB(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightCircularRegisterBForValueMaxBitShouldShiftProperly)
{
	cpu.setRegisterB(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest,
       ExtendedRotateRightCircularRegisterBForMinBitShouldSetCarryAndLoopAround)
{
	cpu.setRegisterB(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightCircularRegisterCForValue0ShouldNotChangeValue)
{
	cpu.setRegisterC(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_C);
	ASSERT_EQ(cpu.getRegisterC(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightCircularRegisterCForValueMaxBitShouldShiftProperly)
{
	cpu.setRegisterC(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest,
       ExtendedRotateRightCircularRegisterCForMinBitShouldSetCarryAndLoopAround)
{
	cpu.setRegisterC(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightCircularRegisterDForValue0ShouldNotChangeValue)
{
	cpu.setRegisterD(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_D);
	ASSERT_EQ(cpu.getRegisterD(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightCircularRegisterDForValueMaxBitShouldShiftProperly)
{
	cpu.setRegisterD(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest,
       ExtendedRotateRightCircularRegisterDForMinBitShouldSetCarryAndLoopAround)
{
	cpu.setRegisterD(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightCircularRegisterEForValue0ShouldNotChangeValue)
{
	cpu.setRegisterE(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_E);
	ASSERT_EQ(cpu.getRegisterE(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightCircularRegisterEForValueMaxBitShouldShiftProperly)
{
	cpu.setRegisterE(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest,
       ExtendedRotateRightCircularRegisterEForMinBitShouldSetCarryAndLoopAround)
{
	cpu.setRegisterE(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightCircularRegisterHForValue0ShouldNotChangeValue)
{
	cpu.setRegisterH(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_H);
	ASSERT_EQ(cpu.getRegisterH(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightCircularRegisterHForValueMaxBitShouldShiftProperly)
{
	cpu.setRegisterH(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest,
       ExtendedRotateRightCircularRegisterHForMinBitShouldSetCarryAndLoopAround)
{
	cpu.setRegisterH(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightCircularRegisterLForValue0ShouldNotChangeValue)
{
	cpu.setRegisterL(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_L);
	ASSERT_EQ(cpu.getRegisterL(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightCircularRegisterLForValueMaxBitShouldShiftProperly)
{
	cpu.setRegisterL(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest,
       ExtendedRotateRightCircularRegisterLForMinBitShouldSetCarryAndLoopAround)
{
	cpu.setRegisterL(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b10000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightCircularInMemoryForValue0ShouldNotChangeValue)
{
	byte startValue = 0x00;
	byte expectedValue = 0x00;
	uint16_t addr = 0x1234;
	mmu.write(addr, startValue);
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	setExpectedTicks(4);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_HLm);
	ASSERT_EQ(mmu.read(addr), expectedValue);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightCircularInMemoryForValueMaxBitShouldShiftProperly)
{
	byte startValue = 0b10000000;
	byte expectedValue = 0b01000000;
	uint16_t addr = 0x1234;
	mmu.write(addr, startValue);
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	setExpectedTicks(4);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_HLm);
	ASSERT_EQ(mmu.read(addr), expectedValue);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest,
       ExtendedRotateRightCircularInMemoryForMinBitShouldSetCarryAndLoopAround)
{
	byte startValue = 0b000000001;
	byte expectedValue = 0b10000000;
	uint16_t addr = 0x1234;
	mmu.write(addr, startValue);
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	setExpectedTicks(4);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RRC_HLm);
	ASSERT_EQ(mmu.read(addr), expectedValue);
}
#pragma endregion

#pragma region Right Rotations
TEST_F(CpuInstructions8BitsRotationShiftsBitTest, RotateRightRegisterAForValue0ShouldNotChangeValue)
{
	cpu.setRegisterA(0x00);
	setExpectedTicks(1);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertStandardInstructionWasExecutedCorrectly(standardInstructions::RR_A);
	ASSERT_EQ(cpu.getRegisterA(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, RotateRightRegisterAForValue1ShouldGive0AndSetCarry)
{
	cpu.setRegisterA(0b00000001);
	setExpectedTicks(1);
	setExpectedFlags(CPU::CpuFlags::CARRY);
	assertStandardInstructionWasExecutedCorrectly(standardInstructions::RR_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, RotateRightRegisterAForMaxBitShouldShiftProperly)
{
	cpu.setRegisterA(0b10000000);
	setExpectedTicks(1);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertStandardInstructionWasExecutedCorrectly(standardInstructions::RR_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterAForValue0ShouldNotChangeValue)
{
	cpu.setRegisterA(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_A);
	ASSERT_EQ(cpu.getRegisterA(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterAForValue1ShouldGive0AndSetCarry)
{
	cpu.setRegisterA(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO | CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b00000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterAForMaxBitShouldShiftProperly)
{
	cpu.setRegisterA(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_A);
	ASSERT_EQ(cpu.getRegisterA(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterBForValue0ShouldNotChangeValue)
{
	cpu.setRegisterB(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_B);
	ASSERT_EQ(cpu.getRegisterB(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterBForValue1ShouldGive0AndSetCarry)
{
	cpu.setRegisterB(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO | CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b00000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterBForMaxBitShouldShiftProperly)
{
	cpu.setRegisterB(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_B);
	ASSERT_EQ(cpu.getRegisterB(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterCForValue0ShouldNotChangeValue)
{
	cpu.setRegisterC(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_C);
	ASSERT_EQ(cpu.getRegisterC(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterCForValue1ShouldGive0AndSetCarry)
{
	cpu.setRegisterC(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO | CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b00000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterCForMaxBitShouldShiftProperly)
{
	cpu.setRegisterC(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_C);
	ASSERT_EQ(cpu.getRegisterC(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterDForValue0ShouldNotChangeValue)
{
	cpu.setRegisterD(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_D);
	ASSERT_EQ(cpu.getRegisterD(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterDForValue1ShouldGive0AndSetCarry)
{
	cpu.setRegisterD(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO | CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b00000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterDForMaxBitShouldShiftProperly)
{
	cpu.setRegisterD(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_D);
	ASSERT_EQ(cpu.getRegisterD(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterEForValue0ShouldNotChangeValue)
{
	cpu.setRegisterE(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_E);
	ASSERT_EQ(cpu.getRegisterE(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterEForValue1ShouldGive0AndSetCarry)
{
	cpu.setRegisterE(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO | CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b00000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterEForMaxBitShouldShiftProperly)
{
	cpu.setRegisterE(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_E);
	ASSERT_EQ(cpu.getRegisterE(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterHForValue0ShouldNotChangeValue)
{
	cpu.setRegisterH(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_H);
	ASSERT_EQ(cpu.getRegisterH(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterHForValue1ShouldGive0AndSetCarry)
{
	cpu.setRegisterH(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO | CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b00000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterHForMaxBitShouldShiftProperly)
{
	cpu.setRegisterH(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_H);
	ASSERT_EQ(cpu.getRegisterH(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterLForValue0ShouldNotChangeValue)
{
	cpu.setRegisterL(0x00);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_L);
	ASSERT_EQ(cpu.getRegisterL(), 0x00);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterLForValue1ShouldGive0AndSetCarry)
{
	cpu.setRegisterL(0b00000001);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::ZERO | CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b00000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightRegisterLForMaxBitShouldShiftProperly)
{
	cpu.setRegisterL(0b10000000);
	setExpectedTicks(2);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_L);
	ASSERT_EQ(cpu.getRegisterL(), 0b01000000);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightInMemoryForValue0ShouldNotChangeValue)
{
	byte startValue = 0x00;
	byte expectedValue = 0x00;
	uint16_t addr = 0x1234;
	mmu.write(addr, startValue);
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	setExpectedTicks(4);
	setExpectedFlags(CPU::CpuFlags::ZERO);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_HLm);
	ASSERT_EQ(mmu.read(addr), expectedValue);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightInMemoryForValue1ShouldGive0AndSetCarry)
{
	byte startValue = 0b00000001;
	byte expectedValue = 0b00000000;
	uint16_t addr = 0x1234;
	mmu.write(addr, startValue);
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	setExpectedTicks(4);
	setExpectedFlags(CPU::CpuFlags::ZERO | CPU::CpuFlags::CARRY);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_HLm);
	ASSERT_EQ(mmu.read(addr), expectedValue);
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ExtendedRotateRightInMemoryForMaxBitShouldShiftProperly)
{
	byte startValue = 0b10000000;
	byte expectedValue = 0b01000000;
	uint16_t addr = 0x1234;
	mmu.write(addr, startValue);
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	setExpectedTicks(4);
	setExpectedFlags(CPU::CpuFlags::NONE);
	assertExtendedInstructionWasExecutedCorrectly(extendedInstructions::RR_HLm);
	ASSERT_EQ(mmu.read(addr), expectedValue);
}
#pragma endregion