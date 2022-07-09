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

    void testIncrement8BitsRegisters(byte instruction, const byte& msbRegister, const byte& lsbRegister) {
        cpu.pc = 0x00;
        uint16_t expected_pc = 1;
        for (int i = 1; i <= UINT16_MAX + 1; ++i) {
            uint16_t expected_value = i;
            mmu.write(cpu.pc, instruction);
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, 2);
            ASSERT_EQ(cpu.getFlag(), 0x00);
            uint16_t regValue = (msbRegister << 8u) | lsbRegister;
            ASSERT_EQ(regValue, expected_value);
            ASSERT_EQ(cpu.pc, expected_pc);
            expected_pc++;
        }
    }

    void testIncrement16BitsRegister(byte instruction, const uint16_t& reg) {
        cpu.pc = 0x00;
        uint16_t expected_pc = 1;
        for (int i = 1; i <= UINT16_MAX + 1; ++i) {
            uint16_t expected_value = i;
            mmu.write(cpu.pc, instruction);
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, 2);
            ASSERT_EQ(cpu.getFlag(), 0x00);
            ASSERT_EQ(reg, expected_value);
            ASSERT_EQ(cpu.pc, expected_pc);
            expected_pc++;
        }
    }

    void testIncrement8BitRegister(byte instruction, const byte& reg) {
        cpu.pc = 0x00;
        uint16_t expected_pc = 1;
        for (int i = 1; i <= UINT8_MAX + 1; ++i) {
            byte expected_value = i;
            mmu.write(cpu.pc, instruction);
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, 1);
            ASSERT_FALSE(cpu.isFlagSet( CPU::CpuFlags::SUBSTRACTION));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), expected_value == 0x10);
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expected_value == 0);
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
            ASSERT_EQ(reg, expected_value);
            ASSERT_EQ(cpu.pc, expected_pc);
            expected_pc++;
        }
    }

    void testIncrement8BitsMemoryValue(byte instruction, byte& msbRegister, byte& lsbRegister) {
        cpu.pc = 0x00;
        msbRegister = 0x0F;
        lsbRegister = 0xFF;
        mmu.write(createAddrFromHighAndLowBytes(msbRegister, lsbRegister), 0);
        uint16_t expected_pc = 1;
        for (int i = 1; i <= UINT8_MAX + 1; ++i) {
            byte expected_value = i;
            mmu.write(cpu.pc, instruction);
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, 3);
            ASSERT_FALSE(cpu.isFlagSet( CPU::CpuFlags::SUBSTRACTION));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), expected_value == 0x10);
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expected_value == 0);
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
            ASSERT_EQ(mmu.read(createAddrFromHighAndLowBytes(msbRegister, lsbRegister)), expected_value);
            ASSERT_EQ(cpu.pc, expected_pc);
            expected_pc++;
        }
    }

    void testDecrement8BitsRegisters(byte instruction, const byte& msbRegister, const byte& lsbRegister) {
        cpu.pc = 0x00;
        uint16_t expected_pc = 1;
        for (int i = UINT16_MAX; i >= 0; --i) {
            uint16_t expected_value = i;
            mmu.write(cpu.pc, instruction);
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, 2);
            ASSERT_EQ(cpu.getFlag(), 0x00);
            uint16_t regValue = (msbRegister << 8u) | lsbRegister;
            ASSERT_EQ(regValue, expected_value);
            ASSERT_EQ(cpu.pc, expected_pc);
            expected_pc++;
        }
    }

    void testDecrement16BitsRegister(byte instruction, const uint16_t& reg) {
        cpu.pc = 0x00;
        uint16_t expected_pc = 1;
        for (int i = UINT16_MAX; i >= 0; --i) {
            uint16_t expected_value = i;
            mmu.write(cpu.pc, instruction);
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, 2);
            ASSERT_EQ(cpu.getFlag(), 0x00);
            ASSERT_EQ(reg, expected_value);
            ASSERT_EQ(cpu.pc, expected_pc);
            expected_pc++;
        }
    }

    void testDecrement8BitRegister(byte instruction, const byte& reg) {
        cpu.pc = 0x00;
        uint16_t expected_pc = 1;
        for (int i = UINT8_MAX; i >= 0; --i) {
            byte expected_value = i;
            mmu.write(cpu.pc, instruction);
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, 1);
            ASSERT_TRUE(cpu.isFlagSet( CPU::CpuFlags::SUBSTRACTION));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), expected_value == 0x08);
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expected_value == 0);
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
            ASSERT_EQ(reg, expected_value);
            ASSERT_EQ(cpu.pc, expected_pc);
            expected_pc++;
        }
    }

    void testDecrement8BitsMemoryValue(byte instruction, byte& msbRegister, byte& lsbRegister) {
        cpu.pc = 0x00;
        msbRegister = 0x0F;
        lsbRegister = 0xFF;
        mmu.write(createAddrFromHighAndLowBytes(msbRegister, lsbRegister), 0);
        uint16_t expected_pc = 1;
        for (int i = UINT8_MAX; i >= 0; --i) {
            byte expected_value = i;
            mmu.write(cpu.pc, instruction);
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, 3);
            ASSERT_TRUE(cpu.isFlagSet( CPU::CpuFlags::SUBSTRACTION));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), expected_value == 0x08);
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expected_value == 0);
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::CARRY));
            ASSERT_EQ(mmu.read(createAddrFromHighAndLowBytes(msbRegister, lsbRegister)), expected_value);
            ASSERT_EQ(cpu.pc, expected_pc);
            expected_pc++;
        }
    }

    void testLoadImmediateValue(byte instruction, byte& reg) {
        cpu.pc = 0x00;
        uint16_t expected_pc = 0;
        for (int i = 0; i < UINT8_MAX; ++i) {
            mmu.write(cpu.pc, instruction);
            mmu.write(cpu.pc + 1, i);
            int ticks = cpu.fetchDecodeAndExecute();
            expected_pc += 2;
            ASSERT_EQ(ticks, 2);
            ASSERT_EQ(cpu.getFlag(), 0x00);
            ASSERT_EQ(cpu.pc, expected_pc);
            ASSERT_EQ(reg, i);
        }
    }

    void testRotateLeftCircular(const std::vector<byte>& instructions, byte& reg, int expectedTicks) {
        reg = 0b00000001;
        cpu.pc = 0x00;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < instructions.size(); ++j) {
                mmu.write(cpu.pc + j, instructions[j]);
            }
            byte registerValue = reg;
            byte expectedValue = (registerValue << 1) | (registerValue >> 7);
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, expectedTicks);
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expectedValue == 0);
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (registerValue & 0x80) > 0);
            ASSERT_EQ(reg, expectedValue);
        }
    }

    void testRotateLeftCircularFromMemory(const std::vector<byte>& instructions, byte& regMsb, byte& regLsb, int expectedTicks) {
        regMsb = 0x42;
        regLsb = 0x11;
        uint16_t addr = createAddrFromHighAndLowBytes(regMsb, regLsb);
        mmu.write(addr, 0b00000001);
        cpu.pc = 0x00;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < instructions.size(); ++j) {
                mmu.write(cpu.pc + j, instructions[j]);
            }
            byte registerValue = mmu.read(addr);
            byte expectedValue = (registerValue << 1) | (registerValue >> 7);
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, expectedTicks);
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expectedValue == 0);
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (registerValue & 0x80) > 0);
            ASSERT_EQ(mmu.read(addr), expectedValue);
        }
    }

    void testRotateLeft(const std::vector<byte>& instructions, byte& reg, int expectedTicks) {
        reg = 0b00000001;
        cpu.pc = 0x00;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < instructions.size(); ++j) {
                mmu.write(cpu.pc + j, instructions[j]);
            }
            byte registerValue = reg;
            bool isCarryFlagSet = cpu.isFlagSet(CPU::CpuFlags::CARRY);
            byte expectedValue = (registerValue << 1) | isCarryFlagSet;
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, expectedTicks);
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expectedValue == 0);
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (registerValue & 0x80) > 0);
            ASSERT_EQ(reg, expectedValue);
        }
    }

    void testRotateLeftFromMemory(const std::vector<byte>& instructions, byte& regMsb, byte& regLsb, int expectedTicks) {
        regMsb = 0x42;
        regLsb = 0x11;
        uint16_t addr = createAddrFromHighAndLowBytes(regMsb, regLsb);
        mmu.write(addr, 0b00000001);
        cpu.pc = 0x00;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < instructions.size(); ++j) {
                mmu.write(cpu.pc + j, instructions[j]);
            }
            byte registerValue = mmu.read(addr);
            bool isCarryFlagSet = cpu.isFlagSet(CPU::CpuFlags::CARRY);
            byte expectedValue = (registerValue << 1) | isCarryFlagSet;
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, expectedTicks);
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expectedValue == 0);
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (registerValue & 0x80) > 0);
            ASSERT_EQ(mmu.read(addr), expectedValue);
        }
    }

    void addTwo8BitsRegisterToTwo8BitsRegister(byte instruction, byte& resultRegMsb, byte& resultRegLsb,
                                               byte& valueRegMsb, byte& valueRegLsb,
                                               std::vector<uint16_t>& startValues, std::vector<uint16_t>& addValues) {
        for (int i = 0; i < startValues.size(); ++i) {
            uint16_t startValue = startValues[i];
            uint16_t addValue = addValues[i];
            resultRegLsb = startValue & 0xFF;
            resultRegMsb = (startValue >> 8) & 0xFF;
            valueRegLsb = addValue & 0xFF;
            valueRegMsb = (addValue >> 8) & 0xFF;
            uint32_t expectedValue = startValue + addValue;
            mmu.write(cpu.pc, instruction);
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, 2);
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (expectedValue > 0xFFFF));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), (startValue <= 0xFFF && expectedValue > 0xFFF));
        }
    }

    void add16BitsRegisterToTwo8BitsRegister(byte instruction, byte& resultRegMsb, byte& resultRegLsb,
                                               uint16_t& valueReg,
                                               std::vector<uint16_t>& startValues, std::vector<uint16_t>& addValues) {
        for (int i = 0; i < startValues.size(); ++i) {
            uint16_t startValue = startValues[i];
            uint16_t addValue = addValues[i];
            resultRegLsb = startValue & 0xFF;
            resultRegMsb = (startValue >> 8) & 0xFF;
            valueReg = addValue;
            uint32_t expectedValue = startValue + addValue;
            mmu.write(cpu.pc, instruction);
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, 2);
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::ZERO));
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (expectedValue > 0xFFFF));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY), (startValue <= 0xFFF && expectedValue > 0xFFF));
        }
    }

    void testLoadValueFromMemoryInto8BitsRegister(byte instruction, byte& reg, byte& addrRegMsb, byte& addrRegLsb) {
        cpu.pc = 0x00;
        std::vector<uint16_t> addr = {0x0042, 0x0F00, 0x0F42};
        for (int i = 0; i < addr.size(); ++i) {
            addrRegMsb = addr[i] >> 8;
            addrRegLsb = addr[i] & 0xFF;
            mmu.write(addr[i], i + 1);
            mmu.write(cpu.pc, instruction);
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, 2);
            ASSERT_EQ(cpu.getFlag(), 0x00);
            ASSERT_EQ(reg, i + 1);
            ASSERT_EQ(cpu.pc, i+1);
        }
    }

    void testRotateRightCircular(const std::vector<byte>& instructions, byte& reg, int expectedTicks) {
        reg = 0b10000000;
        cpu.pc = 0x00;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < instructions.size(); ++j) {
                mmu.write(cpu.pc + j, instructions[j]);
            }
            byte registerValue = reg;
            byte expectedValue = (registerValue << 7) | (registerValue >> 1);
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, expectedTicks);
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expectedValue == 0);
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (registerValue & 0x01) > 0);
            ASSERT_EQ(reg, expectedValue);
        }
    }

    void testRotateRightCircularFromMemory(const std::vector<byte>& instructions, byte& regMsb, byte& regLsb, int expectedTicks) {
        regMsb = 0x42;
        regLsb = 0x11;
        uint16_t addr = createAddrFromHighAndLowBytes(regMsb, regLsb);
        mmu.write(addr, 0b10000000);
        cpu.pc = 0x00;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < instructions.size(); ++j) {
                mmu.write(cpu.pc + j, instructions[j]);
            }
            byte registerValue = mmu.read(addr);
            byte expectedValue = (registerValue << 7) | (registerValue >> 1);
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, expectedTicks);
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expectedValue == 0);
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (registerValue & 0x01) > 0);
            ASSERT_EQ(mmu.read(addr), expectedValue);
        }
    }

    void testRotateRight(const std::vector<byte>& instructions, byte& reg, int expectedTicks) {
        reg = 0b10000000;
        cpu.pc = 0x00;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < instructions.size(); ++j) {
                mmu.write(cpu.pc + j, instructions[j]);
            }
            byte registerValue = reg;
            bool isCarryFlagSet = cpu.isFlagSet(CPU::CpuFlags::CARRY);
            byte expectedValue = (isCarryFlagSet << 7) | (registerValue >> 1);
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, expectedTicks);
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expectedValue == 0);
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (registerValue & 0x01) > 0);
            ASSERT_EQ(reg, expectedValue);
        }
    }

    void testRotateRightFromMemory(const std::vector<byte>& instructions, byte& regMsb, byte& regLsb, int expectedTicks) {
        regMsb = 0x42;
        regLsb = 0x11;
        uint16_t addr = createAddrFromHighAndLowBytes(regMsb, regLsb);
        mmu.write(addr, 0b10000000);
        cpu.pc = 0x00;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < instructions.size(); ++j) {
                mmu.write(cpu.pc + j, instructions[j]);
            }
            byte registerValue = mmu.read(addr);
            bool isCarryFlagSet = cpu.isFlagSet(CPU::CpuFlags::CARRY);
            byte expectedValue = (isCarryFlagSet << 7) | (registerValue >> 1);
            int ticks = cpu.fetchDecodeAndExecute();
            ASSERT_EQ(ticks, expectedTicks);
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::HALF_CARRY));
            ASSERT_FALSE(cpu.isFlagSet(CPU::CpuFlags::SUBSTRACTION));
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::ZERO), expectedValue == 0);
            ASSERT_EQ(cpu.isFlagSet(CPU::CpuFlags::CARRY), (registerValue & 0x01) > 0);
            ASSERT_EQ(mmu.read(addr), expectedValue);
        }
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

