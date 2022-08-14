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

	MMU mmu;
	CPU cpu = CPU(mmu);
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