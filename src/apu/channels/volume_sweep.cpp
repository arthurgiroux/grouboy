#include "volume_sweep.hpp"

VolumeSweep::VolumeSweep()
{
}

void VolumeSweep::setVolume(int volume)
{
    _volume = volume;
}

void VolumeSweep::setDirection(int sign)
{
    _direction = sign;
}

void VolumeSweep::setPeriod(int period)
{
    _period = period;
    // Setting the period will restart the timer
    _timer = period;
}

int VolumeSweep::getVolume() const
{
    return _volume;
}

void VolumeSweep::tick()
{
    if (_period == 0)
    {
        return;
    }

    _timer--;
    if (_timer == 0)
    {
        _timer = _period;
        _volume = (_volume + _direction);

        // If the value underflows or overflows, we loop it back
        if (_volume < 0)
        {
            _volume = 0xF;
        }
        else if (_volume > 0xF)
        {
            _volume = 0;
        }
    }
}

int VolumeSweep::getPeriod() const
{
    return _period;
}

void VolumeSweep::reset()
{
    _volume = 0;
    _direction = 1;
    _period = 0;
    _timer = 0;
}
