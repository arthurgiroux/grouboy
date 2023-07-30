#include "channel_wave.hpp"
#include "channel.hpp"
#include "common/utils.hpp"

ChannelWave::ChannelWave(float highpassCoeff) : Channel(LENGTH_TIMER_DURATION, highpassCoeff)
{
    _frameSequencer.addFrame(FrameSequencer::Frame([&] { _volumeSweep.tick(); }, VOLUME_SWEEP_FREQ));
}

float ChannelWave::getAudioSample()
{
    return convertFromDigitalToAnalog(_squareWave.getAmplitude() * _volumeSweep.getVolume());
}

void ChannelWave::step(int cycles)
{
    _squareWave.step(cycles);
}

void ChannelWave::trigger()
{
    enable(true);
    _squareWave.setDutyPosition(0);
    _squareWave.resetFrequencyTimer();
    _volumeSweep.setPeriod(_volumeCtrl & 0b00000111);
    int volumeDirection = (utils::isNthBitSet(_volumeCtrl, 3) ? 1 : -1);
    _volumeSweep.setDirection(volumeDirection);
    _volumeSweep.setVolume((_volumeCtrl & 0b11110000) >> 4);
    if (_lengthTimer.isTimerElapsed())
    {
        _lengthTimer.setStartValue(0);
    }
    triggerImpl();
}

void ChannelWave::triggerImpl()
{
}

void ChannelWave::setFrequency(int frequency)
{
    _frequency = frequency;

    int waveFrequency = (2048 - frequency) * 4;
    _squareWave.setFrequency(waveFrequency);
}

int ChannelWave::getFrequency()
{
    return _frequency;
}

SquareWave& ChannelWave::getWave()
{
    return _squareWave;
}

void ChannelWave::setVolumeControl(int value)
{
    _volumeCtrl = value;
}

int ChannelWave::getVolumeControl() const
{
    return _volumeCtrl;
}

void ChannelWave::reset()
{
    Channel::reset();
    _frequency = 0;
    _volumeCtrl = 0;
    _volumeSweep.reset();
    _squareWave.reset();
}
