#include "common/utils.hpp"
#include "cpu/cpu.hpp"
#include "cpu/instructions.hpp"

#include <gtest/gtest.h>

using namespace utils;

class CpuInstructions8BitsArithmeticLogicalTest : public ::testing::Test
{
  protected:
    void assertIncrementRegisterWasPerformed(byte instruction, int expectedFlag = 0)
    {
        mmu.write(cpu.getProgramCounter(), instruction);
        int ticks = cpu.fetchDecodeAndExecute();
        ASSERT_EQ(ticks, expectedTicks);
        ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
        ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), (expectedFlag & CPU::HALF_CARRY) > 0);
        ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), (expectedFlag & CPU::ZERO) > 0);
        ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
        ASSERT_EQ(cpu.getProgramCounter(), 1);
    }

    void assertDecrementRegisterWasPerformed(byte instruction, int expectedFlag = 0)
    {
        mmu.write(cpu.getProgramCounter(), instruction);
        int ticks = cpu.fetchDecodeAndExecute();
        ASSERT_EQ(ticks, expectedTicks);
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
        ASSERT_EQ(ticks, expectedTicks);
        ASSERT_EQ(cpu.isFlagSet(CPU::ZERO), cpu.getRegisterA() == 0);
        ASSERT_EQ(cpu.getProgramCounter(), 1);
        ASSERT_EQ(cpu.getRegisterA(), expectedRegisterValue);
        ASSERT_EQ(cpu.isFlagSet(CPU::CARRY), expectedCarryFlag);
        ASSERT_EQ(cpu.isFlagSet(CPU::HALF_CARRY), false);
    }

    void assertComplementRegisterGivesExpectedResult(byte expectedValue)
    {
        mmu.write(cpu.getProgramCounter(), standardInstructions::CPL);
        int ticks = cpu.fetchDecodeAndExecute();
        ASSERT_EQ(ticks, expectedTicks);
        ASSERT_TRUE(cpu.isFlagSet(CPU::HALF_CARRY));
        ASSERT_TRUE(cpu.isFlagSet(CPU::SUBSTRACTION));
        ASSERT_EQ(cpu.getRegisterA(), expectedValue);
        ASSERT_EQ(cpu.getProgramCounter(), 1);
    }

    void assertCompareOperationSetFlagsCorrectly(byte instruction, int expectedFlags)
    {
        mmu.write(cpu.getProgramCounter(), instruction);
        int ticks = cpu.fetchDecodeAndExecute();
        ASSERT_EQ(ticks, 1);
        ASSERT_EQ(cpu.getFlag(), CPU::SUBSTRACTION | expectedFlags);
        ASSERT_EQ(cpu.getProgramCounter(), 1);
    }

    void assertCompareWithImmediateValueSetFlagsCorrectly(byte instruction, byte value, int expectedFlags)
    {
        mmu.write(cpu.getProgramCounter(), instruction);
        mmu.write(cpu.getProgramCounter() + 1, value);
        int ticks = cpu.fetchDecodeAndExecute();
        ASSERT_EQ(ticks, 2);
        ASSERT_EQ(cpu.getFlag(), CPU::SUBSTRACTION | expectedFlags);
        ASSERT_EQ(cpu.getProgramCounter(), 2);
    }

    void assertCompareWithMemorySetFlagsCorrectly(byte instruction, int expectedFlags)
    {
        mmu.write(cpu.getProgramCounter(), instruction);
        int ticks = cpu.fetchDecodeAndExecute();
        ASSERT_EQ(ticks, 2);
        ASSERT_EQ(cpu.getFlag(), CPU::SUBSTRACTION | expectedFlags);
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

    MMU mmu;
    CPU cpu = CPU(mmu);
    int expectedTicks = 1;
    int expectedFlags = CPU::CpuFlags::NONE;
};

