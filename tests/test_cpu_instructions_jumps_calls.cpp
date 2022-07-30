#include "cpu.hpp"
#include "instructions.hpp"
#include "utils.hpp"

#include <gtest/gtest.h>

using namespace utils;

class CpuInstructionsJumpsCallsTest : public ::testing::Test
{
  protected:
	void assertInstructionPerformedJump(byte instruction, bool expected)
	{
		auto flag = cpu.getFlag();
		cpu.setProgramCounter(0x00);
		uint16_t addr = 0x1234;
		mmu.write(cpu.getProgramCounter(), instruction);
		mmu.writeWord(cpu.getProgramCounter() + 1, addr);
		int ticks = cpu.fetchDecodeAndExecute();
		if (expected)
		{
			ASSERT_EQ(ticks, 4);
			ASSERT_EQ(cpu.getProgramCounter(), addr);
		}
		else
		{
			ASSERT_EQ(ticks, 3);
			ASSERT_EQ(cpu.getProgramCounter(), 3);
		}
		ASSERT_EQ(cpu.getFlag(), flag);
	}

	void assertJumpRelativeJumpedToCorrectOffset(int8_t offset)
	{
		uint16_t startAddr = 0x1234;
		cpu.setProgramCounter(startAddr);
		mmu.write(cpu.getProgramCounter(), standardInstructions::JR_n);
		mmu.write(cpu.getProgramCounter() + 1, offset);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 3);
		ASSERT_EQ(cpu.getFlag(), 0x00);
		ASSERT_EQ(cpu.getProgramCounter(), startAddr + 2 + offset);
	}

	void assertInstructionPerformedJumpAtOffset(byte instruction, int8_t offset, bool expected)
	{
		auto flag = cpu.getFlag();
		uint16_t startAddr = 0x1234;
		cpu.setProgramCounter(startAddr);
		mmu.write(cpu.getProgramCounter(), instruction);
		mmu.writeWord(cpu.getProgramCounter() + 1, offset);
		int ticks = cpu.fetchDecodeAndExecute();
		if (expected)
		{
			ASSERT_EQ(ticks, 3);
			ASSERT_EQ(cpu.getProgramCounter(), startAddr + 2 + offset);
		}
		else
		{
			ASSERT_EQ(ticks, 2);
			ASSERT_EQ(cpu.getProgramCounter(), startAddr + 2);
		}
		ASSERT_EQ(cpu.getFlag(), flag);
	}

	MMU mmu;
	CPU cpu = CPU(mmu);
};

