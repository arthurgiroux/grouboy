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
    if (_waveDutyPosition > 7)
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
    // TODO: Document this formula
    _frequencyTimerValue = (2048 - _frequency) * 4;
}

void SquareWave::setDutyPattern(int index)
{
    _dutyPattern = index;
}
