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

	void addTwo8BitsRegisterToTwo8BitsRegister(byte instruction, byte& resultRegMsb, byte& resultRegLsb,
	                                           byte& valueRegMsb, byte& valueRegLsb, std::vector<uint16_t>& startValues,
	                                           std::vector<uint16_t>& addValues, std::vector<int>& expectedFlags)
	{
		for (size_t i = 0; i < startValues.size(); ++i)
		{
			uint16_t startValue = startValues[i];
			uint16_t addValue = addValues[i];
			resultRegLsb = startValue & 0xFF;
			resultRegMsb = (startValue >> 8) & 0xFF;
			valueRegLsb = addValue & 0xFF;
			valueRegMsb = (addValue >> 8) & 0xFF;
			uint32_t expectedValue = startValue + addValue;
			mmu.write(cpu.getProgramCounter(), instruction);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, 2);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), ((expectedFlags[i] & CPU::CpuFlags::CARRY) > 0));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), ((expectedFlags[i] & CPU::CpuFlags::HALF_CARRY) > 0));
		}
	}

	void add16BitsRegisterToTwo8BitsRegister(byte instruction, byte& resultRegMsb, byte& resultRegLsb,
	                                         uint16_t& valueReg, std::vector<uint16_t>& startValues,
	                                         std::vector<uint16_t>& addValues, std::vector<int>& expectedFlags)
	{
		for (size_t i = 0; i < startValues.size(); ++i)
		{
			uint16_t startValue = startValues[i];
			uint16_t addValue = addValues[i];
			resultRegLsb = startValue & 0xFF;
			resultRegMsb = (startValue >> 8) & 0xFF;
			valueReg = addValue;
			uint32_t expectedValue = startValue + addValue;
			mmu.write(cpu.getProgramCounter(), instruction);
			int ticks = cpu.fetchDecodeAndExecute();
			ASSERT_EQ(ticks, 2);
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
			ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), ((expectedFlags[i] & CPU::CpuFlags::CARRY) > 0));
			ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), ((expectedFlags[i] & CPU::CpuFlags::HALF_CARRY) > 0));
		}
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

	void testAdd8BitsRegisterTo8BitsRegister(byte instruction, byte& reg, byte startValue, byte& otherReg, byte value,
	                                         byte expectedResult, bool carryFlag, bool expectedHalfCarryFlag,
	                                         bool expectedCarryFlag)
	{
		testOp8BitsRegisterTo8BitsRegister(instruction, reg, startValue, otherReg, value, expectedResult, carryFlag,
		                                   expectedHalfCarryFlag, expectedCarryFlag, false);
	}

	void testAddValueFromMemoryToRegister(byte instruction, byte& reg, byte startValue, byte regAddrMsb,
	                                      byte regAddrLsb, byte value, byte expectedResult, bool carryFlag,
	                                      bool expectedHalfCarryFlag, bool expectedCarryFlag)
	{
		testOpFromMemoryToRegister(instruction, reg, startValue, regAddrMsb, regAddrLsb, value, expectedResult,
		                           carryFlag, expectedHalfCarryFlag, expectedCarryFlag, false);
	}

	void testAddImmediateValueToRegister(byte instruction, byte& reg, byte startValue, byte value, byte expectedResult,
	                                     bool carryFlag, bool expectedHalfCarryFlag, bool expectedCarryFlag)
	{
		testOpImmediateValueToRegister(instruction, reg, startValue, value, expectedResult, carryFlag,
		                               expectedHalfCarryFlag, expectedCarryFlag, false);
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

	void testAndOperationWithRegister(byte instruction, byte startValue, byte& reg, byte value, byte expectedResult)
	{
	    testOp8BitsRegisterTo8BitsRegister(instruction, cpu.getRegisterA(), startValue, reg, value, expectedResult,
	                                       true, true, false, false);
	}

	void testAndOperationWithRegister(byte instruction, byte& reg)
	{
	    testAndOperationWithRegister(instruction, 0x00, reg, 0x00, 0x00);
	    testAndOperationWithRegister(instruction, 0x0F, reg, 0x00, 0x00);
	    testAndOperationWithRegister(instruction, 0x00, reg, 0x0F, 0x00);
	    testAndOperationWithRegister(instruction, 0x0F, reg, 0x0F, 0x0F);
	    testAndOperationWithRegister(instruction, 0xF0, reg, 0xF0, 0xF0);
	    testAndOperationWithRegister(instruction, 0x01, reg, 0x01, 0x01);
	    testAndOperationWithRegister(instruction, 0x10, reg, 0x10, 0x10);
	}


	void testAndOperationWithImmediateValue(byte instruction, byte startValue, byte value, byte expectedResult)
	{
	    testOpImmediateValueToRegister(instruction, cpu.getRegisterA(), startValue, value, expectedResult, true, true,
	                                   false, false);
	}

	void testAndOperationWithMemory(byte instruction, byte startValue, byte regAddrMsb, byte regAddrLsb, byte value,
	                                byte expectedResult)
	{
	    testOpFromMemoryToRegister(instruction, cpu.getRegisterA(), startValue, regAddrMsb, regAddrLsb, value,
	                               expectedResult, true, true, false, false);
	}

	void testXorOperationWithRegister(byte instruction, byte startValue, byte& reg, byte value, byte expectedResult)
	{
	    testOp8BitsRegisterTo8BitsRegister(instruction, cpu.getRegisterA(), startValue, reg, value, expectedResult,
	                                       true, false, false, false);
	}


	void testXorOperationWithRegister(byte instruction, byte& reg)
	{
	    testXorOperationWithRegister(instruction, 0x00, reg, 0x00, 0x00);
	    testXorOperationWithRegister(instruction, 0x0F, reg, 0x00, 0x0F);
	    testXorOperationWithRegister(instruction, 0x00, reg, 0x0F, 0x0F);
	    testXorOperationWithRegister(instruction, 0x0F, reg, 0x0F, 0x00);
	    testXorOperationWithRegister(instruction, 0xF0, reg, 0xF0, 0x00);
	    testXorOperationWithRegister(instruction, 0x01, reg, 0x01, 0x00);
	    testXorOperationWithRegister(instruction, 0x10, reg, 0x10, 0x00);
	    testXorOperationWithRegister(instruction, 0x01, reg, 0x00, 0x01);
	    testXorOperationWithRegister(instruction, 0x00, reg, 0x01, 0x01);
	    testXorOperationWithRegister(instruction, 0xFF, reg, 0xFF, 0x00);
	}


	void testXorOperationWithImmediateValue(byte instruction, byte startValue, byte value, byte expectedResult)
	{
	    testOpImmediateValueToRegister(instruction, cpu.getRegisterA(), startValue, value, expectedResult, true, false,
	                                   false, false);
	}

	void testXorOperationWithMemory(byte instruction, byte startValue, byte regAddrMsb, byte regAddrLsb, byte value,
	                                byte expectedResult)
	{
	    testOpFromMemoryToRegister(instruction, cpu.getRegisterA(), startValue, regAddrMsb, regAddrLsb, value,
	                               expectedResult, true, false, false, false);
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

TEST_F(CpuInstructionTest, InstructionAddTwo8BitsRegisterToTwo8BitsRegister)
{
    std::vector<uint16_t> startValues = {0x0000, 0x00FF, 0xFFFF, 0x0FFF, 0xF000};
    std::vector<uint16_t> addValues = {0x0012, 0x0012, 0x0050, 0x0012, 0xF000};
    std::vector<int> expectedFlags = {0, 0, CPU::CARRY | CPU::HALF_CARRY, CPU::HALF_CARRY, CPU::CARRY};
    addTwo8BitsRegisterToTwo8BitsRegister(standardInstructions::ADD_HL_BC, cpu.h, cpu.l, cpu.b, cpu.c, startValues,
                                          addValues, expectedFlags);
    addTwo8BitsRegisterToTwo8BitsRegister(standardInstructions::ADD_HL_DE, cpu.h, cpu.l, cpu.d, cpu.e, startValues,
                                          addValues, expectedFlags);
}

TEST_F(CpuInstructionTest, InstructionAdd16BitsRegisterToTwo8BitsRegister)
{
    std::vector<uint16_t> startValues = {0x0000, 0x00FF, 0xFFFF, 0x0FFF, 0xF000};
    std::vector<uint16_t> addValues = {0x0012, 0x0012, 0x0050, 0x0012, 0xF000};
    std::vector<int> expectedFlags = {0, 0, CPU::CARRY | CPU::HALF_CARRY, CPU::HALF_CARRY, CPU::CARRY};
    add16BitsRegisterToTwo8BitsRegister(standardInstructions::ADD_HL_SP, cpu.h, cpu.l, cpu.sp, startValues, addValues,
                                        expectedFlags);
}

TEST_F(CpuInstructionTest, InstructionAdd8BitsRegisterToSameRegisters)
{
    std::vector<uint16_t> startValues = {0x0012, 0x00FF, 0x0FFF, 0xF000};
    std::vector<uint16_t> addValues = {0x0012, 0x00FF, 0x0FFF, 0xF000};
    std::vector<int> expectedFlags = {0, 0, CPU::HALF_CARRY, CPU::CARRY};
    addTwo8BitsRegisterToTwo8BitsRegister(standardInstructions::ADD_HL_HL, cpu.h, cpu.l, cpu.h, cpu.l, startValues,
                                          addValues, expectedFlags);
}

TEST_F(CpuInstructionTest, InstructionAdd8BitsRegisterTogether)
{
    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_B, cpu.getRegisterA(), cpu.b);
    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_C, cpu.getRegisterA(), cpu.c);
    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_D, cpu.getRegisterA(), cpu.d);
    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_E, cpu.getRegisterA(), cpu.e);
    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_H, cpu.getRegisterA(), cpu.h);
    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_L, cpu.getRegisterA(), cpu.l);

    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_A, cpu.getRegisterA(), 0x00, cpu.getRegisterA(),
                                        0x00, 0x00, false, false, false);
    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_A, cpu.getRegisterA(), 0x0F, cpu.getRegisterA(),
                                        0x0F, 0x1E, false, true, false);
    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_A, cpu.getRegisterA(), 0xFF, cpu.getRegisterA(),
                                        0xFF, 0xFE, false, true, true);
    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADD_A_A, cpu.getRegisterA(), 0xF0, cpu.getRegisterA(),
                                        0xF0, 0xE0, false, false, true);
}

