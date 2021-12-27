#include <gtest/gtest.h>
#include "cpu.hpp"
#include "instructions.hpp"

class CpuTest : public ::testing::Test {
protected:
    MMU mmu;
    CPU cpu = CPU(mmu);
};

TEST_F(CpuTest, RegistersValueAtInitAreCorrect) {
    ASSERT_EQ(cpu.a, 0x01);
    ASSERT_EQ(cpu.b, 0x00);
    ASSERT_EQ(cpu.c, 0x13);
    ASSERT_EQ(cpu.d, 0x00);
    ASSERT_EQ(cpu.e, 0xd8);
    ASSERT_EQ(cpu.h, 0x01);
    ASSERT_EQ(cpu.l, 0x4D);
}

TEST_F(CpuTest, FlagValueAtInitAreCorrect) {
    ASSERT_EQ(cpu.f, 0xb0);
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