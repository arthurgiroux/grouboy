#include "apu.hpp"
#include "cpu/cpu.hpp"
#include <cmath>

APU::APU(MMU* mmu, Timer* timer, int samplingFrequency)
    : _mmu(mmu), _timer(timer), _samplingFrequency(samplingFrequency), _channel1(mmu)
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
