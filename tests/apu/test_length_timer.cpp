#include "apu/channels/length_timer.hpp"
#include <gtest/gtest.h>

class LengthTimerTest : public ::testing::Test
{
  protected:
    LengthTimer lengthTimer;
};

TEST_F(LengthTimerTest, SetStartValueShouldSetInitialValueCorrectly)
{
    ASSERT_EQ(lengthTimer.getStartValue(), 0);
    int startValue = 10;
    lengthTimer.setStartValue(startValue);
    ASSERT_EQ(lengthTimer.getStartValue(), startValue);
}

TEST_F(LengthTimerTest, IsTimerElapsedShouldReturnFalseIfTickedForLessThanDuration)
{
    int timerDuration = 64;
    for (int i = 0; i < timerDuration; ++i)
    {
        ASSERT_FALSE(lengthTimer.isTimerElapsed());
        lengthTimer.tick();
    }
}

TEST_F(LengthTimerTest, IsTimerElapsedShouldReturnTrueIfTickedForTimerDuration)
{
    int timerDuration = 64;
    for (int i = 0; i < timerDuration; ++i)
    {
        ASSERT_FALSE(lengthTimer.isTimerElapsed());
        lengthTimer.tick();
    }
    ASSERT_TRUE(lengthTimer.isTimerElapsed());
}

TEST_F(LengthTimerTest, SetStartValueShouldImpactTimerDuration)
{
    int startValue = 10;
    lengthTimer.setStartValue(startValue);
    lengthTimer.restartTimer();
    int timerDuration = 64;
    for (int i = 0; i < timerDuration - startValue; ++i)
    {
        ASSERT_FALSE(lengthTimer.isTimerElapsed());
        lengthTimer.tick();
    }
    ASSERT_TRUE(lengthTimer.isTimerElapsed());
}

TEST_F(LengthTimerTest, RestartTimerShouldRestartCounterToStartValue)
{
    int startValue = 10;
    lengthTimer.setStartValue(startValue);
    lengthTimer.restartTimer();
    int timerDuration = 64;
    for (int i = 0; i < timerDuration - startValue; ++i)
    {
        ASSERT_FALSE(lengthTimer.isTimerElapsed());
        lengthTimer.tick();
    }
    lengthTimer.setStartValue(timerDuration - 1);
    lengthTimer.restartTimer();
    ASSERT_FALSE(lengthTimer.isTimerElapsed());
    lengthTimer.tick();
    ASSERT_TRUE(lengthTimer.isTimerElapsed());
}
