#include "channel4.hpp"
#include "common/utils.hpp"

Channel4::Channel4() : Channel(LENGTH_TIMER_DURATION)
{
    _frameSequencer.addFrame(FrameSequencer::Frame([&] { _volumeSweep.tick(); }, VOLUME_SWEEP_FREQ));
}

void Channel4::step(int cycles)
{
    _noiseSignal.step(cycles);
}

void Channel4::trigger()
{
    _noiseSignal.reset();
    _volumeSweep.setPeriod(_volumeCtrl & 0b00000111);
    int volumeDirection = (utils::isNthBitSet(_volumeCtrl, 3) ? 1 : -1);
    _volumeSweep.setDirection(volumeDirection);
    _volumeSweep.setVolume((_volumeCtrl & 0b11110000) >> 4);
    _lengthTimer.restartTimer();
    enable(true);
}

float Channel4::getAudioSample()
{
    return convertFromDigitalToAnalog(_noiseSignal.getAmplitude() * _volumeSweep.getVolume());
}

void Channel4::setVolumeControl(int value)
{
    _volumeCtrl = value;
}

int Channel4::getVolumeControl() const
{
    return _volumeCtrl;
}

void Channel4::setNoiseControl(int control)
{
    _noiseControl = control;
    int clockShift = (control & 0xF0) >> 4;
    bool wideMode = !utils::isNthBitSet(control, 3);
    int clockDivider = control & 0x07;
    int freqDivider = 0;
    if (clockDivider == 0)
    {
        freqDivider = 8;
    }
    else
    {
        freqDivider = 16 * clockDivider;
    }

    int frequency = freqDivider << clockShift;
    _noiseSignal.enableWideMode(wideMode);
    _noiseSignal.setFrequency(frequency);
}

int Channel4::getNoiseControl() const
{
    return _noiseControl;
}