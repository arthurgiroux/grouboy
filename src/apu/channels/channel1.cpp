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
    _enable = false;
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
    int sweepDirection = (Channel1::_sweepControlValue & 0b00001000) >> 3;
    int sweepPeriod = (Channel1::_sweepControlValue & 0b01110000) >> 4;
    int shift = Channel1::_sweepControlValue & 0b00000111;
    Channel1::_wavelengthSweep.setDirection(sweepDirection);
    Channel1::_wavelengthSweep.setPeriod(sweepPeriod);
    Channel1::_wavelengthSweep.setShift(shift);
    Channel1::_wavelengthSweep.setWavelength(getWavelength());
}
