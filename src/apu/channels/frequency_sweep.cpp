#include "frequency_sweep.hpp"

FrequencySweep::FrequencySweep()
{
}

void FrequencySweep::tick()
{
    _timer--;

    if (_timer <= 0)
    {
        resetTimer();

        if (_period > 0 && _enabled)
        {
            int newFrequency = generateNewFrequency();

            bool overflowDetected = didFrequencyOverflow(newFrequency);
            if (!overflowDetected && _shift > 0)
            {
                _frequency = newFrequency;
                _frequencyChangedCallback(newFrequency);
                int nextFrequency = generateNewFrequency();
                if (didFrequencyOverflow(nextFrequency))
                {
                    _frequencyOverflowCallback();
                }
            }
            else if (overflowDetected)
            {
                _frequencyOverflowCallback();
            }
        }
    }
}

void FrequencySweep::trigger()
{
    resetTimer();
    // When triggering from a channel, the frequency should be updated if necessary
    if (_shift > 0)
    {
        int newFrequency = generateNewFrequency();

        bool overflowDetected = didFrequencyOverflow(newFrequency);
        if (!overflowDetected)
        {
            _frequency = newFrequency;
            _frequencyChangedCallback(newFrequency);
        }
        else
        {
            _frequencyOverflowCallback();
        }
    }
}

void FrequencySweep::resetTimer()
{
    // Quirky behavior: When the period is zero the timer is set to 8
    _timer = _period > 0 ? _period : 8;
}

bool FrequencySweep::didFrequencyOverflow(int frequency) const
{
    return frequency >= 2048;
}

int FrequencySweep::generateNewFrequency() const
{
    return _frequency + _sign * (_frequency >> _shift);
}

int FrequencySweep::getDirection() const
{
    return _sign;
}

int FrequencySweep::getPeriod() const
{
    return _period;
}

void FrequencySweep::setPeriod(int period)
{
    _period = period;
}

void FrequencySweep::setDirection(int sign)
{
    _sign = sign;
}

void FrequencySweep::setFrequency(int frequency)
{
    _frequency = frequency;
}

void FrequencySweep::setFrequencyChangedCallback(FrequencySweep::FrequencyChangedCallback cb)
{
    _frequencyChangedCallback = cb;
}

void FrequencySweep::setFrequencyOverflowCallback(FrequencySweep::FrequencyOverflowCallback cb)
{
    _frequencyOverflowCallback = cb;
}

void FrequencySweep::setShift(int shift)
{
    _shift = shift;
}

int FrequencySweep::getShift() const
{
    return _shift;
}

int FrequencySweep::getFrequency() const
{
    return _frequency;
}

void FrequencySweep::reset()
{
    _timer = 0;
    _period = 0;
    _frequency = 0;
    _sign = 1;
    _shift = 0;
}

void FrequencySweep::setEnabled(bool value)
{
    _enabled = value;
}
