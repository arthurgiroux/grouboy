#include "cpu/cpu.hpp"
#include "memory/mmu.hpp"
#include "timer/timer.hpp"

#include <gtest/gtest.h>

class TimerTest : public ::testing::Test
{
  protected:
    virtual void SetUp() override
    {
        cpu = std::make_unique<CPU>(mmu);
        interruptManager = std::make_unique<InterruptManager>(cpu.get(), &mmu);
        timer = std::make_unique<Timer>(&mmu, interruptManager.get());
    }

    MMU mmu;
    std::unique_ptr<CPU> cpu;
    std::unique_ptr<InterruptManager> interruptManager;
    std::unique_ptr<Timer> timer;
    /*
     * The div register runs at 16384Hz.
     * Since the CPU clocks runs at 4194304Hz, the div register
     * will be incremented every 256 CPU cycles.
     * We convert the number of CPU cycles to ticks by dividing by 4.
     */
    static const int NBR_TICKS_TO_INCREASE_DIV_REGISTER = 64;
};

TEST_F(TimerTest, DivRegisterValueShouldBe0AtInit)
{
    ASSERT_EQ(timer->getDividerRegisterValue(), 0);
}

TEST_F(TimerTest, DivRegisterShouldTickAt16384Hz)
{
    ASSERT_EQ(timer->getDividerRegisterValue(), 0);
    timer->tick(NBR_TICKS_TO_INCREASE_DIV_REGISTER - 1);
    ASSERT_EQ(timer->getDividerRegisterValue(), 0);
    timer->tick(1);
    ASSERT_EQ(timer->getDividerRegisterValue(), 1);
}

TEST_F(TimerTest, WritingToDividerRegisterShouldResetValueTo0)
{
    ASSERT_EQ(timer->getDividerRegisterValue(), 0);
    int expectedDivValue = 4;
    for (int i = 0; i < expectedDivValue; ++i)
    {
        timer->tick(NBR_TICKS_TO_INCREASE_DIV_REGISTER);
    }
    ASSERT_EQ(timer->getDividerRegisterValue(), expectedDivValue);
    mmu.write(Timer::DIVIDER_REGISTER_ADDR, 0x12);
    ASSERT_EQ(timer->getDividerRegisterValue(), 0);
}

TEST_F(TimerTest, DividerRegisterShouldOverflowTo0)
{
    int expectedDivValue = 255;
    for (int i = 0; i < expectedDivValue; ++i)
    {
        timer->tick(NBR_TICKS_TO_INCREASE_DIV_REGISTER);
        ASSERT_EQ(timer->getDividerRegisterValue(), i + 1);
    }
    timer->tick(NBR_TICKS_TO_INCREASE_DIV_REGISTER);
    ASSERT_EQ(timer->getDividerRegisterValue(), 0);
}