TEST_F(CpuInstructionTest, InstructionAddValueFromMemoryToRegister)
{
    cpu.h = 0x12;
    cpu.l = 0x23;
    testAddValueFromMemoryToRegister(standardInstructions::ADD_A_HLm, cpu.getRegisterA(), 0x00, cpu.h, cpu.l, 0x00,
                                     0x00, false, false, false);
    testAddValueFromMemoryToRegister(standardInstructions::ADD_A_HLm, cpu.getRegisterA(), 0x00, cpu.h, cpu.l, 0x05,
                                     0x05, false, false, false);
    testAddValueFromMemoryToRegister(standardInstructions::ADD_A_HLm, cpu.getRegisterA(), 0x0F, cpu.h, cpu.l, 0x05,
                                     0x14, false, true, false);
    testAddValueFromMemoryToRegister(standardInstructions::ADD_A_HLm, cpu.getRegisterA(), 0xFF, cpu.h, cpu.l, 0x05,
                                     0x04, false, true, true);
    testAddValueFromMemoryToRegister(standardInstructions::ADD_A_HLm, cpu.getRegisterA(), 0xFF, cpu.h, cpu.l, 0x01,
                                     0x00, false, true, true);
    testAddValueFromMemoryToRegister(standardInstructions::ADD_A_HLm, cpu.getRegisterA(), 0xF0, cpu.h, cpu.l, 0xF0,
                                     0xE0, false, false, true);
}