TEST_F(CpuInstructionsJumpsCallsTest, JumpShouldGoToImmediateMemoryAddr)
{
	cpu.setProgramCounter(0x00);
	uint16_t addr = 0x1234;
	mmu.write(cpu.getProgramCounter(), standardInstructions::JP_nn);
	mmu.writeWord(cpu.getProgramCounter() + 1, addr);
	int ticks = cpu.fetchDecodeAndExecute();
	ASSERT_EQ(ticks, 4);
	ASSERT_EQ(cpu.getFlag(), 0x00);
	ASSERT_EQ(cpu.getProgramCounter(), addr);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpConditionalCarryShouldNotJumpWithFlagUnset)
{
    assertInstructionPerformedJump(standardInstructions::JP_C_nn, false);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpConditionalCarryShouldJumpWithFlagSet)
{
    cpu.setFlag(CPU::CARRY);
    assertInstructionPerformedJump(standardInstructions::JP_C_nn, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpConditionalNotCarryShouldJumpWithFlagUnset)
{
    assertInstructionPerformedJump(standardInstructions::JP_NC_nn, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpConditionalNotCarryShouldNotJumpWithFlagSet)
{
    cpu.setFlag(CPU::CARRY);
    assertInstructionPerformedJump(standardInstructions::JP_NC_nn, false);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpConditionalZeroShouldNotJumpWithFlagUnset)
{
    assertInstructionPerformedJump(standardInstructions::JP_Z_nn, false);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpConditionalZeroShouldJumpWithFlagSet)
{
    cpu.setFlag(CPU::ZERO);
    assertInstructionPerformedJump(standardInstructions::JP_Z_nn, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpConditionalNotZeroShouldJumpWithFlagUnset)
{
    assertInstructionPerformedJump(standardInstructions::JP_NZ_nn, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpConditionalNotZeroShouldNotJumpWithFlagSet)
{
    cpu.setFlag(CPU::ZERO);
    assertInstructionPerformedJump(standardInstructions::JP_NZ_nn, false);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpShouldGoToHLMemoryAddr)
{
	cpu.setProgramCounter(0x00);
	cpu.setRegisterH(0x12);
	cpu.setRegisterL(0x34);
	uint16_t addr = 0x1234;
	mmu.write(cpu.getProgramCounter(), standardInstructions::JP_HLm);
	int ticks = cpu.fetchDecodeAndExecute();
	ASSERT_EQ(ticks, 1);
	ASSERT_EQ(cpu.getFlag(), 0x00);
	ASSERT_EQ(cpu.getProgramCounter(), addr);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeOffset0ShouldNotChange)
{
	assertJumpRelativeJumpedToCorrectOffset(0);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeOffset5ShouldGoForwardBy5)
{
    assertJumpRelativeJumpedToCorrectOffset(5);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeOffsetMinus5ShouldGoBackwardBy5)
{
    assertJumpRelativeJumpedToCorrectOffset(-5);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeOffset127ShouldGoForwardBy127)
{
    assertJumpRelativeJumpedToCorrectOffset(127);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeOffsetMinus127ShouldGoBackwardBy127)
{
    assertJumpRelativeJumpedToCorrectOffset(-127);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalCarryShouldNotJumpWhenFlagIsUnset)
{
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_C_n, 5, false);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalNotCarryShouldNotJumpWhenFlagIsSet)
{
    cpu.setFlag(CPU::CARRY);
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_NC_n, 5, false);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalZeroShouldNotJumpWhenFlagIsUnset)
{
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_Z_n, 5, false);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalNotZeroShouldNotJumpWhenFlagIsSet)
{
    cpu.setFlag(CPU::ZERO);
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_NZ_n, 5, false);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalCarryWithOffset0ShouldNotChangeWhenFlagIsSet)
{
    cpu.setFlag(CPU::CARRY);
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_C_n, 0, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalCarryWithOffset5ShouldGoForwardBy5WhenFlagIsSet)
{
    cpu.setFlag(CPU::CARRY);
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_C_n, 5, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalCarryWithOffsetMinus5ShouldGoBackwardBy5WhenFlagIsSet)
{
    cpu.setFlag(CPU::CARRY);
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_C_n, -5, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalCarryWithOffset127ShouldGoForwardBy127WhenFlagIsSet)
{
    cpu.setFlag(CPU::CARRY);
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_C_n, 127, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalCarryWithOffsetMinus127ShouldGoBackwardBy127WhenFlagIsSet)
{
    cpu.setFlag(CPU::CARRY);
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_C_n, -127, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalNotCarryWithOffset0ShouldNotChangeWhenFlagIsUnset)
{
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_NC_n, 0, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalNotCarryWithOffset5ShouldGoForwardBy5WhenFlagIsUnset)
{
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_NC_n, 5, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalNotCarryWithOffsetMinus5ShouldGoBackwardBy5WhenFlagIsUnset)
{
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_NC_n, -5, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalNotCarryWithOffset127ShouldGoForwardBy127WhenFlagIsUnset)
{
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_NC_n, 127, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalNotCarryWithOffsetMinus127ShouldGoBackwardBy127WhenFlagIsUnset)
{
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_NC_n, -127, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalZeroWithOffset0ShouldNotChangeWhenFlagIsSet)
{
    cpu.setFlag(CPU::ZERO);
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_Z_n, 0, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalZeroWithOffset5ShouldGoForwardBy5WhenFlagIsSet)
{
    cpu.setFlag(CPU::ZERO);
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_Z_n, 5, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalZeroWithOffsetMinus5ShouldGoBackwardBy5WhenFlagIsSet)
{
    cpu.setFlag(CPU::ZERO);
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_Z_n, -5, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalZeroWithOffset127ShouldGoForwardBy127WhenFlagIsSet)
{
    cpu.setFlag(CPU::ZERO);
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_Z_n, 127, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalZeroWithOffsetMinus127ShouldGoBackwardBy127WhenFlagIsSet)
{
    cpu.setFlag(CPU::ZERO);
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_Z_n, -127, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalNotZeroWithOffset0ShouldNotChangeWhenFlagIsUnset)
{
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_NZ_n, 0, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalNotZeroWithOffset5ShouldGoForwardBy5WhenFlagIsUnset)
{
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_NZ_n, 5, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalNotZeroWithOffsetMinus5ShouldGoBackwardBy5WhenFlagIsUnset)
{
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_NZ_n, -5, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalNotZeroWithOffset127ShouldGoForwardBy127WhenFlagIsUnset)
{
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_NZ_n, 127, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, JumpRelativeConditionalNotZeroWithOffsetMinus127ShouldGoBackwardBy127WhenFlagIsUnset)
{
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_NZ_n, -127, true);
}