#include "apu.hpp"
#include "cpu/cpu.hpp"

const utils::AddressRange APU::CH1_ADDR_RANGE = utils::AddressRange(0xFF10, 0xFF14);
const utils::AddressRange APU::CH2_ADDR_RANGE = utils::AddressRange(0xFF15, 0xFF19);
const utils::AddressRange APU::CH3_WAVE_PATTERN_ADDR = utils::AddressRange(0xFF30, 0xFF3F);

APU::APU(Timer* timer, int samplingFrequency)
    : _timer(timer), _samplingFrequency(samplingFrequency), _mixer(&_channel1, &_channel2, &_channel3, nullptr)
{
    _numberOfCyclesPerAudioSample = CPU::CLOCK_FREQUENCY_HZ / _samplingFrequency;
    _channels.push_back(&_channel1);
    _channels.push_back(&_channel2);
    _channels.push_back(&_channel3);
}

void APU::step(int cycles)
{
    if (_enabled)
    {
        // TODO: Bit 5 instead of bit 4 in double CG mode
        if (_fallingEdgeDetector.detectFallingEdge(_timer->getDividerRegisterValue() & 0x10))
        {
            for (Channel* channel : _channels)
            {
                if (channel->isEnabled())
                {
                    channel->tickCounter();
                }
            }
        }

        for (Channel* channel : _channels)
        {
            if (channel->isEnabled())
            {
                channel->step(cycles);
            }
        }
    }

    _cycleCounter += cycles;
    while (_cycleCounter >= _numberOfCyclesPerAudioSample)
    {
        _cycleCounter -= _numberOfCyclesPerAudioSample;
        addSampleToAudioBuffer();
    }
}

void APU::addSampleToAudioBuffer()
{
    AudioMixer::Sample sample{0.0f, 0.0f};
    if (_enabled)
    {
        sample = _mixer.getSample();
    }

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
    else if (addr == CH1_LENGTH_TIMER_AND_DUTY || addr == CH2_LENGTH_TIMER_AND_DUTY)
    {
        return 0x3F | (getChannelWaveFromRegAddr(addr).getWave().getDutyPattern() << 6);
    }
    else if (addr == CH1_VOLUME_CTRL_ADDR || addr == CH2_VOLUME_CTRL_ADDR)
    {
        return getChannelWaveFromRegAddr(addr).getVolumeControl();
    }
    else if (addr == CH1_WAVELENGTH_LOW_REG_ADDR || addr == CH2_WAVELENGTH_LOW_REG_ADDR)
    {
        return 0xFF;
    }
    else if (addr == CH1_WAVELENGTH_AND_CONTROL_REG_ADDR || addr == CH2_WAVELENGTH_AND_CONTROL_REG_ADDR)
    {
        return getChannelWaveFromRegAddr(addr).isLengthTimerEnabled() ? 0xFF : 0xBF;
    }
    else if (addr == SOUND_CTRL_ADDR)
    {
        return (_enabled << 7) | (_channel3.isEnabled() << 2) | (_channel2.isEnabled() << 1) | (_channel1.isEnabled());
    }
    else if (addr == CH3_DAC_REG_ADDR)
    {
        return 0x7F | (_channel3.isDACEnabled() << 7);
    }
    else if (addr == CH3_LENGTH_TIMER_REG_ADDR)
    {
        return 0xFF;
    }
    else if (addr == CH3_OUTPUT_LEVEL_ADDR)
    {
        return 0x9F & (_channel3.getVolumeControl() << 5);
    }
    else if (addr == CH3_WAVELENGTH_LOW_REG_ADDR)
    {
        return 0xFF;
    }
    else if (addr == CH3_WAVELENGTH_AND_CONTROL_REG_ADDR)
    {
        return _channel3.isLengthTimerEnabled() ? 0xFF : 0xBF;
    }
    else if (CH3_WAVE_PATTERN_ADDR.contains(addr))
    {
        // TODO: Handle reading from Wave RAM while channel is enabled
        int relativeAddr = CH3_WAVE_PATTERN_ADDR.relative(addr);
        int sampleIndex = relativeAddr / 2;
        int nibbleIndex = relativeAddr % 2;
        return (_channel3.getWave().getSample(sampleIndex) >> (nibbleIndex * 4)) & 0x0F;
    }

    return 0;
}

