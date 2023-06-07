#include "channel_wave.hpp"
#include "channel.hpp"

ChannelWave::ChannelWave() : Channel()
{
    _frameSequencer.addFrame(FrameSequencer::Frame([&] { _volumeSweep.tick(); }, VOLUME_SWEEP_FREQ));
    _frameSequencer.addFrame(FrameSequencer::Frame([&] { tickLengthTimer(); }, LENGTH_TIMER_FREQ));
}

float ChannelWave::getAudioSample()
{
    float dacIn = static_cast<float>(_squareWave.getAmplitude() * _volumeSweep.getVolume()) / 15.f;
    return (dacIn * 2 - 1);
}

void ChannelWave::step(int cycles)
{
    _squareWave.step(cycles);
}

void ChannelWave::trigger()
{
    _enable = true;
    _squareWave.reset();
    _volumeSweep.setPeriod(_volumeCtrl & 0b00000111);
    int volumeDirection = (utils::isNthBitSet(_volumeCtrl, 3) ? 1 : -1);
    _volumeSweep.setDirection(volumeDirection);
    _volumeSweep.setVolume((_volumeCtrl & 0b11110000) >> 4);
    _lengthTimer.restartTimer();
    triggerImpl();
}

void ChannelWave::triggerImpl()
{
}

void ChannelWave::setWavelength(int wavelength)
{
    _wavelength = wavelength;

    int waveFrequency = (2048 - wavelength) * 4;
    _squareWave.setFrequency(waveFrequency);
}

int ChannelWave::getWavelength()
{
    return _wavelength;
}

SquareWave& ChannelWave::getWave()
{
    return _squareWave;
}

void ChannelWave::setLengthTimer(int timer)
{
    _lengthTimer.setStartValue(timer);
}

void ChannelWave::enableLengthTimer(bool value)
{
    _lengthTimerEnabled = value;
}

bool ChannelWave::isLengthTimerEnabled() const
{
    return _lengthTimerEnabled;
}

void ChannelWave::setVolumeControl(int value)
{
    _volumeCtrl = value;
}

int ChannelWave::getVolumeControl() const
{
    return _volumeCtrl;
}

void ChannelWave::tickLengthTimer()
{
    if (_lengthTimerEnabled && !_enable)
    {
        _lengthTimer.tick();
        if (_lengthTimer.isTimerElapsed())
        {
            _enable = false;
        }
    }
}
