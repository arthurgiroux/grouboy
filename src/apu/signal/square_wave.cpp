#include "square_wave.hpp"

int SquareWave::getAmplitude() const
{
    return _wavePatterns[_dutyPattern][_waveDutyPosition];
}

SquareWave::SquareWave()
{
}

void SquareWave::step(int cycles)
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

        nextWaveValue();
    }
}

void SquareWave::nextWaveValue()
{
    _waveDutyPosition++;
    if (_waveDutyPosition > _wavePatterns[_dutyPattern].size())
    {
        _waveDutyPosition = 0;
    }
}

void SquareWave::setFrequency(int frequency)
{
    if (_frequency != frequency)
    {
        _frequency = frequency;
        resetFrequencyTimer();
    }
}

void SquareWave::resetFrequencyTimer()
{
    _frequencyTimerValue = _frequency;
}

void SquareWave::setDutyPattern(int index)
{
    _dutyPattern = index;
}

int SquareWave::getDutyPattern() const
{
    return _dutyPattern;
}

int SquareWave::getFrequency() const
{
    return _frequency;
}

void SquareWave::reset()
{
    _waveDutyPosition = 0;
    resetFrequencyTimer();
}
