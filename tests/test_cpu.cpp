#include <gtest/gtest.h>
#include "cpu.hpp"
#include "instructions.hpp"

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

TEST_F(CpuTest, OpcodeNop) {
    mmu.write(0, NOP);
    int ticks = cpu.exec();
    ASSERT_EQ(ticks, 1);
    ASSERT_EQ(cpu.tick, 1);
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