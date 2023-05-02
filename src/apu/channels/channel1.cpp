#include "channel1.hpp"

Channel1::Channel1()
{
    using std::placeholders::_1;
    _wavelengthSweep.setWavelengthChangedCallback(std::bind(&Channel1::onWavelengthChanged, this, _1));
    _wavelengthSweep.setWavelengthOverflowCallback(std::bind(&Channel1::onWavelengthOverflow, this));
}

float Channel1::getAudioSample()
{
    return _squareWave.getAmplitude();
}

void Channel1::step(int cycles)
{
    // Sweep -> Timer -> Duty -> Length Counter -> Envelope -> Mixer
    _squareWave.step(cycles);
}

void Channel1::tickCounter()
{
    _tickCounter++;
    if (_tickCounter % 8 == 0)
    {
        // SWEEP
        _sweepCounter--;
        if (_sweepCounter <= 0)
        {
            _wavelengthSweep.tick();
        }
    }
    if (_tickCounter % 4 == 0)
    {
        // Freq sweep
    }

    if (_tickCounter % 2 == 0)
    {
        // Sound length
    }

    _tickCounter %= 8;
}

void Channel1::trigger()
{
    _enable = true;
    triggerSweep();
}

void Channel1::onWavelengthChanged(int wavelength)
{
    setWavelength(wavelength);
}

void Channel1::onWavelengthOverflow()
{
    _enable = false;
}

void Channel1::triggerSweep()
{
    int sweepDirection = _sweepControlValue & 0b00001000;
    int sweepPeriod = _sweepControlValue & 0b01110000;
    int shift = _sweepControlValue & 0b00000111;
    _wavelengthSweep.setDirection(sweepDirection);
    _wavelengthSweep.setPeriod(sweepPeriod);
    _wavelengthSweep.setShift(shift);
    _wavelengthSweep.setWavelength(_wavelength);
}

void Channel1::setWavelength(int wavelength)
{
    _wavelength = wavelength;

    int waveFrequency = (2048 - wavelength) * 4;
    _squareWave.setFrequency(waveFrequency);
}

int Channel1::getWavelength()
{
    return _wavelength;
}

bool Channel1::isEnabled() const
{
    return _enable;
}

void Channel1::setSweepControl(int value)
{
    _sweepControlValue = value;
}

int Channel1::getSweepControl() const
{
    return _sweepControlValue;
}
