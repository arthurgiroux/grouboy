#include "cpu.hpp"
#include "instructions.hpp"
#include "utils.hpp"

#include <gtest/gtest.h>

using namespace utils;

class CpuInstructions8BitsArithmeticLogicalTest : public ::testing::Test
{
  protected:
	void assertIncrementRegisterWasPerformed(byte instruction, byte expectedValue, int expectedFlag = 0)
	{
		mmu.write(cpu.getProgramCounter(), instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 1);
		ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
		ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), (expectedFlag & CPU::HALF_CARRY) > 0);
		ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), (expectedFlag & CPU::ZERO) > 0);
		ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
		ASSERT_EQ(cpu.getProgramCounter(), 1);
	}

	void assertDecrementRegisterWasPerformed(byte instruction, byte expectedValue, int expectedFlag = 0)
	{
		mmu.write(cpu.getProgramCounter(), instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 1);
		ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
        ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), (expectedFlag & CPU::HALF_CARRY) > 0);
        ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), (expectedFlag & CPU::ZERO) > 0);
		ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
		ASSERT_EQ(cpu.getProgramCounter(), 1);
	}

	void assertDecimalAdjustAccumulatorGivesExpectedResult(byte expectedRegisterValue, bool expectedCarryFlag)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::DAA);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 1);
		ASSERT_EQ(cpu.isFlagSet(CPU::ZERO), cpu.getRegisterA() == 0);
		ASSERT_EQ(cpu.getProgramCounter(), 1);
		ASSERT_EQ(cpu.a, expectedRegisterValue);
		ASSERT_EQ(cpu.isFlagSet(CPU::CARRY), expectedCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::HALF_CARRY), false);
	}

	void assertComplementRegisterGivesExpectedResult(byte expectedValue)
	{
		mmu.write(cpu.getProgramCounter(), standardInstructions::CPL);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 1);
		ASSERT_TRUE(cpu.isFlagSet(CPU::HALF_CARRY));
		ASSERT_TRUE(cpu.isFlagSet(CPU::SUBSTRACTION));
		ASSERT_EQ(cpu.getRegisterA(), expectedValue);
		ASSERT_EQ(cpu.getProgramCounter(), 1);
	}

	MMU mmu;
	CPU cpu = CPU(mmu);
};

