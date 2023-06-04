#include "common/utils.hpp"
#include "cpu/cpu.hpp"
#include "cpu/instructions.hpp"

#include <gtest/gtest.h>

using namespace utils;

class CpuInstructionsMiscControlTest : public ::testing::Test
{
  protected:
    MMU mmu;
    CPU cpu = CPU(mmu);
};

TEST_F(CpuInstructionsMiscControlTest, NoopInstructionsShouldNotChangeAnything)
{
    mmu.write(cpu.getProgramCounter(), standardInstructions::NOP);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 1);
    ASSERT_EQ(cpu.getStackPointer(), 0);
    ASSERT_EQ(cpu.getProgramCounter(), 1);
    ASSERT_EQ(cpu.getFlag(), 0);
    ASSERT_EQ(cpu.getRegisterA(), 0);
    ASSERT_EQ(cpu.getRegisterB(), 0);
    ASSERT_EQ(cpu.getRegisterC(), 0);
    ASSERT_EQ(cpu.getRegisterD(), 0);
    ASSERT_EQ(cpu.getRegisterE(), 0);
    ASSERT_EQ(cpu.getRegisterH(), 0);
    ASSERT_EQ(cpu.getRegisterL(), 0);
}

TEST_F(CpuInstructionsMiscControlTest, EnableInterruptsShouldEnableInterrupts)
{
    mmu.write(cpu.getProgramCounter(), standardInstructions::EI);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 1);
    ASSERT_EQ(cpu.getStackPointer(), 0);
    ASSERT_EQ(cpu.getProgramCounter(), 1);

    // TODO: Add interrupts tests
}

TEST_F(CpuInstructionsMiscControlTest, DisableInterruptsShouldDisableInterrupts)
{
    mmu.write(cpu.getProgramCounter(), standardInstructions::DI);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 1);
    ASSERT_EQ(cpu.getStackPointer(), 0);
    ASSERT_EQ(cpu.getProgramCounter(), 1);

    // TODO: Add interrupts tests
}

TEST_F(CpuInstructionsMiscControlTest, StopInstructionShouldStopCPU)
{
    mmu.write(cpu.getProgramCounter(), standardInstructions::STOP);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 0);
    ASSERT_EQ(cpu.getProgramCounter(), 2);

    // TODO: Check that CPU is stopped
}

TEST_F(CpuInstructionsMiscControlTest, HaltInstructionShouldHaltCPU)
{
    mmu.write(cpu.getProgramCounter(), standardInstructions::HALT);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 0);
    ASSERT_EQ(cpu.getProgramCounter(), 1);

    // TODO: Check that CPU is halted
}
