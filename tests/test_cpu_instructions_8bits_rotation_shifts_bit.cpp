#include "cpu.hpp"
#include "instructions.hpp"
#include "utils.hpp"

#include <gtest/gtest.h>

using namespace utils;

class CpuInstructions8BitsRotationShiftsBitTest : public ::testing::Test
{
  protected:
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

    MMU mmu;
    CPU cpu = CPU(mmu);
};

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
    cpu.setRegisterA(0x02);
    assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_A);
    ASSERT_EQ(cpu.getRegisterA(), 0x01);
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterAShouldCarryFirstBit)
{
    cpu.setRegisterA(0xF1);
    assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_A);
    ASSERT_EQ(cpu.getRegisterA(), 0x78);
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
    cpu.setRegisterB(0x02);
    assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_B);
    ASSERT_EQ(cpu.getRegisterB(), 0x01);
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterBShouldCarryFirstBit)
{
    cpu.setRegisterB(0xF1);
    assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_B);
    ASSERT_EQ(cpu.getRegisterB(), 0x78);
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
    cpu.setRegisterC(0x02);
    assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_C);
    ASSERT_EQ(cpu.getRegisterC(), 0x01);
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterCShouldCarryFirstBit)
{
    cpu.setRegisterC(0xF1);
    assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_C);
    ASSERT_EQ(cpu.getRegisterC(), 0x78);
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
    cpu.setRegisterD(0x02);
    assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_D);
    ASSERT_EQ(cpu.getRegisterD(), 0x01);
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterDShouldCarryFirstBit)
{
    cpu.setRegisterD(0xF1);
    assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_D);
    ASSERT_EQ(cpu.getRegisterD(), 0x78);
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
    cpu.setRegisterE(0x02);
    assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_E);
    ASSERT_EQ(cpu.getRegisterE(), 0x01);
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterEShouldCarryFirstBit)
{
    cpu.setRegisterE(0xF1);
    assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_E);
    ASSERT_EQ(cpu.getRegisterE(), 0x78);
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
    cpu.setRegisterH(0x02);
    assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_H);
    ASSERT_EQ(cpu.getRegisterH(), 0x01);
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterHShouldCarryFirstBit)
{
    cpu.setRegisterH(0xF1);
    assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_H);
    ASSERT_EQ(cpu.getRegisterH(), 0x78);
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
    cpu.setRegisterL(0x02);
    assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_L);
    ASSERT_EQ(cpu.getRegisterL(), 0x01);
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuInstructions8BitsRotationShiftsBitTest, ShiftRightArithmeticRegisterLShouldCarryFirstBit)
{
    cpu.setRegisterL(0xF1);
    assertShiftRightArithmeticRegisterWasPerformed(extendedInstructions::SRA_L);
    ASSERT_EQ(cpu.getRegisterL(), 0x78);
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
    mmu.write(addr, 0x02);
    assertShiftRightArithmeticInMemoryWasPerformed(extendedInstructions::SRA_HLm);
    ASSERT_EQ(mmu.read(addr), 0x01);
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
    ASSERT_EQ(mmu.read(addr), 0x78);
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
    ASSERT_TRUE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}
#pragma endregion