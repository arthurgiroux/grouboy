#include "cpu.hpp"
#include "instructions.hpp"
#include "utils.hpp"

#include <gtest/gtest.h>

using namespace utils;

class CpuInstructions16BitsLoadStoreMoveTest : public ::testing::Test
{
  protected:
	void assertPopIntoMemoryIsPerformed(int instruction, byte msbValue, byte lsbValue)
	{
		uint16_t startSp = 0xFFFC;
		cpu.setStackPointer(startSp);
		mmu.write(cpu.getStackPointer(), lsbValue);
		mmu.write(cpu.getStackPointer() + 1, msbValue);
		mmu.write(cpu.getProgramCounter(), instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 3);
		ASSERT_EQ(cpu.getProgramCounter(), 1);
		ASSERT_EQ(cpu.getStackPointer(), startSp + 2);
	}

	void assertPushChangedStackPointerValue(int instruction, byte expectedMsbValue, byte expectedLsbValue)
	{
		uint16_t startSp = 0x1234;
		cpu.setStackPointer(startSp);
		mmu.write(cpu.getProgramCounter(), instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 4);
		mmu.write(cpu.getStackPointer(), expectedLsbValue);
		mmu.write(cpu.getStackPointer() + 1, expectedMsbValue);
		ASSERT_EQ(cpu.getStackPointer(), startSp - 2);
		ASSERT_EQ(mmu.read(cpu.getStackPointer()), expectedLsbValue);
		ASSERT_EQ(mmu.read(cpu.getStackPointer() + 1), expectedMsbValue);
	}

	void assertloadSPAndOffsetInHLGivesExpectedResult(uint16_t spValue, sbyte offset, uint16_t expectedResult)
	{
		cpu.setStackPointer(spValue);
		mmu.write(cpu.getProgramCounter(), standardInstructions::LDHL_SP_d);
		mmu.write(cpu.getProgramCounter() + 1, offset);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 3);
		ASSERT_EQ(cpu.getProgramCounter(), 2);
		ASSERT_EQ(cpu.getRegisterH(), getMsbFromWord(expectedResult));
		ASSERT_EQ(cpu.getRegisterL(), getLsbFromWord(expectedResult));
	}

	MMU mmu;
	CPU cpu = CPU(mmu);
};

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, PopRegisterBCShouldPutSPInBC)
{
	int msbValue = 0x3C;
	int lsbValue = 0x5F;
	assertPopIntoMemoryIsPerformed(standardInstructions::POP_BC, msbValue, lsbValue);
	ASSERT_EQ(cpu.getRegisterB(), msbValue);
	ASSERT_EQ(cpu.getRegisterC(), lsbValue);
}

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, PopRegisterDEShouldPutSPInDE)
{
	int msbValue = 0x3C;
	int lsbValue = 0x5F;
	assertPopIntoMemoryIsPerformed(standardInstructions::POP_DE, msbValue, lsbValue);
	ASSERT_EQ(cpu.getRegisterD(), msbValue);
	ASSERT_EQ(cpu.getRegisterE(), lsbValue);
}

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, PopRegisterHLShouldPutSPInHL)
{
	int msbValue = 0x3C;
	int lsbValue = 0x5F;
	assertPopIntoMemoryIsPerformed(standardInstructions::POP_HL, msbValue, lsbValue);
	ASSERT_EQ(cpu.getRegisterH(), msbValue);
	ASSERT_EQ(cpu.getRegisterL(), lsbValue);
}

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, PopRegisterAFShouldPutSPInAF)
{
	int msbValue = 0x3C;
	int lsbValue = 0x5F;
	assertPopIntoMemoryIsPerformed(standardInstructions::POP_AF, msbValue, lsbValue);
	ASSERT_EQ(cpu.getRegisterA(), msbValue);
	ASSERT_EQ(cpu.getFlag(), lsbValue);
}

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, PushRegisterBCShouldPutBCInSP)
{
	int msbValue = 0x3C;
	int lsbValue = 0x5F;
	cpu.setRegisterB(msbValue);
	cpu.setRegisterC(lsbValue);
	assertPushChangedStackPointerValue(standardInstructions::PUSH_BC, msbValue, lsbValue);
}

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, PushRegisterDEShouldPutDEInSP)
{
	int msbValue = 0x3C;
	int lsbValue = 0x5F;
	cpu.setRegisterD(msbValue);
	cpu.setRegisterE(lsbValue);
	assertPushChangedStackPointerValue(standardInstructions::PUSH_DE, msbValue, lsbValue);
}

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, PushRegisterHLShouldPutHLInSP)
{
	int msbValue = 0x3C;
	int lsbValue = 0x5F;
	cpu.setRegisterH(msbValue);
	cpu.setRegisterL(lsbValue);
	assertPushChangedStackPointerValue(standardInstructions::PUSH_HL, msbValue, lsbValue);
}

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, PushRegisterAFShouldPutAFInSP)
{
	int msbValue = 0x3C;
	int lsbValue = 0x5F;
	cpu.setRegisterA(msbValue);
	cpu.setFlag(static_cast<CPU::CpuFlags>(lsbValue));
	assertPushChangedStackPointerValue(standardInstructions::PUSH_AF, msbValue, lsbValue);
}

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, LoadSPAndPositiveOffsetInHLGivesExpectedResult)
{
	assertloadSPAndOffsetInHLGivesExpectedResult(0x1234, 0x22, 0x1256);
}

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, LoadSPAndNegativeOffsetInHLGivesExpectedResult)
{
	assertloadSPAndOffsetInHLGivesExpectedResult(0x1234, static_cast<sbyte>(0xDE), 0x1212);
}

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, LoadHLInSPGivesExpectedResult)
{
	cpu.setRegisterH(0x12);
	cpu.setRegisterL(0x34);
    mmu.write(cpu.getProgramCounter(), standardInstructions::LD_SP_HL);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 2);
    ASSERT_EQ(cpu.getStackPointer(), 0x1234);
	ASSERT_EQ(cpu.getProgramCounter(), 1);
}