#include "length_timer.hpp"

LengthTimer::LengthTimer(int durationInTicks) : _durationInTicks(durationInTicks)
{
}

void LengthTimer::tick()
{
    _ticksElapsed++;
}

void LengthTimer::setStartValue(int startValue)
{
    _startValue = startValue;
    restartTimer();
}

int LengthTimer::getStartValue() const
{
    return _startValue;
}

bool LengthTimer::isTimerElapsed() const
{
    return _ticksElapsed >= _durationInTicks;
}

void LengthTimer::restartTimer()
{
    _ticksElapsed = _startValue;
}

int LengthTimer::getDuration() const
{
    return _durationInTicks;
}

void LengthTimer::reset()
{
    _startValue = 0;
    _ticksElapsed = 0;
}
