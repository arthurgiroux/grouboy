#include "channel1.hpp"

Channel1::Channel1() : ChannelWave()
{
    using std::placeholders::_1;
    _wavelengthSweep.setWavelengthChangedCallback(std::bind(&Channel1::onWavelengthChanged, this, _1));
    _wavelengthSweep.setWavelengthOverflowCallback(std::bind(&Channel1::onWavelengthOverflow, this));

    _frameSequencer.addFrame(FrameSequencer::Frame([&] { _wavelengthSweep.tick(); }, WAVELENGTH_SWEEP_FREQ));
}

void Channel1::onWavelengthChanged(int wavelength)
{
    setWavelength(wavelength);
}

void Channel1::onWavelengthOverflow()
{
    enable(false);
}

void Channel1::setSweepControl(int value)
{
    _sweepControlValue = value;
}

int Channel1::getSweepControl() const
{
    return _sweepControlValue;
}

void Channel1::triggerImpl()
{
    int sweepDirection = (_sweepControlValue & 0b00001000) >> 3;
    int sweepPeriod = (_sweepControlValue & 0b01110000) >> 4;
    int shift = _sweepControlValue & 0b00000111;
    _wavelengthSweep.setDirection(sweepDirection ? -1 : 1);
    _wavelengthSweep.setPeriod(sweepPeriod);
    _wavelengthSweep.setShift(shift);
    _wavelengthSweep.setWavelength(getWavelength());
    _wavelengthSweep.setEnabled(shift > 0 || sweepPeriod > 0);
    _wavelengthSweep.trigger();
}

void Channel1::reset()
{
    ChannelWave::reset();
    _sweepControlValue = 0;
    _wavelengthSweep.reset();
}
