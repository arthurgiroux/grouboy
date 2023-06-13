#include "channel3.hpp"

Channel3::Channel3()
{
    _frameSequencer.addFrame(FrameSequencer::Frame([&] { tickLengthTimer(); }, LENGTH_TIMER_FREQ));
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
    _wave.reset();
    _lengthTimer.restartTimer();
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

void Channel3::setLengthTimer(int timer)
{
    _lengthTimer.setStartValue(timer);
}

void Channel3::enableLengthTimer(bool value)
{
    _lengthTimerEnabled = value;
}

bool Channel3::isLengthTimerEnabled() const
{
    return _lengthTimerEnabled;
}

void Channel3::setVolumeControl(int value)
{
    _volumeCtrl = value;
}

int Channel3::getVolumeControl() const
{
    return _volumeCtrl;
}

void Channel3::tickLengthTimer()
{
    if (_lengthTimerEnabled && !_enable)
    {
        _lengthTimer.tick();
        if (_lengthTimer.isTimerElapsed())
        {
            enable(false);
        }
    }
}

int Channel3::adjustVolume(int value)
{
    switch (value)
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
