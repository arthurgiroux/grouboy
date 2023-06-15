#include "channel_wave.hpp"
#include "channel.hpp"
#include "common/utils.hpp"

ChannelWave::ChannelWave() : Channel(LENGTH_TIMER_DURATION)
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

void ChannelWave::setVolumeControl(int value)
{
    _volumeCtrl = value;
}

int ChannelWave::getVolumeControl() const
{
    return _volumeCtrl;
}