void APU::writeRegister(const word& addr, const byte& value)
{
    if (_enabled)
    {
        if (addr == CH1_SWEEP_REG_ADDR)
        {
            _channel1.setSweepControl(value);
        }
        else if (addr == CH1_LENGTH_TIMER_AND_DUTY || addr == CH2_LENGTH_TIMER_AND_DUTY)
        {
            ChannelWave& channel = getChannelWaveFromRegAddr(addr);
            channel.getWave().setDutyPattern(value >> 6);
            channel.setLengthTimer(value & 0b00111111);
        }
        else if (addr == CH1_VOLUME_CTRL_ADDR || addr == CH2_VOLUME_CTRL_ADDR)
        {
            ChannelWave& channel = getChannelWaveFromRegAddr(addr);
            if ((value & 0xF8) == 0)
            {
                channel.enableDAC(false);
                channel.enable(false);
            }
            else
            {
                channel.enableDAC(true);
            }
            channel.setVolumeControl(value);
        }
        else if (addr == CH1_WAVELENGTH_LOW_REG_ADDR || addr == CH2_WAVELENGTH_LOW_REG_ADDR)
        {
            ChannelWave& channel = getChannelWaveFromRegAddr(addr);
            int wavelengthHigh = channel.getWavelength() & 0xFF00;
            channel.setWavelength(wavelengthHigh | value);
        }
        else if (addr == CH1_WAVELENGTH_AND_CONTROL_REG_ADDR || addr == CH2_WAVELENGTH_AND_CONTROL_REG_ADDR)
        {
            ChannelWave& channel = getChannelWaveFromRegAddr(addr);
            channel.enableLengthTimer(utils::isNthBitSet(value, 6));

            int wavelengthLow = channel.getWavelength() & 0xFF;
            channel.setWavelength(((value & 0b00000111) << 8) | wavelengthLow);

            if (utils::isNthBitSet(value, 7) && channel.isDACEnabled())
            {
                channel.trigger();
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
        else if (addr == CH3_DAC_REG_ADDR)
        {
            bool value = utils::isNthBitSet(value, 7);
            _channel3.enableDAC(value);
            // Disabling DAC also disables the channel
            if (!value)
            {
                _channel3.enable(false);
            }
        }
        else if (addr == CH3_LENGTH_TIMER_REG_ADDR)
        {
            _channel3.setLengthTimer(value);
        }
        else if (addr == CH3_OUTPUT_LEVEL_ADDR)
        {
            _channel3.setVolumeControl((value >> 5) & 0x03);
        }
        else if (addr == CH3_WAVELENGTH_LOW_REG_ADDR)
        {
            int wavelengthHigh = _channel3.getWavelength() & 0xFF00;
            _channel3.setWavelength(wavelengthHigh | value);
        }
        else if (addr == CH3_WAVELENGTH_AND_CONTROL_REG_ADDR)
        {
            _channel3.enableLengthTimer(utils::isNthBitSet(value, 6));

            int wavelengthLow = _channel3.getWavelength() & 0xFF;
            _channel3.setWavelength(((value & 0b00000111) << 8) | wavelengthLow);

            if (utils::isNthBitSet(value, 7) && _channel3.isDACEnabled())
            {
                _channel3.trigger();
            }
        }
        else if (CH3_WAVE_PATTERN_ADDR.contains(addr))
        {
            int relativeAddr = CH3_WAVE_PATTERN_ADDR.relative(addr);
            int sampleIndex = relativeAddr / 2;
            int nibbleIndex = relativeAddr % 2;
            int currentSample = _channel3.getWave().getSample(sampleIndex);
            _channel3.getWave().setSample(sampleIndex, currentSample & (value << (nibbleIndex * 4)));
        }
    }

    if (addr == SOUND_CTRL_ADDR)
    {
        _enabled = value >> 7;
    }
}

ChannelWave& APU::getChannelWaveFromRegAddr(word addr)
{
    if (CH1_ADDR_RANGE.contains(addr))
    {
        return _channel1;
    }

    else if (CH2_ADDR_RANGE.contains(addr))
    {
        return _channel2;
    }

    throw std::runtime_error("Couldn't infer APU channel from register address");
}
