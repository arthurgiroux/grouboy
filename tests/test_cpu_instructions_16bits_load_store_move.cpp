#include "common/utils.hpp"
#include "cpu/cpu.hpp"
#include "cpu/instructions.hpp"

#include <gtest/gtest.h>

using namespace utils;

class CpuInstructions16BitsLoadStoreMoveTest : public ::testing::Test
{
  protected:
    void assertPopIntoMemoryIsPerformed(int instruction, byte msbValue, byte lsbValue)
    {
        word startSp = 0xFFFC;
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
        word startSp = 0x1234;
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

    void assertloadSPAndOffsetInHLGivesExpectedResult(word spValue, sbyte offset, word expectedResult)
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
    ASSERT_EQ(cpu.getFlag(), lsbValue & 0xF0);
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

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, BlarggTestSpecial05PopAF)
{
    /**
     * This test is an adaptation of Blargg test special 05 "POP AF".
     * The original assembly code for the test is:
     *
     *      ld   bc,$1200
     * -    push bc
     *      pop  af
     *      push af
     *      pop  de
     *      ld   a,c
     *      and  $F0
     *      cp   e
     *      jp   nz,test_failed
     *      inc  b
     *      inc  c
     *      jr   nz,-
     */
    mmu.write(cpu.getProgramCounter(), standardInstructions::LD_BC_nn);
    mmu.writeWord(cpu.getProgramCounter() + 1, 0x1200);
    cpu.fetchDecodeAndExecute();
    bool finished = false;
    while (!finished)
    {
        mmu.write(cpu.getProgramCounter(), standardInstructions::PUSH_BC);
        cpu.fetchDecodeAndExecute();
        mmu.write(cpu.getProgramCounter(), standardInstructions::POP_AF);
        cpu.fetchDecodeAndExecute();
        mmu.write(cpu.getProgramCounter(), standardInstructions::PUSH_AF);
        cpu.fetchDecodeAndExecute();
        mmu.write(cpu.getProgramCounter(), standardInstructions::POP_DE);
        cpu.fetchDecodeAndExecute();
        mmu.write(cpu.getProgramCounter(), standardInstructions::LD_A_C);
        cpu.fetchDecodeAndExecute();
        mmu.write(cpu.getProgramCounter(), standardInstructions::AND_n);
        mmu.write(cpu.getProgramCounter() + 1, 0xF0);
        cpu.fetchDecodeAndExecute();
        mmu.write(cpu.getProgramCounter(), standardInstructions::CP_E);
        cpu.fetchDecodeAndExecute();
        ASSERT_TRUE(cpu.isFlagSet(CPU::ZERO));
        mmu.write(cpu.getProgramCounter(), standardInstructions::INC_B);
        cpu.fetchDecodeAndExecute();
        mmu.write(cpu.getProgramCounter(), standardInstructions::INC_C);
        cpu.fetchDecodeAndExecute();
        finished = cpu.isFlagSet(CPU::ZERO);
    }
}

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, LoadFromMemoryInBCShouldLoadMSBInBAndLSBInC)
{
    word value = 0x1234;
    byte msb = 0x12;
    byte lsb = 0x34;
    mmu.write(cpu.getProgramCounter(), standardInstructions::LD_BC_nn);
    mmu.writeWord(cpu.getProgramCounter() + 1, value);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 3);
    ASSERT_EQ(cpu.getFlag(), 0x00);
    ASSERT_EQ(cpu.getRegisterB(), msb);
    ASSERT_EQ(cpu.getRegisterC(), lsb);
    ASSERT_EQ(cpu.getProgramCounter(), 3);
}

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, LoadFromMemoryInDEShouldLoadMSBInDAndLSBInE)
{
    word value = 0x1234;
    byte msb = 0x12;
    byte lsb = 0x34;
    mmu.write(cpu.getProgramCounter(), standardInstructions::LD_DE_nn);
    mmu.writeWord(cpu.getProgramCounter() + 1, value);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 3);
    ASSERT_EQ(cpu.getFlag(), 0x00);
    ASSERT_EQ(cpu.getRegisterD(), msb);
    ASSERT_EQ(cpu.getRegisterE(), lsb);
    ASSERT_EQ(cpu.getProgramCounter(), 3);
}

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, LoadFromMemoryInHLShouldLoadMSBInHAndLSBInL)
{
    word value = 0x1234;
    byte msb = 0x12;
    byte lsb = 0x34;
    mmu.write(cpu.getProgramCounter(), standardInstructions::LD_HL_nn);
    mmu.writeWord(cpu.getProgramCounter() + 1, value);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 3);
    ASSERT_EQ(cpu.getFlag(), 0x00);
    ASSERT_EQ(cpu.getRegisterH(), msb);
    ASSERT_EQ(cpu.getRegisterL(), lsb);
    ASSERT_EQ(cpu.getProgramCounter(), 3);
}

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, LoadFromMemoryInSPShouldLoadInSP)
{
    word value = 0x1234;
    mmu.write(cpu.getProgramCounter(), standardInstructions::LD_SP_nn);
    mmu.writeWord(cpu.getProgramCounter() + 1, value);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 3);
    ASSERT_EQ(cpu.getFlag(), 0x00);
    ASSERT_EQ(cpu.getStackPointer(), value);
    ASSERT_EQ(cpu.getProgramCounter(), 3);
}

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, Load16BitsRegisterAtImmediateAddrShouldWriteValueToMemory)
{
    word expectedValue = 0x559A;
    cpu.setStackPointer(expectedValue);
    word addr = 0x5C8;
    mmu.write(cpu.getProgramCounter(), standardInstructions::LD_nnm_SP);
    mmu.writeWord(cpu.getProgramCounter() + 1, addr);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 5);
    ASSERT_EQ(cpu.getFlag(), CPU::CpuFlags::NONE);
    word value = mmu.readWord(addr);
    ASSERT_EQ(value, expectedValue);
    ASSERT_EQ(cpu.getProgramCounter(), 3);
}

TEST_F(CpuInstructions16BitsLoadStoreMoveTest, Load16BitsImmediateValueInRegisterShouldChangeRegister)
{
    word value = 0x1234;
    mmu.write(cpu.getProgramCounter(), standardInstructions::LD_SP_nn);
    mmu.writeWord(cpu.getProgramCounter() + 1, value);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 3);
    ASSERT_EQ(cpu.getFlag(), CPU::CpuFlags::NONE);
    ASSERT_EQ(cpu.getProgramCounter(), 3);
    ASSERT_EQ(cpu.getStackPointer(), value);
}