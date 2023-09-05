#include "square_wave.hpp"
#include "common/utils.hpp"

int SquareWave::getAmplitude() const
{
    return utils::isNthBitSet(_wavePatterns[_dutyPattern], _waveDutyPosition) ? 1 : 0;
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
    _waveDutyPosition = (_waveDutyPosition + 1) % 8;
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

byte SquareWave::getDutyPattern() const
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
    _dutyPattern = 0;
    _frequency = 0;
    resetFrequencyTimer();
}

void SquareWave::setDutyPosition(int index)
{
    _waveDutyPosition = index;
}
