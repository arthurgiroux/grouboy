#include "wave.hpp"

template <int SZ>
Wave<SZ>::Wave()
{
}

template <int SZ>
void Wave<SZ>::reset()
{
    // Quirk: the initial wave position is 1 and not 0.
    // Source: https://github.com/LIJI32/SameSuite/blob/master/apu/channel_3/channel_3_first_sample.asm
    _wavePosition = 1;
    resetFrequencyTimer();
}

template <int SZ>
void Wave<SZ>::step(int cycles)
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

template <int SZ>
int Wave<SZ>::getAmplitude() const
{
    return _waveSamples[_wavePosition];
}

template <int SZ>
void Wave<SZ>::setFrequency(int frequency)
{
    if (_frequency != frequency)
    {
        _frequency = frequency;
        resetFrequencyTimer();
    }
}

template <int SZ>
int Wave<SZ>::getFrequency() const
{
    return _frequency;
}

template <int SZ>
void Wave<SZ>::setSample(int index, int value)
{
    _waveSamples[index] = value;
}

template <int SZ>
byte Wave<SZ>::getSample(int index) const
{
    return _waveSamples[index];
}

template <int SZ>
void Wave<SZ>::resetFrequencyTimer()
{
    _frequencyTimerValue = _frequency;
}

template <int SZ>
void Wave<SZ>::nextSample()
{
    _wavePosition = (_wavePosition + 1) % SZ;
}