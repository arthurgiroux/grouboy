#include "apu/channels/volume_sweep.hpp"
#include <gtest/gtest.h>

class VolumeSweepTest : public ::testing::Test
{
  protected:
    VolumeSweep volumeSweep;
};

TEST_F(VolumeSweepTest, SetPeriodShouldSetPeriodCorrectly)
{
    ASSERT_EQ(volumeSweep.getPeriod(), 0);
    int period = 256;
    volumeSweep.setPeriod(period);
    ASSERT_EQ(volumeSweep.getPeriod(), period);
}

TEST_F(VolumeSweepTest, SetVolumeShouldSetVolumeCorrectly)
{
    ASSERT_EQ(volumeSweep.getVolume(), 0);
    int volume = 5;
    volumeSweep.setVolume(volume);
    ASSERT_EQ(volumeSweep.getVolume(), volume);
}

TEST_F(VolumeSweepTest, TickWhenPeriodIsNotReachedShouldNotChangeVolume)
{
    int period = 10;
    volumeSweep.setPeriod(period);
    int expectedVolume = 5;
    volumeSweep.setVolume(expectedVolume);
    ASSERT_EQ(volumeSweep.getVolume(), expectedVolume);
    volumeSweep.tick();
    ASSERT_EQ(volumeSweep.getVolume(), expectedVolume);
}

TEST_F(VolumeSweepTest, TickWhenPeriodIsReachedAndDirectionIsPositiveShouldIncrementVolume)
{
    int period = 10;
    volumeSweep.setPeriod(period);
    volumeSweep.setDirection(1);
    int expectedVolume = 5;
    volumeSweep.setVolume(expectedVolume);
    for (int i = 0; i < period; ++i)
    {
        ASSERT_EQ(volumeSweep.getVolume(), expectedVolume);
        volumeSweep.tick();
    }
    ASSERT_EQ(volumeSweep.getVolume(), expectedVolume + 1);
}

TEST_F(VolumeSweepTest, TickShouldNotOverflowVolumeOn4Bits)
{
    int period = 10;
    volumeSweep.setPeriod(period);
    volumeSweep.setDirection(1);
    int expectedVolume = 0x0F;
    volumeSweep.setVolume(expectedVolume);
    for (int i = 0; i < period; ++i)
    {
        ASSERT_EQ(volumeSweep.getVolume(), expectedVolume);
        volumeSweep.tick();
    }
    ASSERT_EQ(volumeSweep.getVolume(), 0x0F);
}

TEST_F(VolumeSweepTest, TickWhenPeriodIsReachedAndDirectionIsPositiveShouldDecrementVolume)
{
    int period = 10;
    volumeSweep.setPeriod(period);
    volumeSweep.setDirection(-1);
    int expectedVolume = 5;
    volumeSweep.setVolume(expectedVolume);
    for (int i = 0; i < period; ++i)
    {
        ASSERT_EQ(volumeSweep.getVolume(), expectedVolume);
        volumeSweep.tick();
    }
    ASSERT_EQ(volumeSweep.getVolume(), expectedVolume - 1);
}

TEST_F(VolumeSweepTest, TickShoulNotUnderflowVolumeOn4Bits)
{
    int period = 10;
    volumeSweep.setPeriod(period);
    volumeSweep.setDirection(-1);
    int expectedVolume = 0x00;
    volumeSweep.setVolume(expectedVolume);
    for (int i = 0; i < period; ++i)
    {
        ASSERT_EQ(volumeSweep.getVolume(), expectedVolume);
        volumeSweep.tick();
    }
    ASSERT_EQ(volumeSweep.getVolume(), 0x00);
}