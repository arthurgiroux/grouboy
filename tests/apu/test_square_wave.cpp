#include "apu/signal/square_wave.hpp"
#include <gtest/gtest.h>

class SquareWaveTest : public ::testing::Test
{
  protected:
    SquareWave wave;
};

TEST_F(SquareWaveTest, SetFrequencyShouldChangeFrequency)
{
    int frequency = 2048;
    wave.setFrequency(frequency);
    ASSERT_EQ(wave.getFrequency(), frequency);
}

TEST_F(SquareWaveTest, SetDutyPatternShouldChangeDutyPattern)
{
    int dutyPattern = 2;
    wave.setDutyPattern(dutyPattern);
    ASSERT_EQ(wave.getDutyPattern(), dutyPattern);
}

TEST_F(SquareWaveTest, Step)
{
    int frequency = 2048;
    int dutyPattern = 2;
    int expectedValue = 0b10000111;
    // 0b11100001
    wave.setFrequency(frequency);
    wave.setDutyPattern(dutyPattern);
    ASSERT_EQ(wave.getAmplitude(), 1);
    for (int i = 0; i < frequency; ++i)
    {
        ASSERT_EQ(wave.getAmplitude(), 1);
    }
    wave.setDutyPattern(dutyPattern);
    ASSERT_EQ(wave.getDutyPattern(), dutyPattern);
}