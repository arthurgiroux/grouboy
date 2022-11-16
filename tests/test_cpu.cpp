#include "cpu.hpp"
#include "instructions.hpp"
#include "utils.hpp"
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

	void testOp8BitsRegisterTo8BitsRegister(byte instruction, byte& reg, byte startValue, byte& regOther,
	                                        byte otherValue, byte expectedResult, bool carryFlag,
	                                        bool expectedHalfCarryFlag, bool expectedCarryFlag,
	                                        bool expectedSubstractionFlag)
	{
		cpu.setProgramCounter(0x00);
		reg = startValue;
		regOther = otherValue;
		cpu.setCarryFlag(carryFlag);
		mmu.write(cpu.getProgramCounter(), instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 1);
		ASSERT_EQ(reg, expectedResult);
		ASSERT_EQ(cpu.isFlagSet(CPU::HALF_CARRY), expectedHalfCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::CARRY), expectedCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::SUBSTRACTION), expectedSubstractionFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::ZERO), expectedResult == 0);
		ASSERT_EQ(cpu.getProgramCounter(), 1);
	}

	void testOpFromMemoryToRegister(byte instruction, byte& reg, byte startValue, byte regAddrMsb, byte regAddrLsb,
	                                byte value, byte expectedResult, bool carryFlag, bool expectedHalfCarryFlag,
	                                bool expectedCarryFlag, bool expectedSubstractionFlag)
	{
		cpu.setProgramCounter(0x00);
		reg = startValue;
		mmu.write(createAddrFromHighAndLowBytes(regAddrMsb, regAddrLsb), value);
		mmu.write(cpu.getProgramCounter(), instruction);
		cpu.setCarryFlag(carryFlag);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_EQ(reg, expectedResult);
		ASSERT_EQ(cpu.isFlagSet(CPU::HALF_CARRY), expectedHalfCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::CARRY), expectedCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::SUBSTRACTION), expectedSubstractionFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::ZERO), expectedResult == 0);
		ASSERT_EQ(cpu.getProgramCounter(), 1);
	}

	void testOpImmediateValueToRegister(byte instruction, byte& reg, byte startValue, byte value, byte expectedResult,
	                                    bool carryFlag, bool expectedHalfCarryFlag, bool expectedCarryFlag,
	                                    bool expectedSubstractionFlag)
	{
		cpu.setProgramCounter(0x00);
		reg = startValue;
		mmu.write(cpu.getProgramCounter(), instruction);
		mmu.write(cpu.getProgramCounter() + 1, value);
		cpu.setCarryFlag(carryFlag);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_EQ(reg, expectedResult);
		ASSERT_EQ(cpu.isFlagSet(CPU::HALF_CARRY), expectedHalfCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::CARRY), expectedCarryFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::SUBSTRACTION), expectedSubstractionFlag);
		ASSERT_EQ(cpu.isFlagSet(CPU::ZERO), expectedResult == 0);
		ASSERT_EQ(cpu.getProgramCounter(), 2);
	}

	/*
	void testAdd8BitsRegisterTo8BitsRegister(byte instruction, byte& reg, byte& otherReg)
	{
	    testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0x00, false, false, false);
	    testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0x05, false, false, false);
	    testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x0F, otherReg, 0x05, 0x14, false, true, false);
	    testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x05, 0x04, false, true, true);
	    testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x01, 0x00, false, true, true);
	    testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0xF0, otherReg, 0xF0, 0xE0, false, false, true);
	}

	void testAdd8BitsRegisterAndCarryTo8BitsRegister(byte instruction, byte& reg, byte& otherReg)
	{
	    testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0x00, false, false, false);
	    testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0x01, true, false, false);
	    testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0x05, false, false, false);
	    testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0x06, true, false, false);
	    testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x0F, otherReg, 0x00, 0x10, true, true, false);
	    testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0x0F, otherReg, 0x05, 0x14, false, true, false);
	    testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x05, 0x04, false, true, true);
	    testAdd8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x00, 0x00, true, true, true);
	}

	void testSub8BitsRegisterTo8BitsRegister(byte instruction, byte& reg, byte startValue, byte& otherReg, byte value,
	                                         byte expectedResult, bool carryFlag, bool expectedHalfCarryFlag,
	                                         bool expectedCarryFlag)
	{
	    testOp8BitsRegisterTo8BitsRegister(instruction, reg, startValue, otherReg, value, expectedResult, carryFlag,
	                                       expectedHalfCarryFlag, expectedCarryFlag, true);
	}

	void testSubValueFromMemoryToRegister(byte instruction, byte& reg, byte startValue, byte regAddrMsb,
	                                      byte regAddrLsb, byte value, byte expectedResult, bool carryFlag,
	                                      bool expectedHalfCarryFlag, bool expectedCarryFlag)
	{
	    testOpFromMemoryToRegister(instruction, reg, startValue, regAddrMsb, regAddrLsb, value, expectedResult,
	                               carryFlag, expectedHalfCarryFlag, expectedCarryFlag, true);
	}

	void testSubImmediateValueToRegister(byte instruction, byte& reg, byte startValue, byte value, byte expectedResult,
	                                     bool carryFlag, bool expectedHalfCarryFlag, bool expectedCarryFlag)
	{
	    testOpImmediateValueToRegister(instruction, reg, startValue, value, expectedResult, carryFlag,
	                                   expectedHalfCarryFlag, expectedCarryFlag, true);
	}

	void testSub8BitsRegisterTo8BitsRegister(byte instruction, byte& reg, byte& otherReg)
	{
	    testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0x00, false, false, false);
	    testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0xFB, false, true, true);
	    testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x10, otherReg, 0x05, 0x0B, false, true, false);
	    testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x05, 0xFA, false, false, false);
	    testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x01, 0xFF, false, true, true);
	}

	void testSub8BitsRegisterAndCarryTo8BitsRegister(byte instruction, byte& reg, byte& otherReg)
	{
	    testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0x00, false, false, false);
	    testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x00, 0xFF, true, true, true);
	    testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0xFB, false, true, true);
	    testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x00, otherReg, 0x05, 0xFA, true, true, true);
	    testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x10, otherReg, 0x00, 0x0F, true, true, false);
	    testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0x10, otherReg, 0x05, 0x0B, false, true, false);
	    testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x05, 0xFA, false, false, false);
	    testSub8BitsRegisterTo8BitsRegister(instruction, reg, 0xFF, otherReg, 0x00, 0xFE, true, false, false);
	}
	 */
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

