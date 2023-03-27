#include "apu.hpp"
#include "cpu/cpu.hpp"
#include <cmath>

APU::APU(MMU* mmu, int samplingFrequency) : _mmu(mmu), _samplingFrequency(samplingFrequency)
{
    _numberOfCyclesPerAudioSample = CPU::CLOCK_FREQUENCY_HZ / _samplingFrequency;
}

void APU::step(int cycles)
{
    _cycleCounter += cycles;
    while (_cycleCounter >= _numberOfCyclesPerAudioSample)
    {
        _cycleCounter -= _numberOfCyclesPerAudioSample;
        addSampleToAudioBuffer();
    }
}

void APU::addSampleToAudioBuffer()
{
    // TODO: Replace with real implementation
    _time += 0.01f;
    auto value = sinf(_time);
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
