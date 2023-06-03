#include "apu.hpp"
#include "cpu/cpu.hpp"

APU::APU(Timer* timer, int samplingFrequency)
    : _timer(timer), _samplingFrequency(samplingFrequency), _mixer(&_channel1, nullptr, nullptr, nullptr)
{
    _numberOfCyclesPerAudioSample = CPU::CLOCK_FREQUENCY_HZ / _samplingFrequency;
}

void APU::step(int cycles)
{
    // TODO: Bit 5 instead of bit 4 in double CG mode
    if (_fallingEdgeDetector.detectFallingEdge(_timer->getDividerRegisterValue() & 0x10))
    {
        _channel1.tickCounter();
    }

    _channel1.step(cycles);

    _cycleCounter += cycles;
    while (_cycleCounter >= _numberOfCyclesPerAudioSample)
    {
        _cycleCounter -= _numberOfCyclesPerAudioSample;
        addSampleToAudioBuffer();
    }
}

void APU::addSampleToAudioBuffer()
{
    auto sample = _mixer.getSample();
    _audioBuffer.push_back(sample.left);
    _audioBuffer.push_back(sample.right);
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
    else if (addr == CH1_LENGTH_TIMER_AND_DUTY)
    {
        return 0x3F | _channel1.getWave().getDutyPattern() << 6;
    }
    else if (addr == CH1_VOLUME_CTRL_ADDR)
    {
        return _channel1.getVolumeControl();
    }
    else if (addr == CH1_WAVELENGTH_LOW_REG_ADDR)
    {
        return 0xFF;
    }
    else if (addr == CH1_WAVELENGTH_AND_CONTROL_REG_ADDR)
    {
        return _channel1.isLengthTimerEnabled() ? 0xFF : 0xBF;
    }
    else if (addr == SOUND_CTRL_ADDR)
    {
        return (_enabled << 7) | (_channel1.isEnabled());
    }

    return 0;
}

void APU::writeRegister(const word& addr, const byte& value)
{
    if (addr == CH1_SWEEP_REG_ADDR)
    {
        _channel1.setSweepControl(value);
    }
    else if (addr == CH1_LENGTH_TIMER_AND_DUTY)
    {
        _channel1.getWave().setDutyPattern(value >> 6);
        _channel1.setLengthTimer(value & 0b00111111);
    }
    else if (addr == CH1_VOLUME_CTRL_ADDR)
    {
        _channel1.setVolumeControl(value);
    }
    else if (addr == CH1_WAVELENGTH_LOW_REG_ADDR)
    {
        int wavelengthHigh = _channel1.getWavelength() & 0xFF00;
        _channel1.setWavelength(wavelengthHigh | value);
    }
    else if (addr == CH1_WAVELENGTH_AND_CONTROL_REG_ADDR)
    {
        _channel1.enableLengthTimer(utils::isNthBitSet(value, 6));

        int wavelengthLow = _channel1.getWavelength() & 0xFF;
        _channel1.setWavelength(((value & 0b00000111) << 8) | wavelengthLow);

        if (utils::isNthBitSet(value, 7))
        {
            _channel1.trigger();
        }
    }
    else if (addr == MASTER_VOLUME_ADDR)
    {
        _mixer.setVolumeScaleRight(value & 0x7);
        _mixer.setVolumeScaleLeft((value >> 4) & 0x7);
    }
    else if (addr == SOUND_PANNING_ADDR)
    {
        _mixer.setPanningControlRight(value & 0xF);
        _mixer.setPanningControlLeft((value >> 4) & 0xF);
    }
    else if (addr == SOUND_CTRL_ADDR)
    {
        _enabled = value >> 7;
    }
}
