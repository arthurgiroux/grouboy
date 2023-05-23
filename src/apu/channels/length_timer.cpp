#include "length_timer.hpp"

LengthTimer::LengthTimer()
{
}

void LengthTimer::tick()
{
    _ticksElapsed++;
}

void LengthTimer::setStartValue(int startValue)
{
    _startValue = startValue;
}

int LengthTimer::getStartValue() const
{
    return _startValue;
}

bool LengthTimer::isTimerElapsed() const
{
    return _ticksElapsed >= DURATION_IN_TICKS;
}

void LengthTimer::restartTimer()
{
    _ticksElapsed = _startValue;
}