#pragma region Increment Registers
TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterAFrom0ShouldGive1)
{
	byte startValue = 0x00;
	byte expected = 0x01;
	cpu.setRegisterA(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_A, expected);
	ASSERT_EQ(cpu.getRegisterA(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterAFromMaxShouldGive0)
{
	byte startValue = 0xFF;
	byte expected = 0x00;
	cpu.setRegisterA(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_A, expected, CPU::ZERO | CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterA(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterAFromMidShouldRaiseHalfCarry)
{
	byte startValue = 0x0F;
	byte expected = 0x10;
	cpu.setRegisterA(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_A, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterA(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterBFrom0ShouldGive1)
{
	byte startValue = 0x00;
	byte expected = 0x01;
	cpu.setRegisterB(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_B, expected);
	ASSERT_EQ(cpu.getRegisterB(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterBFromMaxShouldGive0)
{
	byte startValue = 0xFF;
	byte expected = 0x00;
	cpu.setRegisterB(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_B, expected, CPU::ZERO | CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterB(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterBFromMidShouldRaiseHalfCarry)
{
	byte startValue = 0x0F;
	byte expected = 0x10;
	cpu.setRegisterB(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_B, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterB(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterCFrom0ShouldGive1)
{
	byte startValue = 0x00;
	byte expected = 0x01;
	cpu.setRegisterC(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_C, expected);
	ASSERT_EQ(cpu.getRegisterC(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterCFromMaxShouldGive0)
{
	byte startValue = 0xFF;
	byte expected = 0x00;
	cpu.setRegisterC(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_C, expected, CPU::ZERO | CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterC(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterCFromMidShouldRaiseHalfCarry)
{
	byte startValue = 0x0F;
	byte expected = 0x10;
	cpu.setRegisterC(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_C, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterC(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterDFrom0ShouldGive1)
{
	byte startValue = 0x00;
	byte expected = 0x01;
	cpu.setRegisterD(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_D, expected);
	ASSERT_EQ(cpu.getRegisterD(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterDFromMaxShouldGive0)
{
	byte startValue = 0xFF;
	byte expected = 0x00;
	cpu.setRegisterD(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_D, expected, CPU::ZERO | CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterD(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterDFromMidShouldRaiseHalfCarry)
{
	byte startValue = 0x0F;
	byte expected = 0x10;
	cpu.setRegisterD(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_D, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterD(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterEFrom0ShouldGive1)
{
	byte startValue = 0x00;
	byte expected = 0x01;
	cpu.setRegisterE(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_E, expected);
	ASSERT_EQ(cpu.getRegisterE(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterEFromMaxShouldGive0)
{
	byte startValue = 0xFF;
	byte expected = 0x00;
	cpu.setRegisterE(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_E, expected, CPU::ZERO | CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterE(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterEFromMidShouldRaiseHalfCarry)
{
	byte startValue = 0x0F;
	byte expected = 0x10;
	cpu.setRegisterE(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_E, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterE(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterHFrom0ShouldGive1)
{
	byte startValue = 0x00;
	byte expected = 0x01;
	cpu.setRegisterH(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_H, expected);
	ASSERT_EQ(cpu.getRegisterH(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterHFromMaxShouldGive0)
{
	byte startValue = 0xFF;
	byte expected = 0x00;
	cpu.setRegisterH(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_H, expected, CPU::ZERO | CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterH(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterHFromMidShouldRaiseHalfCarry)
{
	byte startValue = 0x0F;
	byte expected = 0x10;
	cpu.setRegisterH(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_H, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterH(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterLFrom0ShouldGive1)
{
	byte startValue = 0x00;
	byte expected = 0x01;
	cpu.setRegisterL(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_L, expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterLFromMaxShouldGive0)
{
	byte startValue = 0xFF;
	byte expected = 0x00;
	cpu.setRegisterL(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_L, expected, CPU::ZERO | CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterL(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterLFromMidShouldRaiseHalfCarry)
{
	byte startValue = 0x0F;
	byte expected = 0x10;
	cpu.setRegisterL(startValue);
	assertIncrementRegisterWasPerformed(standardInstructions::INC_L, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterL(), expected);
}
#pragma endregion

#pragma region Decrement Registers
TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterAFromMaxShouldGiveMaxMinusOne)
{
	byte startValue = 0xFF;
	byte expected = 0xFE;
	cpu.setRegisterA(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_A, expected);
	ASSERT_EQ(cpu.getRegisterA(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterAFrom1ShouldGive0)
{
	byte startValue = 0x01;
	byte expected = 0x00;
	cpu.setRegisterA(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_A, expected, CPU::ZERO);
	ASSERT_EQ(cpu.getRegisterA(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterAFrom0ShouldGiveMax)
{
	byte startValue = 0x00;
	byte expected = 0xFF;
	cpu.setRegisterA(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_A, expected, CPU::HALF_CARRY);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterAFromMidShouldRaiseHalfCarry)
{
	byte startValue = 0x10;
	byte expected = 0x0F;
	cpu.setRegisterA(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_A, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterA(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterBFromMaxShouldGiveMaxMinusOne)
{
	byte startValue = 0xFF;
	byte expected = 0xFE;
	cpu.setRegisterB(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_B, expected);
	ASSERT_EQ(cpu.getRegisterB(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterBFrom1ShouldGive0)
{
	byte startValue = 0x01;
	byte expected = 0x00;
	cpu.setRegisterB(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_B, expected, CPU::ZERO);
	ASSERT_EQ(cpu.getRegisterB(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterBFrom0ShouldGiveMax)
{
	byte startValue = 0x00;
	byte expected = 0xFF;
	cpu.setRegisterB(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_B, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterB(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterBFromMidShouldRaiseHalfCarry)
{
	byte startValue = 0x10;
	byte expected = 0x0F;
	cpu.setRegisterB(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_B, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterB(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterCFromMaxShouldGiveMaxMinusOne)
{
	byte startValue = 0xFF;
	byte expected = 0xFE;
	cpu.setRegisterC(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_C, expected);
	ASSERT_EQ(cpu.getRegisterC(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterCFrom1ShouldGive0)
{
	byte startValue = 0x01;
	byte expected = 0x00;
	cpu.setRegisterC(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_C, expected, CPU::ZERO);
	ASSERT_EQ(cpu.getRegisterC(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterCFrom0ShouldGiveMax)
{
	byte startValue = 0x00;
	byte expected = 0xFF;
	cpu.setRegisterC(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_C, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterC(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterCFromMidShouldRaiseHalfCarry)
{
	byte startValue = 0x10;
	byte expected = 0x0F;
	cpu.setRegisterC(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_C, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterC(), expected);
	ASSERT_TRUE(cpu.isFlagSet(CPU::HALF_CARRY));
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterDFromMaxShouldGiveMaxMinusOne)
{
	byte startValue = 0xFF;
	byte expected = 0xFE;
	cpu.setRegisterD(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_D, expected);
	ASSERT_EQ(cpu.getRegisterD(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterDFrom1ShouldGive0)
{
	byte startValue = 0x01;
	byte expected = 0x00;
	cpu.setRegisterD(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_D, expected, CPU::ZERO);
	ASSERT_EQ(cpu.getRegisterD(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterDFrom0ShouldGiveMax)
{
	byte startValue = 0x00;
	byte expected = 0xFF;
	cpu.setRegisterD(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_D, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterD(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterDFromMidShouldRaiseHalfCarry)
{
	byte startValue = 0x10;
	byte expected = 0x0F;
	cpu.setRegisterD(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_D, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterD(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterEFromMaxShouldGiveMaxMinusOne)
{
	byte startValue = 0xFF;
	byte expected = 0xFE;
	cpu.setRegisterE(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_E, expected);
	ASSERT_EQ(cpu.getRegisterE(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterEFrom1ShouldGive0)
{
	byte startValue = 0x01;
	byte expected = 0x00;
	cpu.setRegisterE(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_E, expected, CPU::ZERO);
	ASSERT_EQ(cpu.getRegisterE(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterEFrom0ShouldGiveMax)
{
	byte startValue = 0x00;
	byte expected = 0xFF;
	cpu.setRegisterE(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_E, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterE(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterEFromMidShouldRaiseHalfCarry)
{
	byte startValue = 0x10;
	byte expected = 0x0F;
	cpu.setRegisterE(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_E, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterE(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterHFromMaxShouldGiveMaxMinusOne)
{
	byte startValue = 0xFF;
	byte expected = 0xFE;
	cpu.setRegisterH(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_H, expected);
	ASSERT_EQ(cpu.getRegisterH(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterHFrom1ShouldGive0)
{
	byte startValue = 0x01;
	byte expected = 0x00;
	cpu.setRegisterH(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_H, expected, CPU::ZERO);
	ASSERT_EQ(cpu.getRegisterH(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterHFrom0ShouldGiveMax)
{
	byte startValue = 0x00;
	byte expected = 0xFF;
	cpu.setRegisterH(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_H, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterH(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterHFromMidShouldRaiseHalfCarry)
{
	byte startValue = 0x10;
	byte expected = 0x0F;
	cpu.setRegisterH(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_H, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterH(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterLFromMaxShouldGiveMaxMinusOne)
{
	byte startValue = 0xFF;
	byte expected = 0xFE;
	cpu.setRegisterL(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_L, expected);
	ASSERT_EQ(cpu.getRegisterL(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterLFrom1ShouldGive0)
{
	byte startValue = 0x01;
	byte expected = 0x00;
	cpu.setRegisterL(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_L, expected, CPU::ZERO);
	ASSERT_EQ(cpu.getRegisterL(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterLFrom0ShouldGiveMax)
{
	byte startValue = 0x00;
	byte expected = 0xFF;
	cpu.setRegisterL(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_L, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterL(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterLFromMidShouldRaiseHalfCarry)
{
	byte startValue = 0x10;
	byte expected = 0x0F;
	cpu.setRegisterL(startValue);
	assertDecrementRegisterWasPerformed(standardInstructions::DEC_L, expected, CPU::HALF_CARRY);
	ASSERT_EQ(cpu.getRegisterL(), expected);
}
#pragma endregion

#pragma region Decimal Adjust Accumulator
TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAWithLSBValueBelow9AndWithoutFlagsShouldKeepValueUnchanged)
{
	cpu.setHalfCarryFlag(false);
	cpu.setCarryFlag(false);
	cpu.setRegisterA(0x00);
	assertDecimalAdjustAccumulatorGivesExpectedResult(0x00, false);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAWithLSBValue9AndWithoutFlagsShouldKeepValueUnchanged)
{
	cpu.setHalfCarryFlag(false);
	cpu.setCarryFlag(false);
	cpu.setRegisterA(0x09);
	assertDecimalAdjustAccumulatorGivesExpectedResult(0x09, false);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAWithMSBValue9AndWithoutFlagsShouldKeepValueUnchanged)
{
	cpu.setHalfCarryFlag(false);
	cpu.setCarryFlag(false);
	cpu.setRegisterA(0x90);
	assertDecimalAdjustAccumulatorGivesExpectedResult(0x90, false);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAWithLSBAndMSBValue9AndWithoutFlagsShouldKeepValueUnchanged)
{
	cpu.setHalfCarryFlag(false);
	cpu.setCarryFlag(false);
	cpu.setRegisterA(0x99);
	assertDecimalAdjustAccumulatorGivesExpectedResult(0x99, false);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAWithLSBValueAbove9AndWithoutFlagsShouldIncreaseValueBy6)
{
	cpu.setHalfCarryFlag(false);
	cpu.setCarryFlag(false);
	cpu.setRegisterA(0x0A);
	assertDecimalAdjustAccumulatorGivesExpectedResult(0x0A + 0x06, false);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAWithMaxLSBValueAndWithoutFlagsShouldIncreaseValueBy6)
{
	cpu.setHalfCarryFlag(false);
	cpu.setCarryFlag(false);
	cpu.setRegisterA(0x0F);
	assertDecimalAdjustAccumulatorGivesExpectedResult(0x0F + 0x06, false);
}

/*
 *     // After an addition, adjust if (half-)carry occurred or if result is out of bounds
	if (!isFlagSet(SUBSTRACTION)) {
        if (isFlagSet(CpuFlags::CARRY) || a > 0x99)
        {
            a += 0x60;
            setFlag(CpuFlags::CARRY);
        }
        if (isFlagSet(CpuFlags::HALF_CARRY) || ((a & 0x0F) > 9))
        {
            a += 0x06;
        }
	}
    // after a subtraction, only adjust if (half-)carry occurred
	else {
        if (isFlagSet(CpuFlags::CARRY))
        {
            a -= 0x60;
        }
        if (isFlagSet(CpuFlags::HALF_CARRY))
        {
            a -= 0x06;
        }
	}
 */
TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAAfterSubstractionWithoutCarryFlagShouldNotDecreaseValue)
{
	cpu.setFlag(CPU::CpuFlags::SUBSTRACTION);
    cpu.setCarryFlag(false);
    cpu.setRegisterA(0x00);
    assertDecimalAdjustAccumulatorGivesExpectedResult(0x00, false);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAAfterSubstractionWithCarryFlagShouldDecreaseValueBy60)
{
    cpu.setFlag(CPU::CpuFlags::SUBSTRACTION);
    cpu.setCarryFlag(true);
    cpu.setRegisterA(0x66);
    assertDecimalAdjustAccumulatorGivesExpectedResult(0x06, true);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAAfterSubstractionWithoutHalfCarryFlagShouldNotDecreaseValue)
{
    cpu.setFlag(CPU::CpuFlags::SUBSTRACTION);
    cpu.setHalfCarryFlag(false);
    cpu.setRegisterA(0x00);
    assertDecimalAdjustAccumulatorGivesExpectedResult(0x00, false);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAAfterSubstractionWithHalfCarryFlagShouldDecreaseValueBy6)
{
    cpu.setFlag(CPU::CpuFlags::SUBSTRACTION);
    cpu.setHalfCarryFlag(true);
    cpu.setRegisterA(0x66);
    assertDecimalAdjustAccumulatorGivesExpectedResult(0x60, false);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAAfterSubstractionWithCarryAndHalfCarryFlagShouldDecreaseValueBy66)
{
    cpu.setFlag(CPU::CpuFlags::SUBSTRACTION);
    cpu.setCarryFlag(true);
    cpu.setHalfCarryFlag(true);
    cpu.setRegisterA(0x66);
    assertDecimalAdjustAccumulatorGivesExpectedResult(0x00, true);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAWithMaxLSBAndMSBBelow9AndWithoutFlagsShouldIncreaseValueBy66)
{
	cpu.setHalfCarryFlag(false);
	cpu.setCarryFlag(false);
	cpu.setRegisterA(0x9F);
	assertDecimalAdjustAccumulatorGivesExpectedResult(0x05, true);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAWithHalfCarryFlagShouldIncreaseValueBy6)
{
	cpu.setHalfCarryFlag(true);
	cpu.setCarryFlag(false);
	cpu.setRegisterA(0x00);
	assertDecimalAdjustAccumulatorGivesExpectedResult(0x00 + 0x06, false);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAWithMsbLowerThan9AndHalfCarryFlagShouldIncreaseValueBy6)
{
	cpu.setHalfCarryFlag(true);
	cpu.setCarryFlag(false);
	cpu.setRegisterA(0x93);
	assertDecimalAdjustAccumulatorGivesExpectedResult(0x93 + 0x06, false);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest,
       DAAWithMsbHigherThan9AndLsbLowerThan9AndWithoutFlagsShouldIncreaseValueBy60AndSetCarry)
{
	cpu.setHalfCarryFlag(false);
	cpu.setCarryFlag(false);
	cpu.setRegisterA(0xA9);
	assertDecimalAdjustAccumulatorGivesExpectedResult(static_cast<byte>(0xA9 + 0x60), true);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest,
       DAAWithMaxMsbAndLsbLowerThan9AndWithoutFlagsShouldIncreaseValueBy60AndSetCarry)
{
	cpu.setHalfCarryFlag(false);
	cpu.setCarryFlag(false);
	cpu.setRegisterA(0xF9);
	assertDecimalAdjustAccumulatorGivesExpectedResult(static_cast<byte>(0xF9 + 0x60), true);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAWithMsbAndLsbHigherThan9ShouldIncreaseValueBy66AndSetCarry)
{
	cpu.setHalfCarryFlag(false);
	cpu.setCarryFlag(false);
	cpu.setRegisterA(0xAA);
	assertDecimalAdjustAccumulatorGivesExpectedResult(static_cast<byte>(0xAA + 0x66), true);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAWithMaxMsbAndLsbShouldIncreaseValueBy66AndSetCarry)
{
	cpu.setHalfCarryFlag(false);
	cpu.setCarryFlag(false);
	cpu.setRegisterA(0xFF);
	assertDecimalAdjustAccumulatorGivesExpectedResult(static_cast<byte>(0xFF + 0x66), true);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAWithMsbHigherThan9AndHalfCarryShouldIncreaseValueBy66AndSetCarry)
{
	cpu.setHalfCarryFlag(true);
	cpu.setCarryFlag(false);
	cpu.setRegisterA(0xA0);
	assertDecimalAdjustAccumulatorGivesExpectedResult(static_cast<byte>(0xA0 + 0x66), true);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAWithMaxMsbAndHalfCarryShouldIncreaseValueBy66AndSetCarry)
{
	cpu.setHalfCarryFlag(true);
	cpu.setCarryFlag(false);
	cpu.setRegisterA(0xF3);
	assertDecimalAdjustAccumulatorGivesExpectedResult(static_cast<byte>(0xF3 + 0x66), true);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAWithCarryFlagShouldIncreaseValueBy60AndSetCarry)
{
	cpu.setHalfCarryFlag(false);
	cpu.setCarryFlag(true);
	cpu.setRegisterA(0x00);
	assertDecimalAdjustAccumulatorGivesExpectedResult(static_cast<byte>(0x00 + 0x60), true);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DAAWithHalfCarryAndCarryFlagShouldIncreaseValueBy66AndSetCarry)
{
	cpu.setHalfCarryFlag(true);
	cpu.setCarryFlag(true);
	cpu.setRegisterA(0x00);
	assertDecimalAdjustAccumulatorGivesExpectedResult(static_cast<byte>(0x00 + 0x66), true);
}
#pragma endregion

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, InstructionSetCarryFlag)
{
	cpu.setFlag(CPU::HALF_CARRY);
	cpu.setFlag(CPU::SUBSTRACTION);
	mmu.write(cpu.getProgramCounter(), standardInstructions::SCF);
	int ticks = cpu.fetchDecodeAndExecute();
	ASSERT_EQ(ticks, 1);
	ASSERT_EQ(cpu.getFlag(), CPU::CARRY);
	ASSERT_EQ(cpu.getProgramCounter(), 1);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, InstructionInvertsCarryFlag)
{
	cpu.setFlag(CPU::HALF_CARRY);
	cpu.setFlag(CPU::SUBSTRACTION);
	mmu.write(cpu.getProgramCounter(), standardInstructions::CCF);
	int ticks = cpu.fetchDecodeAndExecute();
	ASSERT_EQ(ticks, 1);
	ASSERT_EQ(cpu.getFlag(), CPU::CARRY);
	ASSERT_EQ(cpu.getProgramCounter(), 1);
	mmu.write(cpu.getProgramCounter(), standardInstructions::CCF);
	ticks = cpu.fetchDecodeAndExecute();
	ASSERT_EQ(cpu.getFlag(), 0x00);
}

#pragma region Complement register

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, Complement0x00ShouldGive0xFF)
{
	cpu.setRegisterA(0x00);
	assertComplementRegisterGivesExpectedResult(0xFF);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, Complement0xFFShouldGive0x00)
{
	cpu.setRegisterA(0xFF);
	assertComplementRegisterGivesExpectedResult(0x00);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, Complement0x01ShouldGive0xFE)
{
	cpu.setRegisterA(0x01);
	assertComplementRegisterGivesExpectedResult(0xFE);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, Complement0x10ShouldGive0xEF)
{
	cpu.setRegisterA(0x10);
	assertComplementRegisterGivesExpectedResult(0xEF);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, Complement0x55ShouldGive0xAA)
{
	cpu.setRegisterA(0x55);
	assertComplementRegisterGivesExpectedResult(0xAA);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, Complement0xAAShouldGive0x55)
{
	cpu.setRegisterA(0xAA);
	assertComplementRegisterGivesExpectedResult(0x55);
}

#pragma endregion