#pragma region Increment Registers
TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterAFrom0ShouldGive1)
{
    byte startValue = 0x00;
    byte expected = 0x01;
    cpu.setRegisterA(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_A);
    ASSERT_EQ(cpu.getRegisterA(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterAFromMaxShouldGive0)
{
    byte startValue = 0xFF;
    byte expected = 0x00;
    cpu.setRegisterA(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_A, CPU::ZERO | CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterA(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterAFromMidShouldRaiseHalfCarry)
{
    byte startValue = 0x0F;
    byte expected = 0x10;
    cpu.setRegisterA(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_A, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterA(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterBFrom0ShouldGive1)
{
    byte startValue = 0x00;
    byte expected = 0x01;
    cpu.setRegisterB(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_B);
    ASSERT_EQ(cpu.getRegisterB(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterBFromMaxShouldGive0)
{
    byte startValue = 0xFF;
    byte expected = 0x00;
    cpu.setRegisterB(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_B, CPU::ZERO | CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterB(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterBFromMidShouldRaiseHalfCarry)
{
    byte startValue = 0x0F;
    byte expected = 0x10;
    cpu.setRegisterB(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_B, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterB(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterCFrom0ShouldGive1)
{
    byte startValue = 0x00;
    byte expected = 0x01;
    cpu.setRegisterC(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_C);
    ASSERT_EQ(cpu.getRegisterC(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterCFromMaxShouldGive0)
{
    byte startValue = 0xFF;
    byte expected = 0x00;
    cpu.setRegisterC(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_C, CPU::ZERO | CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterC(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterCFromMidShouldRaiseHalfCarry)
{
    byte startValue = 0x0F;
    byte expected = 0x10;
    cpu.setRegisterC(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_C, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterC(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterDFrom0ShouldGive1)
{
    byte startValue = 0x00;
    byte expected = 0x01;
    cpu.setRegisterD(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_D);
    ASSERT_EQ(cpu.getRegisterD(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterDFromMaxShouldGive0)
{
    byte startValue = 0xFF;
    byte expected = 0x00;
    cpu.setRegisterD(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_D, CPU::ZERO | CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterD(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterDFromMidShouldRaiseHalfCarry)
{
    byte startValue = 0x0F;
    byte expected = 0x10;
    cpu.setRegisterD(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_D, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterD(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterEFrom0ShouldGive1)
{
    byte startValue = 0x00;
    byte expected = 0x01;
    cpu.setRegisterE(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_E);
    ASSERT_EQ(cpu.getRegisterE(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterEFromMaxShouldGive0)
{
    byte startValue = 0xFF;
    byte expected = 0x00;
    cpu.setRegisterE(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_E, CPU::ZERO | CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterE(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterEFromMidShouldRaiseHalfCarry)
{
    byte startValue = 0x0F;
    byte expected = 0x10;
    cpu.setRegisterE(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_E, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterE(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterHFrom0ShouldGive1)
{
    byte startValue = 0x00;
    byte expected = 0x01;
    cpu.setRegisterH(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_H);
    ASSERT_EQ(cpu.getRegisterH(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterHFromMaxShouldGive0)
{
    byte startValue = 0xFF;
    byte expected = 0x00;
    cpu.setRegisterH(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_H, CPU::ZERO | CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterH(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterHFromMidShouldRaiseHalfCarry)
{
    byte startValue = 0x0F;
    byte expected = 0x10;
    cpu.setRegisterH(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_H, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterH(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterLFrom0ShouldGive1)
{
    byte startValue = 0x00;
    byte expected = 0x01;
    cpu.setRegisterL(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_L);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterLFromMaxShouldGive0)
{
    byte startValue = 0xFF;
    byte expected = 0x00;
    cpu.setRegisterL(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_L, CPU::ZERO | CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterL(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementRegisterLFromMidShouldRaiseHalfCarry)
{
    byte startValue = 0x0F;
    byte expected = 0x10;
    cpu.setRegisterL(startValue);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_L, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterL(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementInMemoryFrom0ShouldGive1)
{
    byte startValue = 0x00;
    byte expected = 0x01;
    word addr = 0x1234;
    mmu.write(addr, startValue);
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    setExpectedTicks(3);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_HLm);
    ASSERT_EQ(mmu.read(addr), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementInMemoryFromMaxShouldGive0)
{
    byte startValue = 0xFF;
    byte expected = 0x00;
    word addr = 0x1234;
    mmu.write(addr, startValue);
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    setExpectedTicks(3);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_HLm, CPU::ZERO | CPU::HALF_CARRY);
    ASSERT_EQ(mmu.read(addr), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, IncrementInMemoryFromMidShouldRaiseHalfCarry)
{
    byte startValue = 0x0F;
    byte expected = 0x10;
    word addr = 0x1234;
    mmu.write(addr, startValue);
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    setExpectedTicks(3);
    assertIncrementRegisterWasPerformed(standardInstructions::INC_HLm, CPU::HALF_CARRY);
    ASSERT_EQ(mmu.read(addr), expected);
}
#pragma endregion

#pragma region Decrement Registers
TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterAFromMaxShouldGiveMaxMinusOne)
{
    byte startValue = 0xFF;
    byte expected = 0xFE;
    cpu.setRegisterA(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_A);
    ASSERT_EQ(cpu.getRegisterA(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterAFrom1ShouldGive0)
{
    byte startValue = 0x01;
    byte expected = 0x00;
    cpu.setRegisterA(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_A, CPU::ZERO);
    ASSERT_EQ(cpu.getRegisterA(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterAFrom0ShouldGiveMax)
{
    byte startValue = 0x00;
    byte expected = 0xFF;
    cpu.setRegisterA(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_A, CPU::HALF_CARRY);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterAFromMidShouldRaiseHalfCarry)
{
    byte startValue = 0x10;
    byte expected = 0x0F;
    cpu.setRegisterA(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_A, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterA(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterBFromMaxShouldGiveMaxMinusOne)
{
    byte startValue = 0xFF;
    byte expected = 0xFE;
    cpu.setRegisterB(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_B);
    ASSERT_EQ(cpu.getRegisterB(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterBFrom1ShouldGive0)
{
    byte startValue = 0x01;
    byte expected = 0x00;
    cpu.setRegisterB(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_B, CPU::ZERO);
    ASSERT_EQ(cpu.getRegisterB(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterBFrom0ShouldGiveMax)
{
    byte startValue = 0x00;
    byte expected = 0xFF;
    cpu.setRegisterB(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_B, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterB(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterBFromMidShouldRaiseHalfCarry)
{
    byte startValue = 0x10;
    byte expected = 0x0F;
    cpu.setRegisterB(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_B, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterB(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterCFromMaxShouldGiveMaxMinusOne)
{
    byte startValue = 0xFF;
    byte expected = 0xFE;
    cpu.setRegisterC(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_C);
    ASSERT_EQ(cpu.getRegisterC(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterCFrom1ShouldGive0)
{
    byte startValue = 0x01;
    byte expected = 0x00;
    cpu.setRegisterC(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_C, CPU::ZERO);
    ASSERT_EQ(cpu.getRegisterC(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterCFrom0ShouldGiveMax)
{
    byte startValue = 0x00;
    byte expected = 0xFF;
    cpu.setRegisterC(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_C, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterC(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterCFromMidShouldRaiseHalfCarry)
{
    byte startValue = 0x10;
    byte expected = 0x0F;
    cpu.setRegisterC(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_C, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterC(), expected);
    ASSERT_TRUE(cpu.isFlagSet(CPU::HALF_CARRY));
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterDFromMaxShouldGiveMaxMinusOne)
{
    byte startValue = 0xFF;
    byte expected = 0xFE;
    cpu.setRegisterD(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_D);
    ASSERT_EQ(cpu.getRegisterD(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterDFrom1ShouldGive0)
{
    byte startValue = 0x01;
    byte expected = 0x00;
    cpu.setRegisterD(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_D, CPU::ZERO);
    ASSERT_EQ(cpu.getRegisterD(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterDFrom0ShouldGiveMax)
{
    byte startValue = 0x00;
    byte expected = 0xFF;
    cpu.setRegisterD(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_D, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterD(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterDFromMidShouldRaiseHalfCarry)
{
    byte startValue = 0x10;
    byte expected = 0x0F;
    cpu.setRegisterD(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_D, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterD(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterEFromMaxShouldGiveMaxMinusOne)
{
    byte startValue = 0xFF;
    byte expected = 0xFE;
    cpu.setRegisterE(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_E);
    ASSERT_EQ(cpu.getRegisterE(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterEFrom1ShouldGive0)
{
    byte startValue = 0x01;
    byte expected = 0x00;
    cpu.setRegisterE(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_E, CPU::ZERO);
    ASSERT_EQ(cpu.getRegisterE(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterEFrom0ShouldGiveMax)
{
    byte startValue = 0x00;
    byte expected = 0xFF;
    cpu.setRegisterE(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_E, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterE(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterEFromMidShouldRaiseHalfCarry)
{
    byte startValue = 0x10;
    byte expected = 0x0F;
    cpu.setRegisterE(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_E, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterE(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterHFromMaxShouldGiveMaxMinusOne)
{
    byte startValue = 0xFF;
    byte expected = 0xFE;
    cpu.setRegisterH(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_H);
    ASSERT_EQ(cpu.getRegisterH(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterHFrom1ShouldGive0)
{
    byte startValue = 0x01;
    byte expected = 0x00;
    cpu.setRegisterH(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_H, CPU::ZERO);
    ASSERT_EQ(cpu.getRegisterH(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterHFrom0ShouldGiveMax)
{
    byte startValue = 0x00;
    byte expected = 0xFF;
    cpu.setRegisterH(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_H, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterH(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterHFromMidShouldRaiseHalfCarry)
{
    byte startValue = 0x10;
    byte expected = 0x0F;
    cpu.setRegisterH(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_H, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterH(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterLFromMaxShouldGiveMaxMinusOne)
{
    byte startValue = 0xFF;
    byte expected = 0xFE;
    cpu.setRegisterL(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_L);
    ASSERT_EQ(cpu.getRegisterL(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterLFrom1ShouldGive0)
{
    byte startValue = 0x01;
    byte expected = 0x00;
    cpu.setRegisterL(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_L, CPU::ZERO);
    ASSERT_EQ(cpu.getRegisterL(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterLFrom0ShouldGiveMax)
{
    byte startValue = 0x00;
    byte expected = 0xFF;
    cpu.setRegisterL(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_L, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterL(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementRegisterLFromMidShouldRaiseHalfCarry)
{
    byte startValue = 0x10;
    byte expected = 0x0F;
    cpu.setRegisterL(startValue);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_L, CPU::HALF_CARRY);
    ASSERT_EQ(cpu.getRegisterL(), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementInMemoryFromMaxShouldGiveMaxMinusOne)
{
    byte startValue = 0xFF;
    byte expected = 0xFE;
    word addr = 0x1234;
    mmu.write(addr, startValue);
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    setExpectedTicks(3);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_HLm);
    ASSERT_EQ(mmu.read(addr), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementInMemoryFrom1ShouldGive0)
{
    byte startValue = 0x01;
    byte expected = 0x00;
    word addr = 0x1234;
    mmu.write(addr, startValue);
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    setExpectedTicks(3);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_HLm, CPU::ZERO);
    ASSERT_EQ(mmu.read(addr), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementInMemoryFrom0ShouldGiveMax)
{
    byte startValue = 0x00;
    byte expected = 0xFF;
    word addr = 0x1234;
    mmu.write(addr, startValue);
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    setExpectedTicks(3);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_HLm, CPU::HALF_CARRY);
    ASSERT_EQ(mmu.read(addr), expected);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, DecrementInMemoryFromMidShouldRaiseHalfCarry)
{
    byte startValue = 0x10;
    byte expected = 0x0F;
    word addr = 0x1234;
    mmu.write(addr, startValue);
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    setExpectedTicks(3);
    assertDecrementRegisterWasPerformed(standardInstructions::DEC_HLm, CPU::HALF_CARRY);
    ASSERT_EQ(mmu.read(addr), expected);
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

#pragma region Compare
TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterBWithAccuShouldSetZeroFlagIfBothValuesAre0)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterB(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_B, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterBWithBiggerAccuDoesntSetCarryFlag)
{
    byte accuValue = 0xF0;
    byte registerValue = 0x00;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterB(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_B, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterBWithSmallerAccuSetsCarryFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0xF0;
    int expectedFlags = CPU::CARRY;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterB(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_B, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterBWithSmallerValueInLowestNibblesSetsHalfCarryFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x0F;
    int expectedFlags = CPU::HALF_CARRY | CPU::CARRY;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterB(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_B, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterCWithAccuShouldSetZeroFlagIfBothValuesAre0)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterC(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_C, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterCWithBiggerAccuDoesntSetCarryFlag)
{
    byte accuValue = 0xF0;
    byte registerValue = 0x00;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterC(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_C, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterCWithSmallerAccuSetsCarryFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0xF0;
    int expectedFlags = CPU::CARRY;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterC(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_C, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterCWithSmallerValueInLowestNibblesSetsHalfCarryFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x0F;
    int expectedFlags = CPU::HALF_CARRY | CPU::CARRY;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterC(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_C, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterDWithAccuShouldSetZeroFlagIfBothValuesAre0)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterD(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_D, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterDWithBiggerAccuDoesntSetCarryFlag)
{
    byte accuValue = 0xF0;
    byte registerValue = 0x00;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterD(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_D, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterDWithSmallerAccuSetsCarryFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0xF0;
    int expectedFlags = CPU::CARRY;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterD(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_D, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterDWithSmallerValueInLowestNibblesSetsHalfCarryFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x0F;
    int expectedFlags = CPU::HALF_CARRY | CPU::CARRY;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterD(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_D, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterEWithAccuShouldSetZeroFlagIfBothValuesAre0)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterE(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_E, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterEWithBiggerAccuDoesntSetCarryFlag)
{
    byte accuValue = 0xF0;
    byte registerValue = 0x00;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterE(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_E, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterEWithSmallerAccuSetsCarryFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0xF0;
    int expectedFlags = CPU::CARRY;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterE(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_E, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterEWithSmallerValueInLowestNibblesSetsHalfCarryFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x0F;
    int expectedFlags = CPU::HALF_CARRY | CPU::CARRY;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterE(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_E, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterHWithAccuShouldSetZeroFlagIfBothValuesAre0)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterH(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_H, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterHWithBiggerAccuDoesntSetCarryFlag)
{
    byte accuValue = 0xF0;
    byte registerValue = 0x00;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterH(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_H, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterHWithSmallerAccuSetsCarryFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0xF0;
    int expectedFlags = CPU::CARRY;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterH(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_H, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterHWithSmallerValueInLowestNibblesSetsHalfCarryFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x0F;
    int expectedFlags = CPU::HALF_CARRY | CPU::CARRY;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterH(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_H, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterLWithAccuShouldSetZeroFlagIfBothValuesAre0)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterL(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_L, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterLWithBiggerAccuDoesntSetCarryFlag)
{
    byte accuValue = 0xF0;
    byte registerValue = 0x00;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterL(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_L, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterLWithSmallerAccuSetsCarryFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0xF0;
    int expectedFlags = CPU::CARRY;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterL(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_L, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterLWithSmallerValueInLowestNibblesSetsHalfCarryFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x0F;
    int expectedFlags = CPU::HALF_CARRY | CPU::CARRY;
    cpu.setRegisterA(accuValue);
    cpu.setRegisterL(registerValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_L, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareRegisterAWithAccuShouldSetZeroFlagIfBothValuesAre0)
{
    byte accuValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterA(accuValue);
    assertCompareOperationSetFlagsCorrectly(standardInstructions::CP_A, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareImmediateValueWithAccuShouldSetZeroFlagIfBothValuesAre0)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterA(accuValue);
    assertCompareWithImmediateValueSetFlagsCorrectly(standardInstructions::CP_n, registerValue, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareImmediateValueWithBiggerAccuDoesntSetCarryFlag)
{
    byte accuValue = 0xF0;
    byte registerValue = 0x00;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterA(accuValue);
    assertCompareWithImmediateValueSetFlagsCorrectly(standardInstructions::CP_n, registerValue, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareImmediateValueWithSmallerAccuSetsCarryFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0xF0;
    int expectedFlags = CPU::CARRY;
    cpu.setRegisterA(accuValue);
    assertCompareWithImmediateValueSetFlagsCorrectly(standardInstructions::CP_n, registerValue, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareImmediateValueWithSmallerValueInLowestNibblesSetsHalfCarryFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x0F;
    int expectedFlags = CPU::HALF_CARRY | CPU::CARRY;
    cpu.setRegisterA(accuValue);
    assertCompareWithImmediateValueSetFlagsCorrectly(standardInstructions::CP_n, registerValue, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareMemoryValueWithAccuShouldSetZeroFlagIfBothValuesAre0)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    word addr = 0x1234;
    mmu.write(addr, registerValue);
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterA(accuValue);
    assertCompareWithMemorySetFlagsCorrectly(standardInstructions::CP_HLm, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareMemoryValueWithBiggerAccuDoesntSetCarryFlag)
{
    byte accuValue = 0xF0;
    byte registerValue = 0x00;
    word addr = 0x1234;
    mmu.write(addr, registerValue);
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    int expectedFlags = CPU::NONE;
    cpu.setRegisterA(accuValue);
    assertCompareWithMemorySetFlagsCorrectly(standardInstructions::CP_HLm, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareMemoryValueWithSmallerAccuSetsCarryFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0xF0;
    word addr = 0x1234;
    mmu.write(addr, registerValue);
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    int expectedFlags = CPU::CARRY;
    cpu.setRegisterA(accuValue);
    assertCompareWithMemorySetFlagsCorrectly(standardInstructions::CP_HLm, expectedFlags);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, CompareMemoryValueWithSmallerValueInLowestNibblesSetsHalfCarryFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x0F;
    word addr = 0x1234;
    mmu.write(addr, registerValue);
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    int expectedFlags = CPU::HALF_CARRY | CPU::CARRY;
    cpu.setRegisterA(accuValue);
    assertCompareWithMemorySetFlagsCorrectly(standardInstructions::CP_HLm, expectedFlags);
}
#pragma endregion

#pragma region Or Instructions
TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndRegisterAGivesExpectedResult)
{
    byte value = 0x12;
    byte expectedValue = 0x12;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndRegisterAForBothZeroValueSetsZeroFlag)
{
    byte value = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndRegisterBGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x36;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterB(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndRegisterBForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterB(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndRegisterCGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x36;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterC(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndRegisterCForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterC(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndRegisterDGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x36;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterD(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndRegisterDForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterD(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndRegisterEGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x36;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterE(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndRegisterEForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterE(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndRegisterHGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x36;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterH(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndRegisterHForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterH(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndRegisterLGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x36;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterL(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndRegisterLForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterL(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndImmediateValueGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte immediateValue = 0x34;
    byte expectedValue = 0x36;
    int expectedFlags = CPU::NONE;
    mmu.write(cpu.getProgramCounter() + 1, immediateValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndImmediateValueForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte immediateValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    mmu.write(cpu.getProgramCounter() + 1, immediateValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndValueInMemoryGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte value = 0x34;
    byte expectedValue = 0x36;
    int expectedFlags = CPU::NONE;
    word addr = 0x1234;
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    mmu.write(addr, value);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, OrBetweenAccuAndValueInMemoryForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte value = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    word addr = 0x1234;
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    mmu.write(addr, value);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::OR_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}
#pragma endregion

#pragma region Xor Instructions
TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndRegisterAGivesExpectedResult)
{
    byte value = 0x12;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndRegisterAForBothZeroValueSetsZeroFlag)
{
    byte value = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndRegisterBGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x26;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterB(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndRegisterBForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterB(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndRegisterCGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x26;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterC(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndRegisterCForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterC(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndRegisterDGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x26;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterD(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndRegisterDForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterD(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndRegisterEGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x26;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterE(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndRegisterEForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterE(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndRegisterHGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x26;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterH(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndRegisterHForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterH(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndRegisterLGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x26;
    int expectedFlags = CPU::NONE;
    cpu.setRegisterL(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndRegisterLForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    cpu.setRegisterL(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndImmediateValueGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte immediateValue = 0x34;
    byte expectedValue = 0x26;
    int expectedFlags = CPU::NONE;
    mmu.write(cpu.getProgramCounter() + 1, immediateValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndImmediateValueForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte immediateValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    mmu.write(cpu.getProgramCounter() + 1, immediateValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndValueInMemoryGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte value = 0x34;
    byte expectedValue = 0x26;
    int expectedFlags = CPU::NONE;
    word addr = 0x1234;
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    mmu.write(addr, value);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, XorBetweenAccuAndValueInMemoryForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte value = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::ZERO;
    word addr = 0x1234;
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    mmu.write(addr, value);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::XOR_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}
#pragma endregion

#pragma region And Instructions
TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndRegisterAGivesExpectedResult)
{
    byte value = 0x12;
    byte expectedValue = 0x12;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndRegisterAForBothZeroValueSetsZeroFlag)
{
    byte value = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndRegisterBGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x10;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterB(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndRegisterBForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterB(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndRegisterCGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x10;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterC(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndRegisterCForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterC(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndRegisterDGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x10;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterD(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndRegisterDForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterD(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndRegisterEGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x10;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterE(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndRegisterEForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterE(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndRegisterHGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x10;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterH(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndRegisterHForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterH(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndRegisterLGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte registerValue = 0x34;
    byte expectedValue = 0x10;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterL(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndRegisterLForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte registerValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterL(registerValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndImmediateValueGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte immediateValue = 0x34;
    byte expectedValue = 0x10;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    mmu.write(cpu.getProgramCounter() + 1, immediateValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndImmediateValueForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte immediateValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::HALF_CARRY;
    mmu.write(cpu.getProgramCounter() + 1, immediateValue);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndValueInMemoryGivesExpectedResult)
{
    byte accuValue = 0x12;
    byte value = 0x34;
    byte expectedValue = 0x10;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    word addr = 0x1234;
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    mmu.write(addr, value);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AndBetweenAccuAndValueInMemoryForBothZeroValueSetsZeroFlag)
{
    byte accuValue = 0x00;
    byte value = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::HALF_CARRY;
    word addr = 0x1234;
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    mmu.write(addr, value);
    cpu.setRegisterA(accuValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::AND_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}
#pragma endregion

#pragma region Add instructions
TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterAtoARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterAtoAGivesExpectedResult)
{
    byte value = 0x02;
    byte expectedValue = 0x04;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterAtoAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte expectedValue = 0x1E;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterAtoAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF0;
    byte expectedValue = 0xE0;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterBtoARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    cpu.setRegisterB(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterBtoAGivesExpectedResult)
{
    byte value = 0x02;
    byte addValue = 0x03;
    byte expectedValue = 0x05;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setRegisterA(value);
    cpu.setRegisterB(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterBtoAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte addValue = 0x05;
    byte expectedValue = 0x14;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterB(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterBtoAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF5;
    byte addValue = 0x12;
    byte expectedValue = 0x07;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterB(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterCtoARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    cpu.setRegisterC(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterCtoAGivesExpectedResult)
{
    byte value = 0x02;
    byte addValue = 0x03;
    byte expectedValue = 0x05;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setRegisterA(value);
    cpu.setRegisterC(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterCtoAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte addValue = 0x05;
    byte expectedValue = 0x14;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterC(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterCtoAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF5;
    byte addValue = 0x12;
    byte expectedValue = 0x07;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterC(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterDtoARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    cpu.setRegisterD(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterDtoAGivesExpectedResult)
{
    byte value = 0x02;
    byte addValue = 0x03;
    byte expectedValue = 0x05;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setRegisterA(value);
    cpu.setRegisterD(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterDtoAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte addValue = 0x05;
    byte expectedValue = 0x14;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterD(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterDtoAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF5;
    byte addValue = 0x12;
    byte expectedValue = 0x07;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterD(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterEtoARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    cpu.setRegisterE(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterEtoAGivesExpectedResult)
{
    byte value = 0x02;
    byte addValue = 0x03;
    byte expectedValue = 0x05;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setRegisterA(value);
    cpu.setRegisterE(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterEtoAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte addValue = 0x05;
    byte expectedValue = 0x14;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterE(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterEtoAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF5;
    byte addValue = 0x12;
    byte expectedValue = 0x07;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterE(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterHtoARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    cpu.setRegisterH(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterHtoAGivesExpectedResult)
{
    byte value = 0x02;
    byte addValue = 0x03;
    byte expectedValue = 0x05;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setRegisterA(value);
    cpu.setRegisterH(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterHtoAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte addValue = 0x05;
    byte expectedValue = 0x14;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterH(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterHtoAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF5;
    byte addValue = 0x12;
    byte expectedValue = 0x07;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterH(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterLtoARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    cpu.setRegisterL(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterLtoAGivesExpectedResult)
{
    byte value = 0x02;
    byte addValue = 0x03;
    byte expectedValue = 0x05;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setRegisterA(value);
    cpu.setRegisterL(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterLtoAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte addValue = 0x05;
    byte expectedValue = 0x14;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterL(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddRegisterLtoAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF5;
    byte addValue = 0x12;
    byte expectedValue = 0x07;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterL(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddImmediateValuetoARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    mmu.write(cpu.getProgramCounter() + 1, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddImmediateValuetoAGivesExpectedResult)
{
    byte value = 0x02;
    byte addValue = 0x03;
    byte expectedValue = 0x05;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setRegisterA(value);
    mmu.write(cpu.getProgramCounter() + 1, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddImmediateValuetoAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte addValue = 0x05;
    byte expectedValue = 0x14;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterA(value);
    mmu.write(cpu.getProgramCounter() + 1, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddImmediateValuetoAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF5;
    byte addValue = 0x12;
    byte expectedValue = 0x07;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setRegisterA(value);
    mmu.write(cpu.getProgramCounter() + 1, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddValueInMemorytoARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    word addr = 0x1234;
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    mmu.write(addr, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddValueInMemorytoAGivesExpectedResult)
{
    byte value = 0x02;
    byte addValue = 0x03;
    byte expectedValue = 0x05;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setRegisterA(value);
    word addr = 0x1234;
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    mmu.write(addr, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddValueInMemorytoAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte addValue = 0x05;
    byte expectedValue = 0x14;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterA(value);
    word addr = 0x1234;
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    mmu.write(addr, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddValueInMemorytoAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF5;
    byte addValue = 0x12;
    byte expectedValue = 0x07;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setRegisterA(value);
    word addr = 0x1234;
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    mmu.write(addr, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADD_A_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}
#pragma endregion

#pragma region Add With Carry Instructions
TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterAtoARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterAtoAWhenCarryIsSetAddsPlusOne)
{
    byte value = 0x00;
    byte expectedValue = 0x01;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterAtoAGivesExpectedResult)
{
    byte value = 0x02;
    byte expectedValue = 0x05;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterAtoAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte expectedValue = 0x1F;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterAtoAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF0;
    byte expectedValue = 0xE1;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterBtoARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    cpu.setRegisterB(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterBtoAWhenCarryIsSetAddsPlusOne)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x01;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterB(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterBtoAGivesExpectedResult)
{
    byte value = 0x02;
    byte addValue = 0x10;
    byte expectedValue = 0x13;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterB(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterBtoAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte addValue = 0x08;
    byte expectedValue = 0x18;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterB(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterBtoAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF0;
    byte addValue = 0xA0;
    byte expectedValue = 0x91;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterB(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterCtoARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    cpu.setRegisterC(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterCtoAWhenCarryIsSetAddsPlusOne)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x01;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterC(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterCtoAGivesExpectedResult)
{
    byte value = 0x02;
    byte addValue = 0x10;
    byte expectedValue = 0x13;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterC(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterCtoAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte addValue = 0x08;
    byte expectedValue = 0x18;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterC(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterCtoAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF0;
    byte addValue = 0xA0;
    byte expectedValue = 0x91;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterC(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterDtoARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    cpu.setRegisterD(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterDtoAWhenCarryIsSetAddsPlusOne)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x01;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterD(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterDtoAGivesExpectedResult)
{
    byte value = 0x02;
    byte addValue = 0x10;
    byte expectedValue = 0x13;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterD(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterDtoAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte addValue = 0x08;
    byte expectedValue = 0x18;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterD(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterDtoAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF0;
    byte addValue = 0xA0;
    byte expectedValue = 0x91;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterD(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterEtoARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    cpu.setRegisterE(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterEtoAWhenCarryIsSetAddsPlusOne)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x01;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterE(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterEtoAGivesExpectedResult)
{
    byte value = 0x02;
    byte addValue = 0x10;
    byte expectedValue = 0x13;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterE(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterEtoAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte addValue = 0x08;
    byte expectedValue = 0x18;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterE(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterEtoAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF0;
    byte addValue = 0xA0;
    byte expectedValue = 0x91;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterE(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterHtoARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    cpu.setRegisterH(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterHtoAWhenCarryIsSetAddsPlusOne)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x01;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterH(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterHtoAGivesExpectedResult)
{
    byte value = 0x02;
    byte addValue = 0x10;
    byte expectedValue = 0x13;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterH(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterHtoAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte addValue = 0x08;
    byte expectedValue = 0x18;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterH(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterHtoAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF0;
    byte addValue = 0xA0;
    byte expectedValue = 0x91;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterH(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterLtoARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    cpu.setRegisterL(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterLtoAWhenCarryIsSetAddsPlusOne)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x01;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterL(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterLtoAGivesExpectedResult)
{
    byte value = 0x02;
    byte addValue = 0x10;
    byte expectedValue = 0x13;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterL(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterLtoAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte addValue = 0x08;
    byte expectedValue = 0x18;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterL(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryRegisterLtoAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF0;
    byte addValue = 0xA0;
    byte expectedValue = 0x91;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterL(addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryImmediateValuetoARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    mmu.write(cpu.getProgramCounter() + 1, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryImmediateValuetoAWhenCarryIsSetAddsPlusOne)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x01;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    mmu.write(cpu.getProgramCounter() + 1, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryImmediateValuetoAGivesExpectedResult)
{
    byte value = 0x02;
    byte addValue = 0x10;
    byte expectedValue = 0x13;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    mmu.write(cpu.getProgramCounter() + 1, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryImmediateValuetoAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte addValue = 0x08;
    byte expectedValue = 0x18;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    mmu.write(cpu.getProgramCounter() + 1, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryImmediateValuetoAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF0;
    byte addValue = 0xA0;
    byte expectedValue = 0x91;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    mmu.write(cpu.getProgramCounter() + 1, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryFromMemoryToARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO;
    cpu.setRegisterA(value);
    word addr = 0x1234;
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    mmu.write(addr, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryFromMemoryToAWhenCarryIsSetAddsPlusOne)
{
    byte value = 0x00;
    byte addValue = 0x00;
    byte expectedValue = 0x01;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    word addr = 0x1234;
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    mmu.write(addr, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryFromMemoryToAGivesExpectedResult)
{
    byte value = 0x02;
    byte addValue = 0x10;
    byte expectedValue = 0x13;
    int expectedFlags = CPU::CpuFlags::NONE;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    word addr = 0x1234;
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    mmu.write(addr, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryFromMemoryToAGivesExpectedResultAndRaisesHalfCarry)
{
    byte value = 0x0F;
    byte addValue = 0x08;
    byte expectedValue = 0x18;
    int expectedFlags = CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    word addr = 0x1234;
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    mmu.write(addr, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, AddWithCarryFromMemoryToAGivesExpectedResultAndRaisesCarry)
{
    byte value = 0xF0;
    byte addValue = 0xA0;
    byte expectedValue = 0x91;
    int expectedFlags = CPU::CpuFlags::CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    word addr = 0x1234;
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    mmu.write(addr, addValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::ADC_A_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}
#pragma endregion

#pragma region Substraction Instructions
TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterAFromARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterAtFromGivesExpectedResult)
{
    byte value = 0xA0;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterBFromARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte subValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    cpu.setRegisterB(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterBFromAGivesExpectedResult)
{
    byte value = 0xC0;
    byte subValue = 0xA0;
    byte expectedValue = 0x20;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    cpu.setRegisterB(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterBFromAGivesExpectedResultAndRaisesCarryAndHalfCarry)
{
    byte value = 0x00;
    byte subValue = 0x05;
    byte expectedValue = 0xFB;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY | CPU::CpuFlags::CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterB(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterCFromARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte subValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    cpu.setRegisterC(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterCFromAGivesExpectedResult)
{
    byte value = 0xC0;
    byte subValue = 0xA0;
    byte expectedValue = 0x20;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    cpu.setRegisterC(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterCFromAGivesExpectedResultAndRaisesCarryAndHalfCarry)
{
    byte value = 0x00;
    byte subValue = 0x05;
    byte expectedValue = 0xFB;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY | CPU::CpuFlags::CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterC(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterDFromARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte subValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    cpu.setRegisterD(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterDFromAGivesExpectedResult)
{
    byte value = 0xC0;
    byte subValue = 0xA0;
    byte expectedValue = 0x20;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    cpu.setRegisterD(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterDFromAGivesExpectedResultAndRaisesCarryAndHalfCarry)
{
    byte value = 0x00;
    byte subValue = 0x05;
    byte expectedValue = 0xFB;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY | CPU::CpuFlags::CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterD(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterEFromARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte subValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    cpu.setRegisterE(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterEFromAGivesExpectedResult)
{
    byte value = 0xC0;
    byte subValue = 0xA0;
    byte expectedValue = 0x20;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    cpu.setRegisterE(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterEFromAGivesExpectedResultAndRaisesCarryAndHalfCarry)
{
    byte value = 0x00;
    byte subValue = 0x05;
    byte expectedValue = 0xFB;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY | CPU::CpuFlags::CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterE(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterHFromARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte subValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    cpu.setRegisterH(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterHFromAGivesExpectedResult)
{
    byte value = 0xC0;
    byte subValue = 0xA0;
    byte expectedValue = 0x20;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    cpu.setRegisterH(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterHFromAGivesExpectedResultAndRaisesCarryAndHalfCarry)
{
    byte value = 0x00;
    byte subValue = 0x05;
    byte expectedValue = 0xFB;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY | CPU::CpuFlags::CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterH(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterLFromARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte subValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    cpu.setRegisterL(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterLFromAGivesExpectedResult)
{
    byte value = 0xC0;
    byte subValue = 0xA0;
    byte expectedValue = 0x20;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    cpu.setRegisterL(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubRegisterLFromAGivesExpectedResultAndRaisesCarryAndHalfCarry)
{
    byte value = 0x00;
    byte subValue = 0x05;
    byte expectedValue = 0xFB;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY | CPU::CpuFlags::CARRY;
    cpu.setRegisterA(value);
    cpu.setRegisterL(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubImmediateValueFromARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte subValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    mmu.write(cpu.getProgramCounter() + 1, subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubImmediateValueFromAGivesExpectedResult)
{
    byte value = 0xC0;
    byte subValue = 0xA0;
    byte expectedValue = 0x20;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    mmu.write(cpu.getProgramCounter() + 1, subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubImmediateValueFromAGivesExpectedResultAndRaisesCarryAndHalfCarry)
{
    byte value = 0x00;
    byte subValue = 0x05;
    byte expectedValue = 0xFB;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY | CPU::CpuFlags::CARRY;
    cpu.setRegisterA(value);
    mmu.write(cpu.getProgramCounter() + 1, subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubMemoryValueFromARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte subValue = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    word addr = 0x1234;
    mmu.write(addr, subValue);
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubMemoryValueFromAGivesExpectedResult)
{
    byte value = 0xC0;
    byte subValue = 0xA0;
    byte expectedValue = 0x20;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    word addr = 0x1234;
    mmu.write(addr, subValue);
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubMemoryValueFromAGivesExpectedResultAndRaisesCarryAndHalfCarry)
{
    byte value = 0x00;
    byte subValue = 0x05;
    byte expectedValue = 0xFB;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY | CPU::CpuFlags::CARRY;
    cpu.setRegisterA(value);
    word addr = 0x1234;
    mmu.write(addr, subValue);
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SUB_A_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}
#pragma endregion

#pragma region Substraction With Carry Instructions
TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryRegisterAFromARaisesZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte expectedValue = 0x00;
    int expectedFlags = CPU::CpuFlags::ZERO | CPU::CpuFlags::SUBSTRACTION;
    cpu.setRegisterA(value);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryRegisterAFromAGivesExpectedResult)
{
    byte value = 0xA0;
    byte expectedValue = 0xFF;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::CARRY | CPU::CpuFlags::HALF_CARRY;
    cpu.setRegisterA(value);
    cpu.setFlag(CPU::CpuFlags::CARRY);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_A);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryRegisterBFromADoesntRaiseZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte subValue = 0x00;
    byte expectedValue = 0xFF;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::CARRY | CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterB(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryRegisterBFromAGivesExpectedResult)
{
    byte value = 0xC0;
    byte subValue = 0xA0;
    byte expectedValue = 0x1F;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterB(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest,
       SubWithCarryRegisterBFromAGivesExpectedResultAndRaisesCarryAndHalfCarry)
{
    byte value = 0x00;
    byte subValue = 0x05;
    byte expectedValue = 0xFA;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY | CPU::CpuFlags::CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterB(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_B);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryRegisterCFromADoesntRaiseZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte subValue = 0x00;
    byte expectedValue = 0xFF;
    int expectedFlags = CPU::CpuFlags::CARRY | CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterC(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryRegisterCFromAGivesExpectedResult)
{
    byte value = 0xC0;
    byte subValue = 0xA0;
    byte expectedValue = 0x1F;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterC(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest,
       SubWithCarryRegisterCFromAGivesExpectedResultAndRaisesCarryAndHalfCarry)
{
    byte value = 0x00;
    byte subValue = 0x05;
    byte expectedValue = 0xFA;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY | CPU::CpuFlags::CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterC(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_C);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryRegisterDFromADoesntRaiseZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte subValue = 0x00;
    byte expectedValue = 0xFF;
    int expectedFlags = CPU::CpuFlags::CARRY | CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterD(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryRegisterDFromAGivesExpectedResult)
{
    byte value = 0xC0;
    byte subValue = 0xA0;
    byte expectedValue = 0x1F;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterD(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest,
       SubWithCarryRegisterDFromAGivesExpectedResultAndRaisesCarryAndHalfCarry)
{
    byte value = 0x00;
    byte subValue = 0x05;
    byte expectedValue = 0xFA;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY | CPU::CpuFlags::CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterD(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_D);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryRegisterEFromADoesntRaiseZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte subValue = 0x00;
    byte expectedValue = 0xFF;
    int expectedFlags = CPU::CpuFlags::CARRY | CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterE(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryRegisterEFromAGivesExpectedResult)
{
    byte value = 0xC0;
    byte subValue = 0xA0;
    byte expectedValue = 0x1F;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterE(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest,
       SubWithCarryRegisterEFromAGivesExpectedResultAndRaisesCarryAndHalfCarry)
{
    byte value = 0x00;
    byte subValue = 0x05;
    byte expectedValue = 0xFA;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY | CPU::CpuFlags::CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterE(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_E);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryRegisterHFromADoesntRaiseZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte subValue = 0x00;
    byte expectedValue = 0xFF;
    int expectedFlags = CPU::CpuFlags::CARRY | CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterH(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryRegisterHFromAGivesExpectedResult)
{
    byte value = 0xC0;
    byte subValue = 0xA0;
    byte expectedValue = 0x1F;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterH(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest,
       SubWithCarryRegisterHFromAGivesExpectedResultAndRaisesCarryAndHalfCarry)
{
    byte value = 0x00;
    byte subValue = 0x05;
    byte expectedValue = 0xFA;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY | CPU::CpuFlags::CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterH(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_H);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryRegisterLFromADoesntRaiseZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte subValue = 0x00;
    byte expectedValue = 0xFF;
    int expectedFlags = CPU::CpuFlags::CARRY | CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterL(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryRegisterLFromAGivesExpectedResult)
{
    byte value = 0xC0;
    byte subValue = 0xA0;
    byte expectedValue = 0x1F;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterL(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest,
       SubWithCarryRegisterLFromAGivesExpectedResultAndRaisesCarryAndHalfCarry)
{
    byte value = 0x00;
    byte subValue = 0x05;
    byte expectedValue = 0xFA;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY | CPU::CpuFlags::CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    cpu.setRegisterL(subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(1);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_L);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryImmediateValueFromADoesntRaiseZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte subValue = 0x00;
    byte expectedValue = 0xFF;
    int expectedFlags = CPU::CpuFlags::CARRY | CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    mmu.write(cpu.getProgramCounter() + 1, subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryImmediateValueFromAGivesExpectedResult)
{
    byte value = 0xC0;
    byte subValue = 0xA0;
    byte expectedValue = 0x1F;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    mmu.write(cpu.getProgramCounter() + 1, subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest,
       SubWithCarryImmediateValueFromAGivesExpectedResultAndRaisesCarryAndHalfCarry)
{
    byte value = 0x00;
    byte subValue = 0x05;
    byte expectedValue = 0xFA;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY | CPU::CpuFlags::CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    mmu.write(cpu.getProgramCounter() + 1, subValue);
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_n);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryMemoryValueFromADoesntRaiseZeroFlagForZeroValue)
{
    byte value = 0x00;
    byte subValue = 0x00;
    byte expectedValue = 0xFF;
    int expectedFlags = CPU::CpuFlags::CARRY | CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    word addr = 0x1234;
    mmu.write(addr, subValue);
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest, SubWithCarryMemoryValueFromAGivesExpectedResult)
{
    byte value = 0xC0;
    byte subValue = 0xA0;
    byte expectedValue = 0x1F;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    word addr = 0x1234;
    mmu.write(addr, subValue);
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}

TEST_F(CpuInstructions8BitsArithmeticLogicalTest,
       SubWithCarryMemoryValueFromAGivesExpectedResultAndRaisesCarryAndHalfCarry)
{
    byte value = 0x00;
    byte subValue = 0x05;
    byte expectedValue = 0xFA;
    int expectedFlags = CPU::CpuFlags::SUBSTRACTION | CPU::CpuFlags::HALF_CARRY | CPU::CpuFlags::CARRY;
    cpu.setFlag(CPU::CpuFlags::CARRY);
    cpu.setRegisterA(value);
    word addr = 0x1234;
    mmu.write(addr, subValue);
    cpu.setRegisterH(getMsbFromWord(addr));
    cpu.setRegisterL(getLsbFromWord(addr));
    setExpectedFlags(expectedFlags);
    setExpectedTicks(2);
    assertStandardInstructionWasExecutedCorrectly(standardInstructions::SBC_A_HLm);
    ASSERT_EQ(cpu.getRegisterA(), expectedValue);
}
#pragma endregion