TEST_F(CpuInstructionTest, InstructionIncrementRegister) {
    testIncrement8BitsRegisters(standardInstructions::INC_BC, cpu.b, cpu.c);
    testIncrement8BitsRegisters(standardInstructions::INC_DE, cpu.d, cpu.e);
    testIncrement8BitsRegisters(standardInstructions::INC_HL, cpu.h, cpu.l);
    testIncrement16BitsRegister(standardInstructions::INC_SP, cpu.sp);
    testIncrement8BitRegister(standardInstructions::INC_B, cpu.b);
    testIncrement8BitRegister(standardInstructions::INC_C, cpu.c);
    testIncrement8BitRegister(standardInstructions::INC_D, cpu.d);
    testIncrement8BitRegister(standardInstructions::INC_E, cpu.e);
    testIncrement8BitRegister(standardInstructions::INC_H, cpu.h);
    testIncrement8BitRegister(standardInstructions::INC_L, cpu.l);
    testIncrement8BitRegister(standardInstructions::INC_A, cpu.a);
    testIncrement8BitsMemoryValue(standardInstructions::INC_HLm, cpu.h, cpu.l);
}

TEST_F(CpuInstructionTest, InstructionDecrementRegister) {
    testDecrement8BitsRegisters(standardInstructions::DEC_BC, cpu.b, cpu.c);
    testDecrement8BitsRegisters(standardInstructions::DEC_DE, cpu.d, cpu.e);
    testDecrement8BitsRegisters(standardInstructions::DEC_HL, cpu.h, cpu.l);
    testDecrement16BitsRegister(standardInstructions::DEC_SP, cpu.sp);
    testDecrement8BitRegister(standardInstructions::DEC_B, cpu.b);
    testDecrement8BitRegister(standardInstructions::DEC_C, cpu.c);
    testDecrement8BitRegister(standardInstructions::DEC_D, cpu.d);
    testDecrement8BitRegister(standardInstructions::DEC_E, cpu.e);
    testDecrement8BitRegister(standardInstructions::DEC_H, cpu.h);
    testDecrement8BitRegister(standardInstructions::DEC_L, cpu.l);
    testDecrement8BitRegister(standardInstructions::DEC_A, cpu.a);
    testDecrement8BitsMemoryValue(standardInstructions::DEC_HLm, cpu.h, cpu.l);
}

