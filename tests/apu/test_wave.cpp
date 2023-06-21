#include "apu/signal/wave.hpp"
#include <gtest/gtest.h>

class WaveTest : public ::testing::Test
{
  protected:
    static constexpr int WAVE_SIZE = 32;
    Wave<WAVE_SIZE> wave;
};

TEST_F(WaveTest, SetFrequencyShouldChangeFrequency)
{
    int frequency = 2048;
    wave.setFrequency(frequency);
    ASSERT_EQ(wave.getFrequency(), frequency);
}

TEST_F(WaveTest, SetSampleShouldChangeSample)
{
    for (int i = 0; i < WAVE_SIZE; ++i)
    {
        wave.setSample(i, i + 1);
        ASSERT_EQ(wave.getSample(i), i + 1);
    }
}

TEST_F(WaveTest, StepShouldCycleCorrectlyThroughSamples)
{
    int frequency = 2048;
    for (int i = 0; i < WAVE_SIZE; ++i)
    {
        wave.setSample(i, i + 1);
        ASSERT_EQ(wave.getSample(i), i + 1);
    }

    wave.setFrequency(frequency);
    wave.resetFrequencyTimer();
    for (int i = 0; i < WAVE_SIZE; ++i)
    {
        for (int j = 0; j < frequency; ++j)
        {
            ASSERT_EQ(wave.getAmplitude(), i + 1);
            wave.step(1);
        }
    }
}

TEST_F(WaveTest, StepShouldRewindBackToFirstSampleWhenSignalSizeReached)
{
    int frequency = 2048;
    for (int i = 0; i < WAVE_SIZE; ++i)
    {
        wave.setSample(i, i + 1);
        ASSERT_EQ(wave.getSample(i), i + 1);
    }

    wave.setFrequency(frequency);
    for (int i = 0; i < WAVE_SIZE - 1; ++i)
    {
        wave.step(frequency);
    }
    ASSERT_EQ(wave.getAmplitude(), WAVE_SIZE);

    // Next cycle should start from sample 0
    wave.step(frequency);
    ASSERT_EQ(wave.getAmplitude(), wave.getSample(0));
}