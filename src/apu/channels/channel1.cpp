#include "channel1.hpp"
#include <spdlog/spdlog.h>

Channel1::Channel1()
{
    using std::placeholders::_1;
    _wavelengthSweep.setWavelengthChangedCallback(std::bind(&Channel1::onWavelengthChanged, this, _1));
    _wavelengthSweep.setWavelengthOverflowCallback(std::bind(&Channel1::onWavelengthOverflow, this));
}

float Channel1::getAudioSample()
{
    float dacIn = static_cast<float>(_squareWave.getAmplitude() * _volumeSweep.getVolume()) / 15.f;
    return (dacIn * 2 - 1);
}

void Channel1::step(int cycles)
{
    // Sweep -> Timer -> Duty -> Length Counter -> Envelope -> Mixer
    _squareWave.step(cycles);
}

void Channel1::tickCounter()
{
    // TODO: Fix comments
    // This will tick with DIV at 512Hz
    _tickCounter++;
    if (_tickCounter % 8 == 0)
    {
        // volume sweep
        _volumeSweep.tick();
    }

    if (_tickCounter % 4 == 0)
    {
        // Freq sweep
        _wavelengthSweep.tick();
    }

    if (_tickCounter % 2 == 0 && _lengthTimerEnabled && _lengthTimer > 0)
    {
        // Sound length
        _lengthTimerValue--;
        if (_lengthTimerValue == 64)
        {
            _enable = false;
        }
    }

    _tickCounter %= 8;
}

void Channel1::trigger()
{
    _enable = true;
    _squareWave.reset();
    triggerSweep();
    _volumeSweep.setPeriod(_volumeCtrl & 0b00000111);
    int volumeDirection = (utils::isNthBitSet(_volumeCtrl, 3) ? 1 : -1);
    _volumeSweep.setDirection(volumeDirection);
    _volumeSweep.setVolume((_volumeCtrl & 0b11110000) >> 4);
    // TODO: Handle turning channel off
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
    int sweepDirection = (_sweepControlValue & 0b00001000) >> 3;
    int sweepPeriod = (_sweepControlValue & 0b01110000) >> 4;
    int shift = _sweepControlValue & 0b00000111;
    _wavelengthSweep.setDirection(sweepDirection);
    _wavelengthSweep.setPeriod(sweepPeriod);
    _wavelengthSweep.setShift(shift);
    _wavelengthSweep.setWavelength(_wavelength);
    _lengthTimerValue = _lengthTimer;
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

SquareWave& Channel1::getWave()
{
    return _squareWave;
}

void Channel1::setLengthTimer(int timer)
{
    _lengthTimer = timer;
}

void Channel1::enableLengthTimer(bool value)
{
    _lengthTimerEnabled = value;
}

bool Channel1::isLengthTimerEnabled() const
{
    return _lengthTimerEnabled;
}

void Channel1::setVolumeControl(int value)
{
    _volumeCtrl = value;
}

int Channel1::getVolumeControl() const
{
    return _volumeCtrl;
}