TEST_F(CpuInstructionTest, InstructionLoadImmediateValue) {
    testLoadImmediateValue(standardInstructions::LD_A_n, cpu.a);
    testLoadImmediateValue(standardInstructions::LD_B_n, cpu.b);
    testLoadImmediateValue(standardInstructions::LD_C_n, cpu.c);
    testLoadImmediateValue(standardInstructions::LD_D_n, cpu.d);
    testLoadImmediateValue(standardInstructions::LD_E_n, cpu.e);
    testLoadImmediateValue(standardInstructions::LD_H_n, cpu.h);
    testLoadImmediateValue(standardInstructions::LD_L_n, cpu.l);
}

TEST_F(CpuInstructionTest, InstructionRotateLeftCircular) {
    testRotateLeftCircular({standardInstructions::RLC_A}, cpu.a, 1);
    testRotateLeftCircular({standardInstructions::EXT_OPS, extendedInstructions::RLC_A}, cpu.a, 2);
    testRotateLeftCircular({standardInstructions::EXT_OPS, extendedInstructions::RLC_B}, cpu.b, 2);
    testRotateLeftCircular({standardInstructions::EXT_OPS, extendedInstructions::RLC_C}, cpu.c, 2);
    testRotateLeftCircular({standardInstructions::EXT_OPS, extendedInstructions::RLC_D}, cpu.d, 2);
    testRotateLeftCircular({standardInstructions::EXT_OPS, extendedInstructions::RLC_E}, cpu.e, 2);
    testRotateLeftCircular({standardInstructions::EXT_OPS, extendedInstructions::RLC_H}, cpu.h, 2);
    testRotateLeftCircular({standardInstructions::EXT_OPS, extendedInstructions::RLC_L}, cpu.l, 2);
}

