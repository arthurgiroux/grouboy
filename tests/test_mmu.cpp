#include "memory/mmu.hpp"
#include <gtest/gtest.h>

TEST(MMU, ByteReadingWritingValidMemoryShouldSucceed)
{
    auto mmu = MMU();

    for (int i = 0; i < MMU::MEMORY_SIZE_IN_BYTES; i++)
    {
        mmu.write(i, i);
        int expectedValue = i;
        // Special case, writing to timer register address resets value to 0
        int timerRegisterAddr = 0xFF04;
        if (i == timerRegisterAddr)
        {
            expectedValue = 0;
        }
        ASSERT_EQ(static_cast<uint8_t>(expectedValue), mmu.read(i));
    }
}

TEST(MMU, WordReadingWritingValidMemoryShouldSucceed)
{
    auto mmu = MMU();

    for (int i = 0; i < MMU::MEMORY_SIZE_IN_BYTES - 1; i += 2)
    {
        mmu.writeWord(i, i);
        int expectedValue = i;
        // Special case, writing to timer register address resets value to 0
        int timerRegisterAddr = 0xFF04;
        if (i == timerRegisterAddr)
        {
            expectedValue = 0xFF00;
        }
        ASSERT_EQ(static_cast<word>(expectedValue), mmu.readWord(i));
    }
}

TEST(MMU, WordReadingInvalidMemoryShouldThrow)
{
    auto mmu = MMU();

    EXPECT_THROW(mmu.readWord(MMU::MEMORY_SIZE_IN_BYTES - 1), MMU::InvalidMemoryAccessException);
}

TEST(MMU, WordWritingInvalidMemoryShouldThrow)
{
    auto mmu = MMU();

    EXPECT_THROW(mmu.writeWord(MMU::MEMORY_SIZE_IN_BYTES - 1, 0), MMU::InvalidMemoryAccessException);
}

TEST(MMU, MemoryShouldContainBios)
{
    auto mmu = MMU();

    for (int i = 0; i < sizeof(MMU::BIOS); i++)
    {
        ASSERT_EQ(MMU::BIOS[i], mmu.read(i));
    }
}