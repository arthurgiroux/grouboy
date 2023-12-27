#include "common/utils.hpp"
#include "memory/switchable_memory_bank.hpp"
#include <gtest/gtest.h>

class SwitchableMemoryBankTest : public ::testing::Test
{
  protected:
    static const int NBR_BANKS = 2;
    static const int MEM_SIZE = 8_KiB;
    SwitchableMemoryBank<NBR_BANKS, MEM_SIZE> memoryBank;
};

TEST_F(SwitchableMemoryBankTest, ValueAtInitShouldBeZero)
{
    word addr = 0xFF;
    ASSERT_EQ(memoryBank.read(addr), 0);
}

TEST_F(SwitchableMemoryBankTest, WriteAndReadFromSameBankShouldKeepValue)
{
    byte value = 0x0F;
    word addr = 0xFF;
    ASSERT_EQ(memoryBank.read(addr), 0);
    memoryBank.write(addr, value);
    ASSERT_EQ(memoryBank.read(addr), value);
}

TEST_F(SwitchableMemoryBankTest, WriteToOneBankShouldntWriteToOther)
{
    byte value = 0x0F;
    word addr = 0xFF;
    ASSERT_EQ(memoryBank.read(addr), 0);
    memoryBank.write(addr, value);
    ASSERT_EQ(memoryBank.read(addr), value);
    memoryBank.switchBank(1);
    ASSERT_EQ(memoryBank.read(addr), 0);
}

TEST_F(SwitchableMemoryBankTest, WriteAndReadInDifferentBankShouldKeepValue)
{
    byte valueBank1 = 0x0F;
    byte valueBank2 = 0xF0;
    word addr = 0xFF;
    ASSERT_EQ(memoryBank.read(addr), 0);
    memoryBank.write(addr, valueBank1);
    memoryBank.switchBank(1);
    memoryBank.write(addr, valueBank2);
    memoryBank.switchBank(0);
    ASSERT_EQ(memoryBank.read(addr), valueBank1);
    memoryBank.switchBank(1);
    ASSERT_EQ(memoryBank.read(addr), valueBank2);
}

TEST_F(SwitchableMemoryBankTest, GetBankIdShouldReturnActiveBank)
{
    ASSERT_EQ(memoryBank.getBankId(), 0);
    int bankId = 1;
    memoryBank.switchBank(bankId);
    ASSERT_EQ(memoryBank.getBankId(), 1);
}

TEST_F(SwitchableMemoryBankTest, WriteAtMaxValueShouldStoreValueCorrectly)
{
    byte value = 0x0F;
    word addr = MEM_SIZE - 1;
    ASSERT_EQ(memoryBank.read(addr), 0);
    memoryBank.write(addr, value);
    ASSERT_EQ(memoryBank.read(addr), value);
}