/*

TEST_F(CpuInstructionTest, InstructionSub8BitsRegisterTogether)
{
    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_B, cpu.getRegisterA(), cpu.b);
    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_C, cpu.getRegisterA(), cpu.c);
    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_D, cpu.getRegisterA(), cpu.d);
    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_E, cpu.getRegisterA(), cpu.e);
    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_H, cpu.getRegisterA(), cpu.h);
    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_L, cpu.getRegisterA(), cpu.l);

    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_A, cpu.getRegisterA(), 0x00, cpu.getRegisterA(),
                                        0x00, 0x00, false, false, false);
    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_A, cpu.getRegisterA(), 0xFF, cpu.getRegisterA(),
                                        0xFF, 0x00, false, false, false);
    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SUB_A_A, cpu.getRegisterA(), 0x10, cpu.getRegisterA(),
                                        0x10, 0x00, false, false, false);
}

TEST_F(CpuInstructionTest, InstructionSubValueFromMemoryToRegister)
{
    cpu.h = 0x12;
    cpu.l = 0x23;
    testSubValueFromMemoryToRegister(standardInstructions::SUB_A_HLm, cpu.getRegisterA(), 0x00, cpu.h, cpu.l, 0x00,
                                     0x00, false, false, false);
    testSubValueFromMemoryToRegister(standardInstructions::SUB_A_HLm, cpu.getRegisterA(), 0x00, cpu.h, cpu.l, 0x05,
                                     0xFB, false, true, true);
    testSubValueFromMemoryToRegister(standardInstructions::SUB_A_HLm, cpu.getRegisterA(), 0x10, cpu.h, cpu.l, 0x05,
                                     0x0B, false, true, false);
    testSubValueFromMemoryToRegister(standardInstructions::SUB_A_HLm, cpu.getRegisterA(), 0xFF, cpu.h, cpu.l, 0x05,
                                     0xFA, false, false, false);
    testSubValueFromMemoryToRegister(standardInstructions::SUB_A_HLm, cpu.getRegisterA(), 0x00, cpu.h, cpu.l, 0x01,
                                     0xFF, false, true, true);
}

TEST_F(CpuInstructionTest, InstructionSubImmediateValueToRegister)
{
    testSubImmediateValueToRegister(standardInstructions::SUB_A_n, cpu.getRegisterA(), 0x00, 0x00, 0x00, false, false,
                                    false);
    testSubImmediateValueToRegister(standardInstructions::SUB_A_n, cpu.getRegisterA(), 0x00, 0x05, 0xFB, false, true,
                                    true);
    testSubImmediateValueToRegister(standardInstructions::SUB_A_n, cpu.getRegisterA(), 0x10, 0x05, 0x0B, false, true,
                                    false);
    testSubImmediateValueToRegister(standardInstructions::SUB_A_n, cpu.getRegisterA(), 0xFF, 0x05, 0xFA, false, false,
                                    false);
    testSubImmediateValueToRegister(standardInstructions::SUB_A_n, cpu.getRegisterA(), 0x00, 0x01, 0xFF, false, true,
                                    true);
}

TEST_F(CpuInstructionTest, InstructionSub8BitsRegisterAndCarryToAnother)
{
    testSub8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::SBC_A_B, cpu.getRegisterA(), cpu.b);
    testSub8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::SBC_A_C, cpu.getRegisterA(), cpu.c);
    testSub8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::SBC_A_D, cpu.getRegisterA(), cpu.d);
    testSub8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::SBC_A_E, cpu.getRegisterA(), cpu.e);
    testSub8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::SBC_A_H, cpu.getRegisterA(), cpu.h);
    testSub8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::SBC_A_L, cpu.getRegisterA(), cpu.l);

    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SBC_A_A, cpu.getRegisterA(), 0x00, cpu.getRegisterA(),
                                        0x00, 0x00, false, false, false);
    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SBC_A_A, cpu.getRegisterA(), 0x00, cpu.getRegisterA(),
                                        0x00, 0xFF, true, true, true);

    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SBC_A_A, cpu.getRegisterA(), 0x10, cpu.getRegisterA(),
                                        0x10, 0x00, false, false, false);
    testSub8BitsRegisterTo8BitsRegister(standardInstructions::SBC_A_A, cpu.getRegisterA(), 0x10, cpu.getRegisterA(),
                                        0x10, 0xFF, true, true, true);
}
*/

TEST_F(CpuTest, GetSetStackPointerShouldChangeValue)
{
	uint16_t value = 0xCAFE;
	ASSERT_EQ(cpu.getStackPointer(), 0x00);
	cpu.setStackPointer(value);
	ASSERT_EQ(cpu.getStackPointer(), value);
}

TEST_F(CpuTest, GetSetProgramCounterShouldChangeValue)
{
	uint16_t value = 0xCAFE;
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