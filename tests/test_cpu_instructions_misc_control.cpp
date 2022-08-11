#include "cpu.hpp"
#include "instructions.hpp"
#include "utils.hpp"

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
