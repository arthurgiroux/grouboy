#include "common/utils.hpp"
#include "cpu/cpu.hpp"
#include "cpu/instructions.hpp"
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
        ASSERT_EQ(cpu.getCurrentTick(), expectedTicks);
        ASSERT_EQ(cpu.getFlag(), expectedFlag);
    }
};

TEST_F(CpuTest, GetRegistersValueAtInitReturnsZero)
{
    ASSERT_EQ(cpu.getRegisterA(), 0x00);
    ASSERT_EQ(cpu.getRegisterB(), 0x00);
    ASSERT_EQ(cpu.getRegisterC(), 0x00);
    ASSERT_EQ(cpu.getRegisterD(), 0x00);
    ASSERT_EQ(cpu.getRegisterE(), 0x00);
    ASSERT_EQ(cpu.getRegisterH(), 0x00);
    ASSERT_EQ(cpu.getRegisterL(), 0x00);
}

TEST_F(CpuTest, GetFlagAtInitReturnsZero)
{
    ASSERT_EQ(cpu.getFlag(), 0x00);
}

TEST_F(CpuTest, GetCurrentTickAtInitShouldReturnZero)
{
    ASSERT_EQ(cpu.getCurrentTick(), 0);
}

TEST_F(CpuTest, GetProgramCounterAtInitShouldReturnZero)
{
    ASSERT_EQ(cpu.getProgramCounter(), 0);
}

TEST_F(CpuTest, GetStackPointerAtInitShouldReturnZero)
{
    ASSERT_EQ(cpu.getStackPointer(), 0);
}

TEST_F(CpuTest, IsHaltedAtInitShouldReturnFalse)
{
    ASSERT_EQ(cpu.isHalted(), false);
}

TEST_F(CpuTest, AreInterruptsEnabledInitShouldReturnFalse)
{
    ASSERT_EQ(cpu.areInterruptsEnabled(), false);
}

TEST_F(CpuTest, IsZeroFlagSetAtInitShouldReturnFalse)
{
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
}

TEST_F(CpuTest, IsSubtractionFlagSetAtInitShouldReturnFalse)
{
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
}

TEST_F(CpuTest, IsCarryFlagSetAtInitShouldReturnFalse)
{
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
}

TEST_F(CpuTest, IsHalfCarryFlagSetAtInitShouldReturnFalse)
{
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
    ASSERT_EQ(cpu.getProgramCounter(), 0x01);
}

TEST_F(CpuTest, GetSetStackPointerShouldChangeValue)
{
    word value = 0xCAFE;
    ASSERT_EQ(cpu.getStackPointer(), 0x00);
    cpu.setStackPointer(value);
    ASSERT_EQ(cpu.getStackPointer(), value);
}

TEST_F(CpuTest, GetSetProgramCounterShouldChangeValue)
{
    word value = 0xCAFE;
    ASSERT_EQ(cpu.getProgramCounter(), 0x00);
    cpu.setProgramCounter(value);
    ASSERT_EQ(cpu.getProgramCounter(), value);
}

TEST_F(CpuTest, GetSetRegisterAShouldChangeValue)
{
    byte value = 0x12;
    ASSERT_EQ(cpu.getRegisterA(), 0x00);
    cpu.setRegisterA(value);
    ASSERT_EQ(cpu.getRegisterA(), value);
}

TEST_F(CpuTest, GetSetRegisterBShouldChangeValue)
{
    byte value = 0x12;
    ASSERT_EQ(cpu.getRegisterB(), 0x00);
    cpu.setRegisterB(value);
    ASSERT_EQ(cpu.getRegisterB(), value);
}

TEST_F(CpuTest, GetSetRegisterCShouldChangeValue)
{
    byte value = 0x12;
    ASSERT_EQ(cpu.getRegisterC(), 0x00);
    cpu.setRegisterC(value);
    ASSERT_EQ(cpu.getRegisterC(), value);
}

TEST_F(CpuTest, GetSetRegisterDShouldChangeValue)
{
    byte value = 0x12;
    ASSERT_EQ(cpu.getRegisterD(), 0x00);
    cpu.setRegisterD(value);
    ASSERT_EQ(cpu.getRegisterD(), value);
}

TEST_F(CpuTest, GetSetRegisterEShouldChangeValue)
{
    byte value = 0x12;
    ASSERT_EQ(cpu.getRegisterE(), 0x00);
    cpu.setRegisterE(value);
    ASSERT_EQ(cpu.getRegisterE(), value);
}

TEST_F(CpuTest, GetSetRegisterHShouldChangeValue)
{
    byte value = 0x12;
    ASSERT_EQ(cpu.getRegisterH(), 0x00);
    cpu.setRegisterH(value);
    ASSERT_EQ(cpu.getRegisterH(), value);
}

TEST_F(CpuTest, GetSetRegisterLShouldChangeValue)
{
    byte value = 0x12;
    ASSERT_EQ(cpu.getRegisterL(), 0x00);
    cpu.setRegisterL(value);
    ASSERT_EQ(cpu.getRegisterL(), value);
}