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

TEST_F(SquareWaveTest, StepShouldCycleCorrectlyThroughPattern)
{
    int frequency = 2048;
    int dutyPattern = 2;
    int expectedValue = 0b10000111;
    wave.setFrequency(frequency);
    wave.setDutyPattern(dutyPattern);
    ASSERT_EQ(wave.getDutyPattern(), dutyPattern);
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < frequency; ++j)
        {
            ASSERT_EQ(wave.getAmplitude(), (expectedValue >> (i)) & 0x01);
            wave.step(1);
        }
    }
}