TEST_F(CpuInstructionTest, InstructionRotateLeftCircularFromMemory) {
    testRotateLeftCircularFromMemory({standardInstructions::EXT_OPS, extendedInstructions::RLC_HLm}, cpu.h, cpu.l, 4);
}

TEST_F(CpuInstructionTest, InstructionRotateLeft) {
    testRotateLeft({standardInstructions::EXT_OPS, extendedInstructions::RL_A}, cpu.a, 2);
    testRotateLeft({standardInstructions::EXT_OPS, extendedInstructions::RL_B}, cpu.b, 2);
    testRotateLeft({standardInstructions::EXT_OPS, extendedInstructions::RL_C}, cpu.c, 2);
    testRotateLeft({standardInstructions::EXT_OPS, extendedInstructions::RL_D}, cpu.d, 2);
    testRotateLeft({standardInstructions::EXT_OPS, extendedInstructions::RL_E}, cpu.e, 2);
    testRotateLeft({standardInstructions::EXT_OPS, extendedInstructions::RL_H}, cpu.h, 2);
    testRotateLeft({standardInstructions::EXT_OPS, extendedInstructions::RL_L}, cpu.l, 2);
}

TEST_F(CpuInstructionTest, InstructionRotateLeftFromMemory) {
    testRotateLeftFromMemory({standardInstructions::EXT_OPS, extendedInstructions::RL_HLm}, cpu.h, cpu.l, 4);
}

