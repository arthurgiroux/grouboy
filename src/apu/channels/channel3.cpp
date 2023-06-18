#include "channel3.hpp"

Channel3::Channel3() : Channel(LENGTH_TIMER_DURATION)
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
