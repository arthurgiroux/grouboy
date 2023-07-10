#include "wavelength_sweep.hpp"

WavelengthSweep::WavelengthSweep()
{
}

void WavelengthSweep::tick()
{
    _timer--;

    if (_timer <= 0)
    {
        resetTimer();

        if (_period > 0 && _enabled)
        {
            int newWavelength = generateNewWavelength();

            bool overflowDetected = didWavelengthOverflow(newWavelength);
            if (!overflowDetected && _shift > 0)
            {
                _wavelength = newWavelength;
                _wavelengthChangedCallback(newWavelength);
                int nextWavelength = generateNewWavelength();
                if (didWavelengthOverflow(nextWavelength))
                {
                    _wavelengthOverflowCallback();
                }
            }
            else if (overflowDetected)
            {
                _wavelengthOverflowCallback();
            }
        }
    }
}

void WavelengthSweep::trigger()
{
    resetTimer();
    // When triggering from a channel, the wavelength should be updated if necessary
    if (_shift > 0)
    {
        int newWavelength = generateNewWavelength();

        bool overflowDetected = didWavelengthOverflow(newWavelength);
        if (!overflowDetected)
        {
            _wavelength = newWavelength;
            _wavelengthChangedCallback(newWavelength);
        }
        else
        {
            _wavelengthOverflowCallback();
        }
    }
}

void WavelengthSweep::resetTimer()
{
    // Quirky behavior: When the period is zero the timer is set to 8
    _timer = _period > 0 ? _period : 8;
}

bool WavelengthSweep::didWavelengthOverflow(int wavelength) const
{
    return wavelength >= 2048;
}

int WavelengthSweep::generateNewWavelength() const
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

int WavelengthSweep::getWavelength() const
{
    return _wavelength;
}

void WavelengthSweep::reset()
{
    _timer = 0;
    _period = 0;
    _wavelength = 0;
    _sign = 1;
    _shift = 0;
}

void WavelengthSweep::setEnabled(bool value)
{
    _enabled = value;
}