TEST_F(CpuInstructionTest, InstructionAddImmediateValueToRegister)
{
    testAddImmediateValueToRegister(standardInstructions::ADD_A_n, cpu.getRegisterA(), 0x00, 0x00, 0x00, false, false,
                                    false);
    testAddImmediateValueToRegister(standardInstructions::ADD_A_n, cpu.getRegisterA(), 0x00, 0x05, 0x05, false, false,
                                    false);
    testAddImmediateValueToRegister(standardInstructions::ADD_A_n, cpu.getRegisterA(), 0x0F, 0x05, 0x14, false, true,
                                    false);
    testAddImmediateValueToRegister(standardInstructions::ADD_A_n, cpu.getRegisterA(), 0xFF, 0x05, 0x04, false, true,
                                    true);
    testAddImmediateValueToRegister(standardInstructions::ADD_A_n, cpu.getRegisterA(), 0xFF, 0x01, 0x00, false, true,
                                    true);
    testAddImmediateValueToRegister(standardInstructions::ADD_A_n, cpu.getRegisterA(), 0xF0, 0xF0, 0xE0, false, false,
                                    true);
}

TEST_F(CpuInstructionTest, InstructionAdd8BitsRegisterAndCarryToAnother)
{
    testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_B, cpu.getRegisterA(), cpu.b);
    testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_C, cpu.getRegisterA(), cpu.c);
    testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_D, cpu.getRegisterA(), cpu.d);
    testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_E, cpu.getRegisterA(), cpu.e);
    testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_H, cpu.getRegisterA(), cpu.h);
    testAdd8BitsRegisterAndCarryTo8BitsRegister(standardInstructions::ADC_A_L, cpu.getRegisterA(), cpu.l);

    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.getRegisterA(), 0x00, cpu.getRegisterA(),
                                        0x00, 0x00, false, false, false);
    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.getRegisterA(), 0x00, cpu.getRegisterA(),
                                        0x00, 0x01, true, false, false);

    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.getRegisterA(), 0x0F, cpu.getRegisterA(),
                                        0x0F, 0x1E, false, true, false);
    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.getRegisterA(), 0x0F, cpu.getRegisterA(),
                                        0x0F, 0x1F, true, true, false);

    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.getRegisterA(), 0xFF, cpu.getRegisterA(),
                                        0xFF, 0xFE, false, true, true);
    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.getRegisterA(), 0xFF, cpu.getRegisterA(),
                                        0xFF, 0xFF, true, true, true);

    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.getRegisterA(), 0xF0, cpu.getRegisterA(),
                                        0xF0, 0xE0, false, false, true);
    testAdd8BitsRegisterTo8BitsRegister(standardInstructions::ADC_A_A, cpu.getRegisterA(), 0xF0, cpu.getRegisterA(),
                                        0xF0, 0xE1, true, false, true);
}

