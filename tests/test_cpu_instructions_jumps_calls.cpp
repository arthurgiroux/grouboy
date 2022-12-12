#include "common/utils.hpp"
#include "cpu/cpu.hpp"
#include "cpu/instructions.hpp"

#include <gtest/gtest.h>

using namespace utils;

class CpuInstructionsJumpsCallsTest : public ::testing::Test
{
  protected:
    void assertInstructionPerformedJump(byte instruction, bool expected)
    {
        auto flag = cpu.getFlag();
        cpu.setProgramCounter(0x00);
        word addr = 0x1234;
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
        word startAddr = 0x1234;
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
        word startAddr = 0x1234;
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

    void assertWeCanCallASubroutineAndReturnFromIt(int instruction)
    {
        /**
         *  We are creating the following code execution in memory:
         *
         *  0x1234:    call fct;
         *  .....
         *  .....
         *  0x4321:    fct:
         *                return
         *
         */
        word startPc = 0x1234;
        word routineAddr = 0x4321;
        cpu.setProgramCounter(startPc);
        mmu.write(cpu.getProgramCounter(), instruction);
        mmu.writeWord(cpu.getProgramCounter() + 1, routineAddr);
        mmu.write(routineAddr, standardInstructions::RET);

        int ticks = cpu.fetchDecodeAndExecute();
        ASSERT_EQ(ticks, 6);
        ASSERT_EQ(cpu.getProgramCounter(), routineAddr);
        cpu.fetchDecodeAndExecute();
        ASSERT_EQ(cpu.getProgramCounter(), startPc + 3);
    }

    void assertSubroutineIsNotCalled(int instruction)
    {
        word startPc = 0x1234;
        word routineAddr = 0x4321;
        cpu.setProgramCounter(startPc);
        mmu.write(cpu.getProgramCounter(), instruction);
        mmu.writeWord(cpu.getProgramCounter() + 1, routineAddr);

        int ticks = cpu.fetchDecodeAndExecute();
        ASSERT_EQ(ticks, 3);
        ASSERT_EQ(cpu.getProgramCounter(), startPc + 3);
    }

    void assertRestartRoutineIsCalled(int instruction, word expectedAddr)
    {
        word startPc = 0x1234;
        word startSp = 0x4321;
        cpu.setProgramCounter(startPc);
        cpu.setStackPointer(startSp);
        mmu.write(cpu.getProgramCounter(), instruction);
        int ticks = cpu.fetchDecodeAndExecute();
        ASSERT_EQ(ticks, 4);
        ASSERT_EQ(cpu.getStackPointer(), startSp - 2);
        ASSERT_EQ(mmu.readWord(cpu.getStackPointer()), startPc + 1);
        ASSERT_EQ(cpu.getProgramCounter(), expectedAddr);
    }

    void testReturnConditional(int instruction, bool expectReturn)
    {
        word callerAddr = 0x1234;
        mmu.writeWord(cpu.getProgramCounter(), callerAddr);
        cpu.setProgramCounter(cpu.getProgramCounter() + 2);
        mmu.write(cpu.getProgramCounter(), instruction);
        int ticks = cpu.fetchDecodeAndExecute();
        if (expectReturn)
        {
            ASSERT_EQ(ticks, 5);
            ASSERT_EQ(cpu.getProgramCounter(), callerAddr);
        }
        else
        {
            ASSERT_EQ(ticks, 2);
            ASSERT_EQ(cpu.getProgramCounter(), 0x03);
        }
    }

    MMU mmu;
    CPU cpu = CPU(mmu);
};

TEST_F(CpuInstructionsJumpsCallsTest, JumpShouldGoToImmediateMemoryAddr)
{
    cpu.setProgramCounter(0x00);
    word addr = 0x1234;
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
    word addr = 0x1234;
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

TEST_F(CpuInstructionsJumpsCallsTest,
       JumpRelativeConditionalNotCarryWithOffsetMinus127ShouldGoBackwardBy127WhenFlagIsUnset)
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

TEST_F(CpuInstructionsJumpsCallsTest,
       JumpRelativeConditionalNotZeroWithOffsetMinus127ShouldGoBackwardBy127WhenFlagIsUnset)
{
    assertInstructionPerformedJumpAtOffset(standardInstructions::JR_NZ_n, -127, true);
}

TEST_F(CpuInstructionsJumpsCallsTest, CallSubroutineShouldExecuteSubroutine)
{
    assertWeCanCallASubroutineAndReturnFromIt(standardInstructions::CALL_nn);
}

TEST_F(CpuInstructionsJumpsCallsTest, CallSubroutineConditionalCarryWhenCarryIsSetShouldExecuteSubroutine)
{
    cpu.setFlag(CPU::CARRY);
    assertWeCanCallASubroutineAndReturnFromIt(standardInstructions::CALL_C_nn);
}

TEST_F(CpuInstructionsJumpsCallsTest, CallSubroutineConditionalNotCarryWhenCarryIsUnsetShouldExecuteSubroutine)
{
    assertWeCanCallASubroutineAndReturnFromIt(standardInstructions::CALL_NC_nn);
}

TEST_F(CpuInstructionsJumpsCallsTest, CallSubroutineConditionalZeroWhenZeroIsSetShouldExecuteSubroutine)
{
    cpu.setFlag(CPU::ZERO);
    assertWeCanCallASubroutineAndReturnFromIt(standardInstructions::CALL_Z_nn);
}

TEST_F(CpuInstructionsJumpsCallsTest, CallSubroutineConditionalNotZeroWhenZeroIsUnsetShouldExecuteSubroutine)
{
    assertWeCanCallASubroutineAndReturnFromIt(standardInstructions::CALL_NZ_nn);
}

TEST_F(CpuInstructionsJumpsCallsTest, CallSubroutineConditionalCarryWhenCarryIsUnsetShouldNotExecuteSubroutine)
{
    assertSubroutineIsNotCalled(standardInstructions::CALL_C_nn);
}

TEST_F(CpuInstructionsJumpsCallsTest, CallSubroutineConditionalNotCarryWhenCarryIsSetShouldNotExecuteSubroutine)
{
    cpu.setFlag(CPU::CARRY);
    assertSubroutineIsNotCalled(standardInstructions::CALL_NC_nn);
}

TEST_F(CpuInstructionsJumpsCallsTest, CallSubroutineConditionalZeroWhenZeroIsUnsetShouldNotExecuteSubroutine)
{
    assertSubroutineIsNotCalled(standardInstructions::CALL_Z_nn);
}

TEST_F(CpuInstructionsJumpsCallsTest, CallSubroutineConditionalNotZeroWhenZeroIsSetShouldNotExecuteSubroutine)
{
    cpu.setFlag(CPU::ZERO);
    assertSubroutineIsNotCalled(standardInstructions::CALL_NZ_nn);
}

TEST_F(CpuInstructionsJumpsCallsTest, CallRestartRoutine0x00ShouldGoToAddr0x00)
{
    assertRestartRoutineIsCalled(standardInstructions::RST_0, 0x00);
}

TEST_F(CpuInstructionsJumpsCallsTest, CallRestartRoutine0x10ShouldGoToAddr0x10)
{
    assertRestartRoutineIsCalled(standardInstructions::RST_10, 0x10);
}

TEST_F(CpuInstructionsJumpsCallsTest, CallRestartRoutine0x20ShouldGoToAddr0x20)
{
    assertRestartRoutineIsCalled(standardInstructions::RST_20, 0x20);
}

TEST_F(CpuInstructionsJumpsCallsTest, CallRestartRoutine0x30ShouldGoToAddr0x30)
{
    assertRestartRoutineIsCalled(standardInstructions::RST_30, 0x30);
}

TEST_F(CpuInstructionsJumpsCallsTest, CallRestartRoutine0x08ShouldGoToAddr0x08)
{
    assertRestartRoutineIsCalled(standardInstructions::RST_8, 0x08);
}

TEST_F(CpuInstructionsJumpsCallsTest, CallRestartRoutine0x18ShouldGoToAddr0x18)
{
    assertRestartRoutineIsCalled(standardInstructions::RST_18, 0x18);
}

TEST_F(CpuInstructionsJumpsCallsTest, CallRestartRoutine0x28ShouldGoToAddr0x28)
{
    assertRestartRoutineIsCalled(standardInstructions::RST_28, 0x28);
}

TEST_F(CpuInstructionsJumpsCallsTest, CallRestartRoutine0x38ShouldGoToAddr0x38)
{
    assertRestartRoutineIsCalled(standardInstructions::RST_38, 0x38);
}

TEST_F(CpuInstructionsJumpsCallsTest, InstructionReturn)
{
    word callerAddr = 0x1234;
    mmu.writeWord(cpu.getProgramCounter(), callerAddr);
    cpu.setProgramCounter(cpu.getProgramCounter() + 2);
    mmu.write(cpu.getProgramCounter(), standardInstructions::RET);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 4);
    ASSERT_EQ(cpu.getFlag(), 0x00);
    ASSERT_EQ(cpu.getProgramCounter(), callerAddr);
}

TEST_F(CpuInstructionsJumpsCallsTest, InstructionReturnConditionalCarryTrue)
{
    cpu.setFlag(CPU::CARRY);
    testReturnConditional(standardInstructions::RET_C, true);
    ASSERT_EQ(cpu.getFlag(), CPU::CARRY);
}

TEST_F(CpuInstructionsJumpsCallsTest, InstructionReturnConditionalCarryFalse)
{
    testReturnConditional(standardInstructions::RET_C, false);
    ASSERT_EQ(cpu.getFlag(), 0x00);
}

TEST_F(CpuInstructionsJumpsCallsTest, InstructionReturnConditionalZeroTrue)
{
    cpu.setFlag(CPU::ZERO);
    testReturnConditional(standardInstructions::RET_Z, true);
    ASSERT_EQ(cpu.getFlag(), CPU::ZERO);
}

TEST_F(CpuInstructionsJumpsCallsTest, InstructionReturnConditionalZeroFalse)
{
    testReturnConditional(standardInstructions::RET_Z, false);
    ASSERT_EQ(cpu.getFlag(), 0x00);
}

TEST_F(CpuInstructionsJumpsCallsTest, InstructionReturnConditionalNotCarryTrue)
{
    testReturnConditional(standardInstructions::RET_NC, true);
    ASSERT_EQ(cpu.getFlag(), 0x00);
}

TEST_F(CpuInstructionsJumpsCallsTest, InstructionReturnConditionalNotCarryFalse)
{
    cpu.setFlag(CPU::CARRY);
    testReturnConditional(standardInstructions::RET_NC, false);
    ASSERT_EQ(cpu.getFlag(), CPU::CARRY);
}

TEST_F(CpuInstructionsJumpsCallsTest, InstructionReturnConditionalNotZeroTrue)
{
    testReturnConditional(standardInstructions::RET_NZ, true);
    ASSERT_EQ(cpu.getFlag(), 0x00);
}

TEST_F(CpuInstructionsJumpsCallsTest, InstructionReturnConditionalNotZeroFalse)
{
    cpu.setFlag(CPU::ZERO);
    testReturnConditional(standardInstructions::RET_NZ, false);
    ASSERT_EQ(cpu.getFlag(), CPU::ZERO);
}

TEST_F(CpuInstructionsJumpsCallsTest, InstructionReturnAfterInterrupt)
{
    cpu.enableInterrupts();
    word callerAddr = 0x1234;
    mmu.writeWord(cpu.getProgramCounter(), callerAddr);
    cpu.setProgramCounter(cpu.getProgramCounter() + 2);
    mmu.write(cpu.getProgramCounter(), standardInstructions::RETI);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 4);
    ASSERT_EQ(cpu.getFlag(), 0x00);
    ASSERT_EQ(cpu.getProgramCounter(), callerAddr);
    ASSERT_TRUE(cpu.areInterruptsEnabled());
}