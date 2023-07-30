#include "apu/channels/frequency_sweep.hpp"
#include <gtest/gtest.h>

class FrequencySweepTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        using std::placeholders::_1;
        frequencySweep.setFrequencyChangedCallback(std::bind(&FrequencySweepTest::onFrequencyChanged, this, _1));
        frequencySweep.setFrequencyOverflowCallback(std::bind(&FrequencySweepTest::onFrequencyOverflow, this));
    }

    void onFrequencyChanged(int frequency)
    {
        frequencyChangeDetected = true;
        frequencyChangeValue = frequency;
    }

    void onFrequencyOverflow()
    {
        overflowDetected = true;
    }

    void assertFrequencyChanged(bool expected, int value)
    {
        ASSERT_EQ(frequencyChangeDetected, expected);
        if (frequencyChangeDetected)
        {
            ASSERT_EQ(frequencyChangeValue, value);
        }
    }

    void assertFrequencyOverflow(bool expected)
    {
        ASSERT_EQ(overflowDetected, expected);
    }

    FrequencySweep frequencySweep;
    bool overflowDetected = false;
    bool frequencyChangeDetected = false;
    int frequencyChangeValue = 0;
};

TEST_F(FrequencySweepTest, TickWhenPeriodIsZeroShouldNotDoAnything)
{
    int period = 0;
    frequencySweep.setPeriod(period);
    ASSERT_EQ(frequencySweep.getPeriod(), period);
    for (int i = 0; i < 200; ++i)
    {
        frequencySweep.tick();
    }
    assertFrequencyChanged(false, 0);
    assertFrequencyOverflow(false);
}

TEST_F(FrequencySweepTest, TickWhenPeriodIsSetAndDirectionIsPositiveShouldIncreaseFrequency)
{
    int period = 200;
    frequencySweep.setPeriod(period);
    ASSERT_EQ(frequencySweep.getPeriod(), period);

    int shift = 1;
    frequencySweep.setShift(shift);
    ASSERT_EQ(frequencySweep.getShift(), shift);

    int frequency = 16;
    frequencySweep.setFrequency(frequency);
    ASSERT_EQ(frequencySweep.getFrequency(), frequency);

    int direction = 1;
    frequencySweep.setDirection(direction);
    ASSERT_EQ(frequencySweep.getDirection(), direction);
    frequencySweep.setEnabled(true);
    frequencySweep.resetTimer();

    for (int i = 0; i < period - 1; ++i)
    {
        frequencySweep.tick();
        assertFrequencyChanged(false, 0);
    }

    frequencySweep.tick();

    assertFrequencyChanged(true, 24);
    assertFrequencyOverflow(false);
}

TEST_F(FrequencySweepTest, TickWhenPeriodIsSetAndDirectionIsNegativeShouldDecreaseFrequency)
{
    int period = 200;
    frequencySweep.setPeriod(period);
    ASSERT_EQ(frequencySweep.getPeriod(), period);

    int shift = 1;
    frequencySweep.setShift(shift);
    ASSERT_EQ(frequencySweep.getShift(), shift);

    int frequency = 16;
    frequencySweep.setFrequency(frequency);
    ASSERT_EQ(frequencySweep.getFrequency(), frequency);

    int direction = -1;
    frequencySweep.setDirection(direction);
    ASSERT_EQ(frequencySweep.getDirection(), direction);

    frequencySweep.setEnabled(true);
    frequencySweep.resetTimer();

    for (int i = 0; i < period - 1; ++i)
    {
        frequencySweep.tick();
        assertFrequencyChanged(false, 0);
    }

    frequencySweep.tick();

    assertFrequencyChanged(true, 8);
    assertFrequencyOverflow(false);
}

TEST_F(FrequencySweepTest, TickWhenFrequencyOverflowsShouldNotChangeFrequencyAndDetectOverflow)
{
    int period = 200;
    frequencySweep.setPeriod(period);
    ASSERT_EQ(frequencySweep.getPeriod(), period);

    int shift = 1;
    frequencySweep.setShift(shift);
    ASSERT_EQ(frequencySweep.getShift(), shift);

    int frequency = 1536;
    frequencySweep.setFrequency(frequency);
    ASSERT_EQ(frequencySweep.getFrequency(), frequency);

    int direction = 1;
    frequencySweep.setDirection(direction);
    ASSERT_EQ(frequencySweep.getDirection(), direction);
    frequencySweep.setEnabled(true);
    frequencySweep.resetTimer();

    for (int i = 0; i < period - 1; ++i)
    {
        frequencySweep.tick();
        assertFrequencyChanged(false, 0);
    }

    frequencySweep.tick();

    assertFrequencyChanged(false, 0);
    assertFrequencyOverflow(true);
}

TEST_F(FrequencySweepTest, TickWhenFrequencyPostOverflowsShouldChangeFrequencyAndDetectOverflow)
{
    int period = 200;
    frequencySweep.setPeriod(period);
    ASSERT_EQ(frequencySweep.getPeriod(), period);

    int shift = 1;
    frequencySweep.setShift(shift);
    ASSERT_EQ(frequencySweep.getShift(), shift);

    int frequency = 1024;
    frequencySweep.setFrequency(frequency);
    ASSERT_EQ(frequencySweep.getFrequency(), frequency);

    int direction = 1;
    frequencySweep.setDirection(direction);
    ASSERT_EQ(frequencySweep.getDirection(), direction);
    frequencySweep.setEnabled(true);
    frequencySweep.resetTimer();

    for (int i = 0; i < period - 1; ++i)
    {
        frequencySweep.tick();
        assertFrequencyChanged(false, 0);
    }

    frequencySweep.tick();

    assertFrequencyChanged(true, 1536);
    assertFrequencyOverflow(true);
}