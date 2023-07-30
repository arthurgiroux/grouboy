#include "channel1.hpp"

Channel1::Channel1(float highpassCoeff) : ChannelWave(highpassCoeff)
{
    using std::placeholders::_1;
    _frequencySweep.setFrequencyChangedCallback(std::bind(&Channel1::onFrequencyChanged, this, _1));
    _frequencySweep.setFrequencyOverflowCallback(std::bind(&Channel1::onFrequencyOverflow, this));

    _frameSequencer.addFrame(FrameSequencer::Frame([&] { _frequencySweep.tick(); }, FREQUENCY_SWEEP_FREQ));
}

void Channel1::onFrequencyChanged(int frequency)
{
    setFrequency(frequency);
}

void Channel1::onFrequencyOverflow()
{
    enable(false);
}

void Channel1::setSweepControl(int value)
{
    _sweepControlValue = value;
    int sweepDirection = (_sweepControlValue & 0b00001000) >> 3;
    int sweepPeriod = (_sweepControlValue & 0b01110000) >> 4;
    int shift = _sweepControlValue & 0b00000111;
    _frequencySweep.setDirection(sweepDirection ? -1 : 1);
    _frequencySweep.setPeriod(sweepPeriod);
    _frequencySweep.setShift(shift);
}

int Channel1::getSweepControl() const
{
    return _sweepControlValue;
}

void Channel1::triggerImpl()
{
    _frequencySweep.setFrequency(getFrequency());
    _frequencySweep.setEnabled(_frequencySweep.getShift() > 0 || _frequencySweep.getPeriod() > 0);
    _frequencySweep.trigger();
}

void Channel1::reset()
{
    ChannelWave::reset();
    _sweepControlValue = 0;
    _frequencySweep.reset();
}
