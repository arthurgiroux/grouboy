#include "cpu.hpp"
#include "instructions.hpp"
#include "utils.hpp"

#include <gtest/gtest.h>

using namespace utils;

class CpuInstructions8BitsLoadStoreMoveTest : public ::testing::Test
{
  protected:
	void assertLoadInMemoryWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_EQ(cpu.getFlag(), 0x00);
		ASSERT_EQ(cpu.getProgramCounter(), 0x01);
	}

	void assertLoadImmediateValueWasPerformed(byte instruction, byte value)
	{
		mmu.write(cpu.getProgramCounter(), instruction);
		mmu.write(cpu.getProgramCounter() + 1, value);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_EQ(cpu.getFlag(), 0x00);
		ASSERT_EQ(cpu.getProgramCounter(), 2);
	}

	void assertLoadValueToMemoryAndIncreaseAddr(byte loadValue, uint16_t expectedAddr)
	{
		cpu.setRegisterA(loadValue);
		uint16_t addr = createAddrFromHighAndLowBytes(cpu.getRegisterH(), cpu.getRegisterL());
		mmu.write(cpu.getProgramCounter(), standardInstructions::LD_HLm_I_A);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_EQ(cpu.getFlag(), 0x00);
		ASSERT_EQ(cpu.getProgramCounter(), 1);
		ASSERT_EQ(mmu.read(addr), loadValue);
		ASSERT_EQ(getMsbFromWord(expectedAddr), cpu.getRegisterH());
		ASSERT_EQ(getLsbFromWord(expectedAddr), cpu.getRegisterL());
	}

	void assertLoadValueToMemoryAndDecreaseAddr(byte loadValue, uint16_t expectedAddr)
	{
		cpu.setRegisterA(loadValue);
		uint16_t addr = createAddrFromHighAndLowBytes(cpu.getRegisterH(), cpu.getRegisterL());
		mmu.write(cpu.getProgramCounter(), standardInstructions::LD_HLm_D_A);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_EQ(cpu.getFlag(), 0x00);
		ASSERT_EQ(cpu.getProgramCounter(), 1);
		ASSERT_EQ(mmu.read(addr), loadValue);
		ASSERT_EQ(getMsbFromWord(expectedAddr), cpu.getRegisterH());
		ASSERT_EQ(getLsbFromWord(expectedAddr), cpu.getRegisterL());
	}

	void assertLoadValueFromMemoryInRegisterWasPerfomed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 2);
		ASSERT_EQ(cpu.getFlag(), 0x00);
		ASSERT_EQ(cpu.getProgramCounter(), 1);
	}

	void assertLoadValueIntoRegisterWasPerformed(byte instruction)
	{
		mmu.write(cpu.getProgramCounter(), instruction);
		int ticks = cpu.fetchDecodeAndExecute();
		ASSERT_EQ(ticks, 1);
		ASSERT_EQ(cpu.getFlag(), 0);
		ASSERT_EQ(cpu.getProgramCounter(), 1);
	}

	MMU mmu;
	CPU cpu = CPU(mmu);
};

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, Load_BCm_A_ShouldLoadValueInMemory)
{
	byte value = 0x42;
	byte msb = 0x05;
	byte lsb = 0x60;
	cpu.setRegisterA(value);
	cpu.setRegisterB(msb);
	cpu.setRegisterC(lsb);
	assertLoadInMemoryWasPerformed(standardInstructions::LD_BCm_A);
	ASSERT_EQ(mmu.read(createAddrFromHighAndLowBytes(msb, lsb)), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, Load_DEm_A_ShouldLoadValueInMemory)
{
	byte value = 0x42;
	byte msb = 0x05;
	byte lsb = 0x60;
	cpu.setRegisterA(value);
	cpu.setRegisterD(msb);
	cpu.setRegisterE(lsb);
	assertLoadInMemoryWasPerformed(standardInstructions::LD_DEm_A);
	ASSERT_EQ(mmu.read(createAddrFromHighAndLowBytes(msb, lsb)), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, Load_HLm_B_ShouldLoadValueInMemory)
{
	byte value = 0x42;
	byte msb = 0x05;
	byte lsb = 0x60;
	cpu.setRegisterB(value);
	cpu.setRegisterH(msb);
	cpu.setRegisterL(lsb);
	assertLoadInMemoryWasPerformed(standardInstructions::LD_HLm_B);
	ASSERT_EQ(mmu.read(createAddrFromHighAndLowBytes(msb, lsb)), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, Load_HLm_C_ShouldLoadValueInMemory)
{
	byte value = 0x42;
	byte msb = 0x05;
	byte lsb = 0x60;
	cpu.setRegisterC(value);
	cpu.setRegisterH(msb);
	cpu.setRegisterL(lsb);
	assertLoadInMemoryWasPerformed(standardInstructions::LD_HLm_C);
	ASSERT_EQ(mmu.read(createAddrFromHighAndLowBytes(msb, lsb)), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, Load_HLm_D_ShouldLoadValueInMemory)
{
	byte value = 0x42;
	byte msb = 0x05;
	byte lsb = 0x60;
	cpu.setRegisterD(value);
	cpu.setRegisterH(msb);
	cpu.setRegisterL(lsb);
	assertLoadInMemoryWasPerformed(standardInstructions::LD_HLm_D);
	ASSERT_EQ(mmu.read(createAddrFromHighAndLowBytes(msb, lsb)), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, Load_HLm_E_ShouldLoadValueInMemory)
{
	byte value = 0x42;
	byte msb = 0x05;
	byte lsb = 0x60;
	cpu.setRegisterE(value);
	cpu.setRegisterH(msb);
	cpu.setRegisterL(lsb);
	assertLoadInMemoryWasPerformed(standardInstructions::LD_HLm_E);
	ASSERT_EQ(mmu.read(createAddrFromHighAndLowBytes(msb, lsb)), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, Load_HLm_H_ShouldLoadValueInMemory)
{
	byte msb = 0x05;
	byte lsb = 0x60;
	cpu.setRegisterH(msb);
	cpu.setRegisterL(lsb);
	assertLoadInMemoryWasPerformed(standardInstructions::LD_HLm_H);
	ASSERT_EQ(mmu.read(createAddrFromHighAndLowBytes(msb, lsb)), msb);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, Load_HLm_L_ShouldLoadValueInMemory)
{
	byte msb = 0x05;
	byte lsb = 0x60;
	cpu.setRegisterH(msb);
	cpu.setRegisterL(lsb);
	assertLoadInMemoryWasPerformed(standardInstructions::LD_HLm_L);
	ASSERT_EQ(mmu.read(createAddrFromHighAndLowBytes(msb, lsb)), lsb);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, Load_HLm_A_ShouldLoadValueInMemory)
{
	byte value = 0x42;
	byte msb = 0x05;
	byte lsb = 0x60;
	cpu.setRegisterA(value);
	cpu.setRegisterH(msb);
	cpu.setRegisterL(lsb);
	assertLoadInMemoryWasPerformed(standardInstructions::LD_HLm_A);
	ASSERT_EQ(mmu.read(createAddrFromHighAndLowBytes(msb, lsb)), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadImmediateValueInRegAShouldLoadValue)
{
	byte value = 0x12;
	assertLoadImmediateValueWasPerformed(standardInstructions::LD_A_n, value);
	ASSERT_EQ(cpu.getRegisterA(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadImmediateValueInRegBShouldLoadValue)
{
	byte value = 0x12;
	assertLoadImmediateValueWasPerformed(standardInstructions::LD_B_n, value);
	ASSERT_EQ(cpu.getRegisterB(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadImmediateValueInRegCShouldLoadValue)
{
	byte value = 0x12;
	assertLoadImmediateValueWasPerformed(standardInstructions::LD_C_n, value);
	ASSERT_EQ(cpu.getRegisterC(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadImmediateValueInRegDShouldLoadValue)
{
	byte value = 0x12;
	assertLoadImmediateValueWasPerformed(standardInstructions::LD_D_n, value);
	ASSERT_EQ(cpu.getRegisterD(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadImmediateValueInRegEShouldLoadValue)
{
	byte value = 0x12;
	assertLoadImmediateValueWasPerformed(standardInstructions::LD_E_n, value);
	ASSERT_EQ(cpu.getRegisterE(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadImmediateValueInRegHShouldLoadValue)
{
	byte value = 0x12;
	assertLoadImmediateValueWasPerformed(standardInstructions::LD_H_n, value);
	ASSERT_EQ(cpu.getRegisterH(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadImmediateValueInRegLShouldLoadValue)
{
	byte value = 0x12;
	assertLoadImmediateValueWasPerformed(standardInstructions::LD_L_n, value);
	ASSERT_EQ(cpu.getRegisterL(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueAndIncreaseFor0ShouldIncreaseTo1)
{
	cpu.setRegisterH(0x00);
	cpu.setRegisterL(0x01);
	assertLoadValueToMemoryAndIncreaseAddr(42, 0x0002);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueAndIncreaseShouldIncreaseLsb)
{
	cpu.setRegisterH(0xFF);
	cpu.setRegisterL(0x01);
	assertLoadValueToMemoryAndIncreaseAddr(42, 0xFF02);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueAndIncreaseShouldIncreaseMsb)
{
	cpu.setRegisterH(0x00);
	cpu.setRegisterL(0xFF);
	assertLoadValueToMemoryAndIncreaseAddr(200, 0x0100);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueAndIncreaseForMaxValueShouldGive0)
{
	cpu.setRegisterH(0xFF);
	cpu.setRegisterL(0xFF);
	assertLoadValueToMemoryAndIncreaseAddr(250, 0x0000);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueAndDecreaseShouldDecreaseLsb)
{
	cpu.setRegisterH(0xFF);
	cpu.setRegisterL(0x01);
	assertLoadValueToMemoryAndDecreaseAddr(42, 0xFF00);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueAndDecreaseShouldDecreaseMsb)
{
	cpu.setRegisterH(0xFF);
	cpu.setRegisterL(0x00);
	assertLoadValueToMemoryAndDecreaseAddr(200, 0xFEFF);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueAndDecreaseForZeroValueShouldGiveMaxValue)
{
	cpu.setRegisterH(0x00);
	cpu.setRegisterL(0x00);
	assertLoadValueToMemoryAndDecreaseAddr(250, 0xFFFF);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromMemoryHLInRegisterAShouldLoadCorrectly)
{
	uint16_t addr = 0x1234;
	byte value = 0x62;

	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, value);

	assertLoadValueFromMemoryInRegisterWasPerfomed(standardInstructions::LD_A_HLm);
	ASSERT_EQ(cpu.getRegisterA(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromMemoryHLInRegisterBShouldLoadCorrectly)
{
	uint16_t addr = 0x1234;
	byte value = 0x62;

	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, value);

	assertLoadValueFromMemoryInRegisterWasPerfomed(standardInstructions::LD_B_HLm);
	ASSERT_EQ(cpu.getRegisterB(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromMemoryHLInRegisterCShouldLoadCorrectly)
{
	uint16_t addr = 0x1234;
	byte value = 0x62;

	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, value);

	assertLoadValueFromMemoryInRegisterWasPerfomed(standardInstructions::LD_C_HLm);
	ASSERT_EQ(cpu.getRegisterC(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromMemoryHLInRegisterDShouldLoadCorrectly)
{
	uint16_t addr = 0x1234;
	byte value = 0x62;

	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, value);

	assertLoadValueFromMemoryInRegisterWasPerfomed(standardInstructions::LD_D_HLm);
	ASSERT_EQ(cpu.getRegisterD(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromMemoryHLInRegisterHShouldLoadCorrectly)
{
	uint16_t addr = 0x1234;
	byte value = 0x62;

	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, value);

	assertLoadValueFromMemoryInRegisterWasPerfomed(standardInstructions::LD_H_HLm);
	ASSERT_EQ(cpu.getRegisterH(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromMemoryHLInRegisterLShouldLoadCorrectly)
{
	uint16_t addr = 0x1234;
	byte value = 0x62;

	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
	mmu.write(addr, value);

	assertLoadValueFromMemoryInRegisterWasPerfomed(standardInstructions::LD_L_HLm);
	ASSERT_EQ(cpu.getRegisterL(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromMemoryBCInRegisterAShouldLoadCorrectly)
{
	uint16_t addr = 0x1234;
	byte value = 0x62;

	cpu.setRegisterB(getMsbFromWord(addr));
	cpu.setRegisterC(getLsbFromWord(addr));
	mmu.write(addr, value);

	assertLoadValueFromMemoryInRegisterWasPerfomed(standardInstructions::LD_A_BCm);
	ASSERT_EQ(cpu.getRegisterA(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromMemoryDEInRegisterAShouldLoadCorrectly)
{
	uint16_t addr = 0x1234;
	byte value = 0x62;

	cpu.setRegisterD(getMsbFromWord(addr));
	cpu.setRegisterE(getLsbFromWord(addr));
	mmu.write(addr, value);

	assertLoadValueFromMemoryInRegisterWasPerfomed(standardInstructions::LD_A_DEm);
	ASSERT_EQ(cpu.getRegisterA(), value);
}

#pragma region Load into register A

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterAIntoAWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterA(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_A_A);
	ASSERT_EQ(cpu.getRegisterA(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterBIntoAWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterB(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_A_B);
	ASSERT_EQ(cpu.getRegisterA(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterCIntoAWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterC(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_A_C);
	ASSERT_EQ(cpu.getRegisterA(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterDIntoAWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterD(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_A_D);
	ASSERT_EQ(cpu.getRegisterA(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterEIntoAWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterE(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_A_E);
	ASSERT_EQ(cpu.getRegisterA(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterHIntoAWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterH(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_A_H);
	ASSERT_EQ(cpu.getRegisterA(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterLIntoAWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterL(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_A_L);
	ASSERT_EQ(cpu.getRegisterA(), value);
}

#pragma endregion

#pragma region Load into register B

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterAIntoBWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterA(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_B_A);
	ASSERT_EQ(cpu.getRegisterB(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterBIntoBWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterB(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_B_B);
	ASSERT_EQ(cpu.getRegisterB(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterCIntoBWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterC(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_B_C);
	ASSERT_EQ(cpu.getRegisterB(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterDIntoBWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterD(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_B_D);
	ASSERT_EQ(cpu.getRegisterB(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterEIntoBWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterE(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_B_E);
	ASSERT_EQ(cpu.getRegisterB(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterHIntoBWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterH(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_B_H);
	ASSERT_EQ(cpu.getRegisterB(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterLIntoBWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterL(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_B_L);
	ASSERT_EQ(cpu.getRegisterB(), value);
}

#pragma endregion

#pragma region Load into register C

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterAIntoCWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterA(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_C_A);
	ASSERT_EQ(cpu.getRegisterC(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterBIntoCWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterB(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_C_B);
	ASSERT_EQ(cpu.getRegisterC(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterCIntoCWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterC(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_C_C);
	ASSERT_EQ(cpu.getRegisterC(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterDIntoCWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterD(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_C_D);
	ASSERT_EQ(cpu.getRegisterC(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterEIntoCWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterE(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_C_E);
	ASSERT_EQ(cpu.getRegisterC(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterHIntoCWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterH(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_C_H);
	ASSERT_EQ(cpu.getRegisterC(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterLIntoCWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterL(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_C_L);
	ASSERT_EQ(cpu.getRegisterC(), value);
}

#pragma endregion

#pragma region Load into register D

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterAIntoDWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterA(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_D_A);
	ASSERT_EQ(cpu.getRegisterD(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterBIntoDWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterB(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_D_B);
	ASSERT_EQ(cpu.getRegisterD(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterCIntoDWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterC(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_D_C);
	ASSERT_EQ(cpu.getRegisterD(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterDIntoDWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterD(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_D_D);
	ASSERT_EQ(cpu.getRegisterD(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterEIntoDWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterE(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_D_E);
	ASSERT_EQ(cpu.getRegisterD(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterHIntoDWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterH(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_D_H);
	ASSERT_EQ(cpu.getRegisterD(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterLIntoDWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterL(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_D_L);
	ASSERT_EQ(cpu.getRegisterD(), value);
}

#pragma endregion

#pragma region Load into register E

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterAIntoEWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterA(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_E_A);
	ASSERT_EQ(cpu.getRegisterE(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterBIntoEWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterB(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_E_B);
	ASSERT_EQ(cpu.getRegisterE(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterCIntoEWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterC(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_E_C);
	ASSERT_EQ(cpu.getRegisterE(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterDIntoEWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterD(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_E_D);
	ASSERT_EQ(cpu.getRegisterE(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterEIntoEWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterE(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_E_E);
	ASSERT_EQ(cpu.getRegisterE(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterHIntoEWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterH(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_E_H);
	ASSERT_EQ(cpu.getRegisterE(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterLIntoEWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterL(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_E_L);
	ASSERT_EQ(cpu.getRegisterE(), value);
}

#pragma endregion

#pragma region Load into register H

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterAIntoHWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterA(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_H_A);
	ASSERT_EQ(cpu.getRegisterH(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterBIntoHWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterB(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_H_B);
	ASSERT_EQ(cpu.getRegisterH(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterCIntoHWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterC(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_H_C);
	ASSERT_EQ(cpu.getRegisterH(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterDIntoHWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterD(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_H_D);
	ASSERT_EQ(cpu.getRegisterH(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterEIntoHWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterE(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_H_E);
	ASSERT_EQ(cpu.getRegisterH(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterHIntoHWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterH(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_H_H);
	ASSERT_EQ(cpu.getRegisterH(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterLIntoHWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterL(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_H_L);
	ASSERT_EQ(cpu.getRegisterH(), value);
}

#pragma endregion

#pragma region Load into register L

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterAIntoLWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterA(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_L_A);
	ASSERT_EQ(cpu.getRegisterL(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterBIntoLWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterB(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_L_B);
	ASSERT_EQ(cpu.getRegisterL(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterCIntoLWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterC(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_L_C);
	ASSERT_EQ(cpu.getRegisterL(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterDIntoLWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterD(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_L_D);
	ASSERT_EQ(cpu.getRegisterL(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterEIntoLWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterE(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_L_E);
	ASSERT_EQ(cpu.getRegisterL(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterHIntoLWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterH(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_L_H);
	ASSERT_EQ(cpu.getRegisterL(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadValueFromRegisterLIntoLWasSuccessful)
{
	uint8_t value = 0x56;
	cpu.setRegisterL(value);
	assertLoadValueIntoRegisterWasPerformed(standardInstructions::LD_L_L);
	ASSERT_EQ(cpu.getRegisterL(), value);
}

#pragma endregion

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, Load8BitsRegisterAtImmediateAddrShouldLoadValue)
{
    byte expectedValue = 0x9A;
	cpu.setRegisterA(expectedValue);
    uint16_t addr = 0x5C8;
    mmu.write(cpu.getProgramCounter(), standardInstructions::LD_nnm_A);
    mmu.writeWord(cpu.getProgramCounter() + 1, addr);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 4);
    ASSERT_EQ(cpu.getFlag(), 0x00);
    uint16_t value = mmu.readWord(addr);
    ASSERT_EQ(value, expectedValue);
    ASSERT_EQ(cpu.getProgramCounter(), 3);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, Load8BitsImmediateValueInMemoryShouldLoadValue)
{
    byte value = 0x12;
    uint16_t addr = 0x3456;
	cpu.setRegisterH(getMsbFromWord(addr));
	cpu.setRegisterL(getLsbFromWord(addr));
    mmu.write(cpu.getProgramCounter(), standardInstructions::LD_HLm_n);
    mmu.writeWord(cpu.getProgramCounter() + 1, value);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 3);
    ASSERT_EQ(cpu.getFlag(), 0x00);
    ASSERT_EQ(cpu.getProgramCounter(), 2);
    ASSERT_EQ(mmu.read(addr), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadAccummulatorInHighMemoryShouldLoadCorrectly)
{
    byte addrOffset = 0x12;
	byte value = 0x42;
	uint16_t expectedAddr = 0xFF12;
	cpu.setRegisterA(value);
    mmu.write(cpu.getProgramCounter(), standardInstructions::LDH_nm_A);
    mmu.writeWord(cpu.getProgramCounter() + 1, addrOffset);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 3);
    ASSERT_EQ(cpu.getFlag(), 0x00);
    ASSERT_EQ(cpu.getProgramCounter(), 2);
    ASSERT_EQ(mmu.read(expectedAddr), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadHighMemoryInAccumulatorShouldLoadCorrectly)
{
	byte addrOffset = 0x12;
	uint16_t expectedAddr = 0xFF12;
	byte value = 0x42;
	mmu.write(expectedAddr, value);
	mmu.write(cpu.getProgramCounter(), standardInstructions::LDH_A_nm);
	mmu.writeWord(cpu.getProgramCounter() + 1, addrOffset);
	int ticks = cpu.fetchDecodeAndExecute();
	ASSERT_EQ(ticks, 3);
	ASSERT_EQ(cpu.getFlag(), 0x00);
	ASSERT_EQ(cpu.getProgramCounter(), 2);
	ASSERT_EQ(cpu.getRegisterA(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadAccummulatorInHighMemoryPointedByCShouldLoadCorrectly)
{
    byte addrOffset = 0x12;
    byte value = 0x42;
    uint16_t expectedAddr = 0xFF12;
    cpu.setRegisterA(value);
	cpu.setRegisterC(addrOffset);
    mmu.write(cpu.getProgramCounter(), standardInstructions::LD_Cm_A);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 2);
    ASSERT_EQ(cpu.getFlag(), 0x00);
    ASSERT_EQ(cpu.getProgramCounter(), 1);
    ASSERT_EQ(mmu.read(expectedAddr), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadHighMemoryPointedByCInAccumulatorShouldLoadCorrectly)
{
    byte addrOffset = 0x12;
    uint16_t expectedAddr = 0xFF12;
    byte value = 0x42;
	cpu.setRegisterC(addrOffset);
    mmu.write(expectedAddr, value);
    mmu.write(cpu.getProgramCounter(), standardInstructions::LD_A_Cm);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 2);
    ASSERT_EQ(cpu.getFlag(), 0x00);
    ASSERT_EQ(cpu.getProgramCounter(), 1);
    ASSERT_EQ(cpu.getRegisterA(), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadRegisterAAtImmediate16BitsAddressShouldLoadCorrectly)
{
    uint16_t addr = 0x1234;
    byte value = 0x42;
    cpu.setRegisterA(value);
    mmu.write(cpu.getProgramCounter(), standardInstructions::LD_nnm_A);
	mmu.writeWord(cpu.getProgramCounter() + 1, addr);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 4);
    ASSERT_EQ(cpu.getFlag(), 0x00);
    ASSERT_EQ(cpu.getProgramCounter(), 3);
    ASSERT_EQ(mmu.read(addr), value);
}

TEST_F(CpuInstructions8BitsLoadStoreMoveTest, LoadImmediate16BitsAddressInRegisterAShouldLoadCorrectly)
{
    uint16_t addr = 0x1234;
    byte value = 0x42;
	mmu.write(addr, value);
    mmu.write(cpu.getProgramCounter(), standardInstructions::LD_A_nnm);
    mmu.writeWord(cpu.getProgramCounter() + 1, addr);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 4);
    ASSERT_EQ(cpu.getFlag(), 0x00);
    ASSERT_EQ(cpu.getProgramCounter(), 3);
    ASSERT_EQ(cpu.getRegisterA(), value);
}