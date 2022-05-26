#include <gtest/gtest.h>
#include "cpu.hpp"
#include "instructions.hpp"
#include "utils.hpp"

using namespace utils;

class CpuTest : public ::testing::Test {
protected:
    MMU mmu;
    CPU cpu = CPU(mmu);

    void assertFlagCanBeSetAndUnset(CPU::CpuFlags flag) {
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

class CpuInstructionTest : public ::testing::Test {
protected:
    MMU mmu;
    CPU cpu = CPU(mmu);

    void performInstructionAndAssertTicksAndFlag(byte instruction, int expectedTicks, byte expectedFlag) {
        mmu.write(0, instruction);
        int ticks = cpu.fetchDecodeAndExecute();
        ASSERT_EQ(ticks, expectedTicks);
        ASSERT_EQ(cpu.tick, expectedTicks);
        ASSERT_EQ(cpu.getFlag(), expectedFlag);
    }

    void testLoad16BitsValueInRegisters(byte instruction, uint16_t value, byte& msbRegister, byte& lsbRegister) {
        cpu.pc = 0x00;
        mmu.write(cpu.pc, instruction);
        mmu.writeWord(cpu.pc + 1, value);
        int ticks = cpu.fetchDecodeAndExecute();
        ASSERT_EQ(ticks, 3);
        ASSERT_EQ(cpu.getFlag(), 0x00);
        ASSERT_EQ(msbRegister, value >> 8);
        ASSERT_EQ(lsbRegister, value & 0xFF);
        ASSERT_EQ(cpu.pc, 0x03);
    }

    void testLoad16BitsValueInRegisters(byte instruction, uint16_t value, uint16_t& reg) {
        cpu.pc = 0x00;
        mmu.write(cpu.pc, instruction);
        mmu.writeWord(cpu.pc + 1, value);
        int ticks = cpu.fetchDecodeAndExecute();
        ASSERT_EQ(ticks, 3);
        ASSERT_EQ(cpu.getFlag(), 0x00);
        ASSERT_EQ(reg, value);
        ASSERT_EQ(cpu.pc, 0x03);
    }

    void testLoad8BitsValueInMemoryAddr(byte instruction, byte& msb, byte& lsb, byte& value) {
        cpu.pc = 0x00;
        value = 42;
        msb = 0x05;
        lsb = 0x60;
        mmu.write(cpu.pc, instruction);
        int ticks = cpu.fetchDecodeAndExecute();
        ASSERT_EQ(ticks, 2);
        ASSERT_EQ(cpu.getFlag(), 0x00);
        ASSERT_EQ(mmu.read(createAddrFromHighAndLowBytes(msb, lsb)), value);
        ASSERT_EQ(cpu.pc, 0x01);
    }
};

TEST_F(CpuTest, RegistersValueAtInitAreCorrect) {
    ASSERT_EQ(cpu.a, 0x00);
    ASSERT_EQ(cpu.b, 0x00);
    ASSERT_EQ(cpu.c, 0x00);
    ASSERT_EQ(cpu.d, 0x00);
    ASSERT_EQ(cpu.e, 0x00);
    ASSERT_EQ(cpu.h, 0x00);
    ASSERT_EQ(cpu.l, 0x00);
}

TEST_F(CpuTest, FlagValueAtInitAreCorrect) {
    ASSERT_EQ(cpu.f, 0x00);
}

TEST_F(CpuTest, CheckCpuStateAtInit) {
    ASSERT_EQ(cpu.tick, 0);
    ASSERT_EQ(cpu.pc, 0);
    ASSERT_EQ(cpu.sp, 0);
    ASSERT_EQ(cpu.halted, false);
    ASSERT_EQ(cpu.interrupts, false);
}

TEST_F(CpuTest, FlagAreUnsetByDefault) {
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
    ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
}

TEST_F(CpuTest, FlagZeroCanBeSet) {
    assertFlagCanBeSetAndUnset(CPU::CpuFlags::ZERO);
}

TEST_F(CpuTest, FlagSubstractionCanBeSet) {
    assertFlagCanBeSetAndUnset(CPU::CpuFlags::SUBSTRACTION);
}

TEST_F(CpuTest, FlagCarryCanBeSet) {
    assertFlagCanBeSetAndUnset(CPU::CpuFlags::CARRY);
}

TEST_F(CpuTest, FlagHalfCarryCanBeSet) {
    assertFlagCanBeSetAndUnset(CPU::CpuFlags::HALF_CARRY);
}

TEST_F(CpuInstructionTest, InstructionNoop) {
    performInstructionAndAssertTicksAndFlag(standardInstructions::NOP, 1, 0x00);
    ASSERT_EQ(cpu.pc, 0x01);
}

TEST_F(CpuInstructionTest, InstructionLoad16BitsValueInRegisters) {
    testLoad16BitsValueInRegisters(standardInstructions::LD_BC_nn, 0x0012, cpu.b, cpu.c);
    testLoad16BitsValueInRegisters(standardInstructions::LD_BC_nn, 0x1200, cpu.b, cpu.c);
    testLoad16BitsValueInRegisters(standardInstructions::LD_BC_nn, 0x1234, cpu.b, cpu.c);

    testLoad16BitsValueInRegisters(standardInstructions::LD_DE_nn, 0x0012, cpu.d, cpu.e);
    testLoad16BitsValueInRegisters(standardInstructions::LD_DE_nn, 0x1200, cpu.d, cpu.e);
    testLoad16BitsValueInRegisters(standardInstructions::LD_DE_nn, 0x1234, cpu.d, cpu.e);

    testLoad16BitsValueInRegisters(standardInstructions::LD_HL_nn, 0x0012, cpu.h, cpu.l);
    testLoad16BitsValueInRegisters(standardInstructions::LD_HL_nn, 0x1200, cpu.h, cpu.l);
    testLoad16BitsValueInRegisters(standardInstructions::LD_HL_nn, 0x1234, cpu.h, cpu.l);

    testLoad16BitsValueInRegisters(standardInstructions::LD_SP_nn, 0x0012, cpu.sp);
    testLoad16BitsValueInRegisters(standardInstructions::LD_SP_nn, 0x1200, cpu.sp);
    testLoad16BitsValueInRegisters(standardInstructions::LD_SP_nn, 0x1234, cpu.sp);
}

TEST_F(CpuInstructionTest, InstructionLoad8BitsValueInMemoryAddr) {
    testLoad8BitsValueInMemoryAddr(standardInstructions::LD_BCm_A, cpu.b, cpu.c, cpu.a);
    testLoad8BitsValueInMemoryAddr(standardInstructions::LD_DEm_A, cpu.d, cpu.e, cpu.a);
    testLoad8BitsValueInMemoryAddr(standardInstructions::LD_HLm_B, cpu.h, cpu.l, cpu.b);
    testLoad8BitsValueInMemoryAddr(standardInstructions::LD_HLm_C, cpu.h, cpu.l, cpu.c);
    testLoad8BitsValueInMemoryAddr(standardInstructions::LD_HLm_D, cpu.h, cpu.l, cpu.d);
    testLoad8BitsValueInMemoryAddr(standardInstructions::LD_HLm_E, cpu.h, cpu.l, cpu.e);
    testLoad8BitsValueInMemoryAddr(standardInstructions::LD_HLm_H, cpu.h, cpu.l, cpu.h);
    testLoad8BitsValueInMemoryAddr(standardInstructions::LD_HLm_L, cpu.h, cpu.l, cpu.l);
    testLoad8BitsValueInMemoryAddr(standardInstructions::LD_HLm_A, cpu.h, cpu.l, cpu.a);
}
