#include "wavelength_sweep.hpp"
#include <functional>

WavelengthSweep::WavelengthSweep()
{
}

void WavelengthSweep::tick()
{
    _timer--;

    if (_timer <= 0)
    {
        // TODO: check about 8 value
        _timer = _period ? _period : 8;

        if (_period > 0)
        {
            int newFrequency = generateNewFrequency();

            if (!didFrequencyOverflow(newFrequency) && _shift > 0)
            {
                _wavelength = newFrequency;
                _wavelengthChangedCallback(newFrequency);
                int nextFrequency = generateNewFrequency();
                if (didFrequencyOverflow(nextFrequency))
                {
                    _wavelengthOverflowCallback();
                }
            }
            else
            {
                _wavelengthOverflowCallback();
            }
        }
    }
}

bool WavelengthSweep::didFrequencyOverflow(int frequency) const
{
    return frequency > 2048;
}

int WavelengthSweep::generateNewFrequency() const
{
    return _wavelength + _sign * (_wavelength >> _shift);
}

int WavelengthSweep::getDirection() const
{
    return _sign;
}

int WavelengthSweep::getPeriod() const
{
    return _period;
}

void WavelengthSweep::setPeriod(int period)
{
    _period = period;
}

void WavelengthSweep::setDirection(int sign)
{
    _sign = sign;
}

void WavelengthSweep::setWavelength(int wavelength)
{
    _wavelength = wavelength;
}

void WavelengthSweep::setWavelengthChangedCallback(WavelengthSweep::WavelengthChangedCallback callback)
{
    _wavelengthChangedCallback = callback;
}

void WavelengthSweep::setWavelengthOverflowCallback(WavelengthSweep::WavelengthOverflowCallback callback)
{
    _wavelengthOverflowCallback = callback;
}

void WavelengthSweep::setShift(int shift)
{
    _shift = shift;
}

int WavelengthSweep::getShift() const
{
    return _shift;
}