TEST_F(CpuInstructionTest, InstructionAddValueFromMemoryAndCarryToRegister)
{
    cpu.h = 0x12;
    cpu.l = 0x23;
    testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.getRegisterA(), 0x00, cpu.h, cpu.l, 0x00,
                                     0x00, false, false, false);
    testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.getRegisterA(), 0x00, cpu.h, cpu.l, 0x00,
                                     0x01, true, false, false);
    testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.getRegisterA(), 0x00, cpu.h, cpu.l, 0x05,
                                     0x05, false, false, false);
    testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.getRegisterA(), 0x0F, cpu.h, cpu.l, 0x05,
                                     0x14, false, true, false);
    testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.getRegisterA(), 0x0F, cpu.h, cpu.l, 0x05,
                                     0x15, true, true, false);

    testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.getRegisterA(), 0xFF, cpu.h, cpu.l, 0x05,
                                     0x04, false, true, true);
    testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.getRegisterA(), 0xFF, cpu.h, cpu.l, 0x05,
                                     0x05, true, true, true);

    testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.getRegisterA(), 0xFF, cpu.h, cpu.l, 0x01,
                                     0x00, false, true, true);
    testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.getRegisterA(), 0xFF, cpu.h, cpu.l, 0x00,
                                     0x00, true, true, true);

    testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.getRegisterA(), 0xF0, cpu.h, cpu.l, 0xF0,
                                     0xE0, false, false, true);
    testAddValueFromMemoryToRegister(standardInstructions::ADC_A_HLm, cpu.getRegisterA(), 0xF0, cpu.h, cpu.l, 0xF0,
                                     0xE1, true, false, true);
}

