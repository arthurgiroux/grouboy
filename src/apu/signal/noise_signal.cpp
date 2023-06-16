#include "noise_signal.hpp"
#include "common/utils.hpp"
#include "spdlog/spdlog.h"

NoiseSignal::NoiseSignal()
{
}

void NoiseSignal::reset()
{
    _lfsrValue = 0;
    resetFrequencyTimer();
}

void NoiseSignal::step(int cycles)
{
    _frequencyTimerValue -= cycles;
    // Timer reached the end
    if (_frequencyTimerValue <= 0)
    {
        int value = _frequencyTimerValue;
        resetFrequencyTimer();

        // If we have remainder we deduct it from the new value
        if (value < 0)
        {
            _frequencyTimerValue -= abs(value);
        }

        nextSample();
    }
}

int NoiseSignal::getAmplitude() const
{
    return _lfsrValue & 0x01;
}

void NoiseSignal::setFrequency(int frequency)
{
    if (_frequency != frequency)
    {
        _frequency = frequency;
    }
}

int NoiseSignal::getFrequency() const
{
    return _frequency;
}

void NoiseSignal::enableWideMode(bool enabled)
{
    _wideModeEnabled = enabled;
}

bool NoiseSignal::isWideModeEnabled() const
{
    return _wideModeEnabled;
}

void NoiseSignal::nextSample()
{
    int val = (~((_lfsrValue) ^ (_lfsrValue >> 1)) & 0x01);
    utils::setNthBit(_lfsrValue, 15, val);
    if (!_wideModeEnabled)
    {
        utils::setNthBit(_lfsrValue, 7, val);
    }

    _lfsrValue >>= 1;
}

void NoiseSignal::resetFrequencyTimer()
{
    _frequencyTimerValue = _frequency;
}
