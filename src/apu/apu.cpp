#include "apu.hpp"
#include "cpu/cpu.hpp"

APU::APU(Timer* timer, int samplingFrequency) : _timer(timer), _samplingFrequency(samplingFrequency)
{
    _numberOfCyclesPerAudioSample = CPU::CLOCK_FREQUENCY_HZ / _samplingFrequency;
    _lastDivValue = _timer->getDividerRegisterValue();
}

void APU::step(int cycles)
{
    // TODO: Bit 5 instead of bit 4 in double CG mode
    // We detect a falling edge on the DIV register
    if (utils::isNthBitSet(_lastDivValue, 4) && !utils::isNthBitSet(_timer->getDividerRegisterValue(), 4))
    {
        // APU
        _channel1.tickCounter();
    }

    _channel1.step(cycles);

    _lastDivValue = _timer->getDividerRegisterValue();

    _cycleCounter += cycles;
    while (_cycleCounter >= _numberOfCyclesPerAudioSample)
    {
        _cycleCounter -= _numberOfCyclesPerAudioSample;
        addSampleToAudioBuffer();
    }
}

void APU::addSampleToAudioBuffer()
{
    float value = 0;
    if (_channel1.isEnabled())
    {
        value += _channel1.getAudioSample();
    }

    _audioBuffer.push_back(value);
}

const APU::AudioBuffer& APU::getAudioBuffer()
{
    return _audioBuffer;
}

void APU::resetAudioBuffer()
{
    _audioBuffer.clear();
}

void APU::reset()
{
    _cycleCounter = 0;
    _audioBuffer.clear();
}

byte APU::readRegister(const word& addr)
{
    if (addr == CH1_SWEEP_REG_ADDR)
    {
        return _channel1.getSweepControl();
    }

    return 0;
}

void APU::writeRegister(const word& addr, const byte& value)
{
    if (addr == CH1_SWEEP_REG_ADDR)
    {
        _channel1.setSweepControl(value);
    }
    else if (addr == CH1_WAVELENGTH_AND_CONTROL_REG_ADDR)
    {
        if ((value & 0b10000000) > 0)
        {
            _channel1.trigger();
        }
    }
}