TEST_F(CpuInstructionTest, InstructionAddImmediateValueAndCarryToRegister)
{
    testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.getRegisterA(), 0x00, 0x00, 0x00, false, false,
                                    false);
    testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.getRegisterA(), 0x00, 0x00, 0x01, true, false,
                                    false);

    testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.getRegisterA(), 0x00, 0x05, 0x05, false, false,
                                    false);
    testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.getRegisterA(), 0x00, 0x05, 0x06, true, false,
                                    false);

    testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.getRegisterA(), 0x0F, 0x05, 0x14, false, true,
                                    false);
    testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.getRegisterA(), 0x0F, 0x05, 0x15, true, true,
                                    false);

    testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.getRegisterA(), 0xFF, 0x05, 0x04, false, true,
                                    true);
    testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.getRegisterA(), 0xFF, 0x05, 0x05, true, true,
                                    true);

    testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.getRegisterA(), 0xFF, 0x01, 0x00, false, true,
                                    true);
    testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.getRegisterA(), 0xFF, 0x00, 0x00, true, true,
                                    true);

    testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.getRegisterA(), 0xF0, 0xF0, 0xE0, false, false,
                                    true);
    testAddImmediateValueToRegister(standardInstructions::ADC_A_n, cpu.getRegisterA(), 0xF0, 0xEF, 0xE0, true, true,
                                    true);
}

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

TEST_F(CpuInstructionTest, InstructionAndBetweenRegisterAndAccumulator)
{
    testAndOperationWithRegister(standardInstructions::AND_B, cpu.b);
    testAndOperationWithRegister(standardInstructions::AND_C, cpu.c);
    testAndOperationWithRegister(standardInstructions::AND_D, cpu.d);
    testAndOperationWithRegister(standardInstructions::AND_E, cpu.e);
    testAndOperationWithRegister(standardInstructions::AND_H, cpu.h);
    testAndOperationWithRegister(standardInstructions::AND_L, cpu.l);

    testAndOperationWithRegister(standardInstructions::AND_A, 0x00, cpu.getRegisterA(), 0x00, 0x00);
    testAndOperationWithRegister(standardInstructions::AND_A, 0x0F, cpu.getRegisterA(), 0x0F, 0x0F);
    testAndOperationWithRegister(standardInstructions::AND_A, 0xF0, cpu.getRegisterA(), 0xF0, 0xF0);
    testAndOperationWithRegister(standardInstructions::AND_A, 0x01, cpu.getRegisterA(), 0x01, 0x01);
    testAndOperationWithRegister(standardInstructions::AND_A, 0x10, cpu.getRegisterA(), 0x10, 0x10);
}

TEST_F(CpuInstructionTest, InstructionAndBetweenRegisterAndImmediateValue)
{
    testAndOperationWithImmediateValue(standardInstructions::AND_n, 0x00, 0x00, 0x00);
    testAndOperationWithImmediateValue(standardInstructions::AND_n, 0x0F, 0x00, 0x00);
    testAndOperationWithImmediateValue(standardInstructions::AND_n, 0x00, 0x0F, 0x00);
    testAndOperationWithImmediateValue(standardInstructions::AND_n, 0x0F, 0x0F, 0x0F);
    testAndOperationWithImmediateValue(standardInstructions::AND_n, 0xF0, 0xF0, 0xF0);
    testAndOperationWithImmediateValue(standardInstructions::AND_n, 0x01, 0x01, 0x01);
    testAndOperationWithImmediateValue(standardInstructions::AND_n, 0x10, 0x10, 0x10);
}

TEST_F(CpuInstructionTest, InstructionAndBetweenRegisterAndMemory)
{
    cpu.h = 0x12;
    cpu.l = 0x23;
    testAndOperationWithMemory(standardInstructions::AND_HLm, 0x00, cpu.h, cpu.l, 0x00, 0x00);
    testAndOperationWithMemory(standardInstructions::AND_HLm, 0x0F, cpu.h, cpu.l, 0x00, 0x00);
    testAndOperationWithMemory(standardInstructions::AND_HLm, 0x00, cpu.h, cpu.l, 0x0F, 0x00);
    testAndOperationWithMemory(standardInstructions::AND_HLm, 0x0F, cpu.h, cpu.l, 0x0F, 0x0F);
    testAndOperationWithMemory(standardInstructions::AND_HLm, 0xF0, cpu.h, cpu.l, 0xF0, 0xF0);
    testAndOperationWithMemory(standardInstructions::AND_HLm, 0x01, cpu.h, cpu.l, 0x01, 0x01);
    testAndOperationWithMemory(standardInstructions::AND_HLm, 0x10, cpu.h, cpu.l, 0x10, 0x10);
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