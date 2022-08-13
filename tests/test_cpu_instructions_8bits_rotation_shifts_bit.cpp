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