TEST_F(CpuInstructionTest, InstructionLoad16BitsRegisterAtImmediateAddr) {
    uint16_t expectedValue = 0x559A;
    cpu.sp = expectedValue;
    cpu.pc = 0x00;
    uint16_t addr = 0x5C8;
    mmu.write(cpu.pc, standardInstructions::LD_nnm_SP);
    mmu.writeWord(cpu.pc + 1, addr);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 5);
    ASSERT_EQ(cpu.getFlag(), 0x00);
    uint16_t value = mmu.readWord(addr);
    ASSERT_EQ(value, expectedValue);
    ASSERT_EQ(cpu.pc, 3);
}

TEST_F(CpuInstructionTest, InstructionLoad8BitsRegisterAtImmediateAddr) {
    byte expectedValue = 0x9A;
    cpu.a = expectedValue;
    cpu.pc = 0x00;
    uint16_t addr = 0x5C8;
    mmu.write(cpu.pc, standardInstructions::LD_nnm_A);
    mmu.writeWord(cpu.pc + 1, addr);
    int ticks = cpu.fetchDecodeAndExecute();
    ASSERT_EQ(ticks, 4);
    ASSERT_EQ(cpu.getFlag(), 0x00);
    uint16_t value = mmu.readWord(addr);
    ASSERT_EQ(value, expectedValue);
    ASSERT_EQ(cpu.pc, 3);
}

TEST_F(CpuInstructionTest, InstructionAddTwo8BitsRegisterToTwo8BitsRegister) {
    std::vector<uint16_t> startValues = { 0x0000, 0x00FF, 0xFFFF, 0x0FFF, 0x0FFF };
    std::vector<uint16_t> addValues = { 0x0012, 0x0012, 0x0050, 0x0012, 0x0000 };
    addTwo8BitsRegisterToTwo8BitsRegister(standardInstructions::ADD_HL_BC, cpu.h, cpu.l,
                                          cpu.b, cpu.c, startValues, addValues);
    addTwo8BitsRegisterToTwo8BitsRegister(standardInstructions::ADD_HL_DE, cpu.h, cpu.l,
                                          cpu.d, cpu.e, startValues, addValues);
}

