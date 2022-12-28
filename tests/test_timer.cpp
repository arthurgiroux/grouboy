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

TEST_F(TimerTest, SetTimerModuloValueShouldSetValueCorrectly)
{
    int timerModuloValue = 0x05;
    timer->setTimerModuloValue(timerModuloValue);
    ASSERT_EQ(timer->getTimerModuloValue(), timerModuloValue);
}

TEST_F(TimerTest, EnableTimerCounterShouldEnableTimer)
{
    ASSERT_FALSE(timer->isTimerCounterEnabled());
    timer->enableTimerCounter();
    ASSERT_TRUE(timer->isTimerCounterEnabled());
}

TEST_F(TimerTest, DisableTimerCounterShouldDisableTimer)
{
    timer->enableTimerCounter();
    ASSERT_TRUE(timer->isTimerCounterEnabled());
    timer->disableTimerCounter();
    ASSERT_FALSE(timer->isTimerCounterEnabled());
}

TEST_F(TimerTest, SetClockDividerShouldSetExpectedValue)
{
    int clockDividerValue = 1024;
    timer->setClockDivider(clockDividerValue);
    ASSERT_EQ(timer->getClockDivider(), clockDividerValue);
    clockDividerValue = 256;
    timer->setClockDivider(clockDividerValue);
    ASSERT_EQ(timer->getClockDivider(), clockDividerValue);
}

TEST_F(TimerTest, TimerCounterAt4096HzShouldIncrementEvery256Tick)
{
    timer->enableTimerCounter();
    int clockDivider = 1024;
    int ticksToIncrement = clockDivider / 4;
    timer->setClockDivider(clockDivider);
    ASSERT_EQ(timer->getTimerCounterValue(), 0);
    timer->tick(ticksToIncrement - 1);
    ASSERT_EQ(timer->getTimerCounterValue(), 0);
    timer->tick(1);
    ASSERT_EQ(timer->getTimerCounterValue(), 1);
}

TEST_F(TimerTest, TimerCounterAt262144HzShouldIncrementEvery4Tick)
{
    timer->enableTimerCounter();
    int clockDivider = 16;
    int ticksToIncrement = clockDivider / 4;
    timer->setClockDivider(clockDivider);
    ASSERT_EQ(timer->getTimerCounterValue(), 0);
    timer->tick(ticksToIncrement - 1);
    ASSERT_EQ(timer->getTimerCounterValue(), 0);
    timer->tick(1);
    ASSERT_EQ(timer->getTimerCounterValue(), 1);
}

TEST_F(TimerTest, TimerCounterAt65536HzShouldIncrementEvery16Tick)
{
    timer->enableTimerCounter();
    int clockDivider = 64;
    int ticksToIncrement = clockDivider / 4;
    timer->setClockDivider(clockDivider);
    ASSERT_EQ(timer->getTimerCounterValue(), 0);
    timer->tick(ticksToIncrement - 1);
    ASSERT_EQ(timer->getTimerCounterValue(), 0);
    timer->tick(1);
    ASSERT_EQ(timer->getTimerCounterValue(), 1);
}

TEST_F(TimerTest, TimerCounterAt16384HzShouldIncrementEvery64Tick)
{
    timer->enableTimerCounter();
    int clockDivider = 256;
    int ticksToIncrement = clockDivider / 4;
    timer->setClockDivider(clockDivider);
    ASSERT_EQ(timer->getTimerCounterValue(), 0);
    timer->tick(ticksToIncrement - 1);
    ASSERT_EQ(timer->getTimerCounterValue(), 0);
    timer->tick(1);
    ASSERT_EQ(timer->getTimerCounterValue(), 1);
}

TEST_F(TimerTest, TimerCounterShouldOverflowToModuloValue)
{
    timer->enableTimerCounter();
    int clockDivider = 256;
    int ticksToIncrement = clockDivider / 4;
    int moduloValue = 0x42;
    timer->setTimerModuloValue(moduloValue);
    timer->setClockDivider(clockDivider);

    int maxTimerValue = 0xFF;
    for (int i = 0; i < maxTimerValue; ++i)
    {
        timer->tick(ticksToIncrement);
        ASSERT_EQ(timer->getTimerCounterValue(), i + 1);
    }
    timer->tick(ticksToIncrement);
    ASSERT_EQ(timer->getTimerCounterValue(), moduloValue);
}

TEST_F(TimerTest, TimerCounterShouldRaiseTimerInterruptOnOverflow)
{
    timer->enableTimerCounter();
    int clockDivider = 256;
    int ticksToIncrement = clockDivider / 4;
    timer->setClockDivider(clockDivider);

    int maxTimerValue = 0xFF;
    for (int i = 0; i < maxTimerValue; ++i)
    {
        timer->tick(ticksToIncrement);
        ASSERT_EQ(timer->getTimerCounterValue(), i + 1);
    }
    ASSERT_FALSE(interruptManager->isInterruptPending(InterruptType::TIMER));
    timer->tick(ticksToIncrement);
    ASSERT_TRUE(interruptManager->isInterruptPending(InterruptType::TIMER));
}

TEST_F(TimerTest, InternalTimerCounterShouldResetWhenChangingClockDivider)
{
    timer->enableTimerCounter();
    int clockDivider = 256;
    int ticksToIncrement = clockDivider / 4;
    timer->setClockDivider(clockDivider);
    ASSERT_EQ(timer->getTimerCounterValue(), 0);
    timer->tick(ticksToIncrement - 1);
    ASSERT_EQ(timer->getTimerCounterValue(), 0);
    clockDivider = 1024;
    ticksToIncrement = clockDivider / 4;
    timer->setClockDivider(clockDivider);
    timer->tick(1);
    ASSERT_EQ(timer->getTimerCounterValue(), 0);
    timer->tick(ticksToIncrement);
    ASSERT_EQ(timer->getTimerCounterValue(), 1);
}