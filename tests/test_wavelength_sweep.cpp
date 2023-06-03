#include "apu/channels/wavelength_sweep.hpp"
#include <gtest/gtest.h>

class WavelengthSweepTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        using std::placeholders::_1;
        wavelengthSweep.setWavelengthChangedCallback(std::bind(&WavelengthSweepTest::onWavelengthChanged, this, _1));
        wavelengthSweep.setWavelengthOverflowCallback(std::bind(&WavelengthSweepTest::onWavelengthOverflow, this));
    }

    void onWavelengthChanged(int wavelength)
    {
        wavelengthChangeDetected = true;
        wavelengthChangeValue = wavelength;
    }

    void onWavelengthOverflow()
    {
        overflowDetected = true;
    }

    void assertWavelengthChanged(bool expected, int value)
    {
        ASSERT_EQ(wavelengthChangeDetected, expected);
        if (wavelengthChangeDetected)
        {
            ASSERT_EQ(wavelengthChangeValue, value);
        }
    }

    void assertWavelengthOverflow(bool expected)
    {
        ASSERT_EQ(overflowDetected, expected);
    }

    WavelengthSweep wavelengthSweep;
    bool overflowDetected = false;
    bool wavelengthChangeDetected = false;
    int wavelengthChangeValue = 0;
};

TEST_F(WavelengthSweepTest, TickWhenPeriodIsZeroShouldNotDoAnything)
{
    int period = 0;
    wavelengthSweep.setPeriod(period);
    ASSERT_EQ(wavelengthSweep.getPeriod(), period);
    for (int i = 0; i < 200; ++i)
    {
        wavelengthSweep.tick();
    }
    assertWavelengthChanged(false, 0);
    assertWavelengthOverflow(false);
}

TEST_F(WavelengthSweepTest, TickWhenPeriodIsSetAndDirectionIsPositiveShouldIncreaseWavelength)
{
    int period = 200;
    wavelengthSweep.setPeriod(period);
    ASSERT_EQ(wavelengthSweep.getPeriod(), period);

    int shift = 1;
    wavelengthSweep.setShift(shift);
    ASSERT_EQ(wavelengthSweep.getShift(), shift);

    int wavelength = 16;
    wavelengthSweep.setWavelength(wavelength);
    ASSERT_EQ(wavelengthSweep.getWavelength(), wavelength);

    int direction = 1;
    wavelengthSweep.setDirection(direction);
    ASSERT_EQ(wavelengthSweep.getDirection(), direction);

    for (int i = 0; i < period - 1; ++i)
    {
        wavelengthSweep.tick();
        assertWavelengthChanged(false, 0);
    }

    wavelengthSweep.tick();

    assertWavelengthChanged(true, 24);
    assertWavelengthOverflow(false);
}

TEST_F(WavelengthSweepTest, TickWhenPeriodIsSetAndDirectionIsNegativeShouldDecreaseWavelength)
{
    int period = 200;
    wavelengthSweep.setPeriod(period);
    ASSERT_EQ(wavelengthSweep.getPeriod(), period);

    int shift = 1;
    wavelengthSweep.setShift(shift);
    ASSERT_EQ(wavelengthSweep.getShift(), shift);

    int wavelength = 16;
    wavelengthSweep.setWavelength(wavelength);
    ASSERT_EQ(wavelengthSweep.getWavelength(), wavelength);

    int direction = -1;
    wavelengthSweep.setDirection(direction);
    ASSERT_EQ(wavelengthSweep.getDirection(), direction);
    for (int i = 0; i < period - 1; ++i)
    {
        wavelengthSweep.tick();
        assertWavelengthChanged(false, 0);
    }

    wavelengthSweep.tick();

    assertWavelengthChanged(true, 8);
    assertWavelengthOverflow(false);
}

TEST_F(WavelengthSweepTest, TickWhenWavelengthOverflowsShouldNotChangeWavelengthAndDetectOverflow)
{
    int period = 200;
    wavelengthSweep.setPeriod(period);
    ASSERT_EQ(wavelengthSweep.getPeriod(), period);

    int shift = 1;
    wavelengthSweep.setShift(shift);
    ASSERT_EQ(wavelengthSweep.getShift(), shift);

    int wavelength = 1536;
    wavelengthSweep.setWavelength(wavelength);
    ASSERT_EQ(wavelengthSweep.getWavelength(), wavelength);

    int direction = 1;
    wavelengthSweep.setDirection(direction);
    ASSERT_EQ(wavelengthSweep.getDirection(), direction);
    for (int i = 0; i < period - 1; ++i)
    {
        wavelengthSweep.tick();
        assertWavelengthChanged(false, 0);
    }

    wavelengthSweep.tick();

    assertWavelengthChanged(false, 0);
    assertWavelengthOverflow(true);
}

TEST_F(WavelengthSweepTest, TickWhenWavelengthPostOverflowsShouldChangeWavelengthAndDetectOverflow)
{
    int period = 200;
    wavelengthSweep.setPeriod(period);
    ASSERT_EQ(wavelengthSweep.getPeriod(), period);

    int shift = 1;
    wavelengthSweep.setShift(shift);
    ASSERT_EQ(wavelengthSweep.getShift(), shift);

    int wavelength = 1024;
    wavelengthSweep.setWavelength(wavelength);
    ASSERT_EQ(wavelengthSweep.getWavelength(), wavelength);

    int direction = 1;
    wavelengthSweep.setDirection(direction);
    ASSERT_EQ(wavelengthSweep.getDirection(), direction);
    for (int i = 0; i < period - 1; ++i)
    {
        wavelengthSweep.tick();
        assertWavelengthChanged(false, 0);
    }

    wavelengthSweep.tick();

    assertWavelengthChanged(true, 1536);
    assertWavelengthOverflow(true);
}