TEST_F(CpuInstructionTest, InstructionAdd16BitsRegisterToTwo8BitsRegister) {
    std::vector<uint16_t> startValues = { 0x0000, 0x00FF, 0xFFFF, 0x0FFF, 0x0FFF };
    std::vector<uint16_t> addValues = { 0x0012, 0x0012, 0x0050, 0x0012, 0x0000 };
    add16BitsRegisterToTwo8BitsRegister(standardInstructions::ADD_HL_SP, cpu.h, cpu.l,
                                        cpu.sp, startValues, addValues);
}

TEST_F(CpuInstructionTest, InstructionAdd8BitsRegisterToSameRegisters) {
    std::vector<uint16_t> startValues = { 0x0012, 0x00FF, 0x0FFF };
    std::vector<uint16_t> addValues = { 0x0012, 0x00FF, 0x0FFF };
    addTwo8BitsRegisterToTwo8BitsRegister(standardInstructions::ADD_HL_HL, cpu.h, cpu.l,
                                          cpu.h, cpu.l, startValues, addValues);
}

TEST_F(CpuInstructionTest, InstructionLoadValueFromMemoryInto8BitsRegister) {
    testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_B_HLm, cpu.b, cpu.h, cpu.l);
    testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_D_HLm, cpu.d, cpu.h, cpu.l);
    testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_H_HLm, cpu.h, cpu.h, cpu.l);
    testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_A_BCm, cpu.a, cpu.b, cpu.c);
    testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_A_DEm, cpu.a, cpu.d, cpu.e);
    testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_C_HLm, cpu.c, cpu.h, cpu.l);
    testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_E_HLm, cpu.e, cpu.h, cpu.l);
    testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_L_HLm, cpu.l, cpu.h, cpu.l);
    testLoadValueFromMemoryInto8BitsRegister(standardInstructions::LD_A_HLm, cpu.a, cpu.h, cpu.l);
}

TEST_F(CpuInstructionTest, InstructionRotateRightCircular) {
    testRotateRightCircular({standardInstructions::RRC_A}, cpu.a, 1);
    testRotateRightCircular({standardInstructions::EXT_OPS, extendedInstructions::RRC_A}, cpu.a, 2);
    testRotateRightCircular({standardInstructions::EXT_OPS, extendedInstructions::RRC_B}, cpu.b, 2);
    testRotateRightCircular({standardInstructions::EXT_OPS, extendedInstructions::RRC_C}, cpu.c, 2);
    testRotateRightCircular({standardInstructions::EXT_OPS, extendedInstructions::RRC_D}, cpu.d, 2);
    testRotateRightCircular({standardInstructions::EXT_OPS, extendedInstructions::RRC_E}, cpu.e, 2);
    testRotateRightCircular({standardInstructions::EXT_OPS, extendedInstructions::RRC_H}, cpu.h, 2);
    testRotateRightCircular({standardInstructions::EXT_OPS, extendedInstructions::RRC_L}, cpu.l, 2);
}

TEST_F(CpuInstructionTest, InstructionRotateRightCircularFromMemory) {
    testRotateRightCircularFromMemory({standardInstructions::EXT_OPS, extendedInstructions::RRC_HLm}, cpu.h, cpu.l, 4);
}

TEST_F(CpuInstructionTest, InstructionRotateRight) {
    testRotateRight({standardInstructions::EXT_OPS, extendedInstructions::RR_A}, cpu.a, 2);
    testRotateRight({standardInstructions::EXT_OPS, extendedInstructions::RR_B}, cpu.b, 2);
    testRotateRight({standardInstructions::EXT_OPS, extendedInstructions::RR_C}, cpu.c, 2);
    testRotateRight({standardInstructions::EXT_OPS, extendedInstructions::RR_D}, cpu.d, 2);
    testRotateRight({standardInstructions::EXT_OPS, extendedInstructions::RR_E}, cpu.e, 2);
    testRotateRight({standardInstructions::EXT_OPS, extendedInstructions::RR_H}, cpu.h, 2);
    testRotateRight({standardInstructions::EXT_OPS, extendedInstructions::RR_L}, cpu.l, 2);
}

TEST_F(CpuInstructionTest, InstructionRotateRightFromMemory) {
    testRotateRightFromMemory({standardInstructions::EXT_OPS, extendedInstructions::RR_HLm}, cpu.h, cpu.l, 4);
}
