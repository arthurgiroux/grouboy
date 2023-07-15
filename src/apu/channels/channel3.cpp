#include "channel3.hpp"

Channel3::Channel3(float highpassCoeff) : Channel(LENGTH_TIMER_DURATION, highpassCoeff)
{
}

float Channel3::getAudioSample()
{
    return convertFromDigitalToAnalog(adjustVolume(_wave.getAmplitude()));
}

void Channel3::step(int cycles)
{
    _wave.step(cycles);
}

void Channel3::trigger()
{
    enable(true);
    if (_lengthTimer.isTimerElapsed())
    {
        _lengthTimer.setStartValue(0);
    }
    _wave.reset();
    // Quirk: When triggering, the initial wave position is 1 and not 0.
    // Source: https://github.com/LIJI32/SameSuite/blob/master/apu/channel_3/channel_3_first_sample.asm
    _wave.setPosition(1);
}

void Channel3::setWavelength(int wavelength)
{
    _wavelength = wavelength;

    int waveFrequency = (2048 - wavelength) * 2;
    _wave.setFrequency(waveFrequency);
}

int Channel3::getWavelength()
{
    return _wavelength;
}

Wave<32>& Channel3::getWave()
{
    return _wave;
}

void Channel3::setVolumeControl(int value)
{
    _volumeCtrl = value;
}

int Channel3::getVolumeControl() const
{
    return _volumeCtrl;
}

int Channel3::adjustVolume(int value)
{
    switch (_volumeCtrl)
    {
    case 0:
        return 0;
    case 1:
        return value;
    case 2:
        return value >> 1;
    case 3:
        return value >> 2;
    default:
        throw std::runtime_error("Unhandled volume adjustement value");
    }
}

void Channel3::reset()
{
    Channel::reset();
    _wave.reset();
    _wavelength = 0;
    _lengthTimerEnabled = false;
    _volumeCtrl = 0;
}