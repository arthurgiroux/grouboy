#include "apu.hpp"
#include "cpu/cpu.hpp"

const utils::AddressRange APU::CH1_ADDR_RANGE = utils::AddressRange(0xFF10, 0xFF14);
const utils::AddressRange APU::CH2_ADDR_RANGE = utils::AddressRange(0xFF15, 0xFF19);
const utils::AddressRange APU::CH3_WAVE_PATTERN_ADDR = utils::AddressRange(0xFF30, 0xFF3F);

APU::APU(Timer* timer, int samplingFrequency) : _timer(timer), _samplingFrequency(samplingFrequency)
{
    float subsamplingRatio = CPU::CLOCK_FREQUENCY_HZ / static_cast<float>(samplingFrequency);
    float highpassCoeff = std::pow(HIGHPASS_BASE_COEFF, subsamplingRatio);

    _channel1 = std::make_unique<Channel1>(highpassCoeff);
    _channel2 = std::make_unique<Channel2>(highpassCoeff);
    _channel3 = std::make_unique<Channel3>(highpassCoeff);
    _channel4 = std::make_unique<Channel4>(highpassCoeff);

    _mixer = std::make_unique<AudioMixer>(_channel1.get(), _channel2.get(), _channel3.get(), _channel4.get());
    _numberOfCyclesPerAudioSample = static_cast<int>(subsamplingRatio);
    _channels.push_back(_channel1.get());
    _channels.push_back(_channel2.get());
    _channels.push_back(_channel3.get());
    _channels.push_back(_channel4.get());
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
                channel->tickCounter();
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
        sample = _mixer->getSample();
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
        return 0x80 | _channel1->getSweepControl();
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
        return 0x70 | (_enabled << 7) | (_channel4->isEnabled() << 3) | (_channel3->isEnabled() << 2) |
               (_channel2->isEnabled() << 1) | (_channel1->isEnabled());
    }
    else if (addr == CH3_DAC_REG_ADDR)
    {
        return 0x7F | (_channel3->isDACEnabled() << 7);
    }
    else if (addr == CH3_LENGTH_TIMER_REG_ADDR)
    {
        return 0xFF;
    }
    else if (addr == CH3_OUTPUT_LEVEL_ADDR)
    {
        return 0x9F | (_channel3->getVolumeControl() << 5);
    }
    else if (addr == CH3_WAVELENGTH_LOW_REG_ADDR)
    {
        return 0xFF;
    }
    else if (addr == CH3_WAVELENGTH_AND_CONTROL_REG_ADDR)
    {
        return _channel3->isLengthTimerEnabled() ? 0xFF : 0xBF;
    }
    else if (CH3_WAVE_PATTERN_ADDR.contains(addr))
    {
        int relativeAddr = CH3_WAVE_PATTERN_ADDR.relative(addr);
        int sampleIndex = relativeAddr * 2;
        return (_channel3->getWave().getSample(sampleIndex) << 4) | _channel3->getWave().getSample(sampleIndex + 1);
    }
    else if (addr == CH4_LENGTH_TIMER)
    {
        return 0xFF;
    }
    else if (addr == CH4_VOLUME_CTRL_ADDR)
    {
        return _channel4->getVolumeControl();
    }
    else if (addr == CH4_NOISE_CTRL_ADDR)
    {
        return _channel4->getNoiseControl();
    }
    else if (addr == CH4_CHANNEL_CTRL_ADDR)
    {
        return _channel4->isLengthTimerEnabled() ? 0xFF : 0xBF;
    }
    else if (addr == MASTER_VOLUME_ADDR)
    {
        return (_mixer->isVinLeftEnabled() << 7) | (_mixer->getVolumeScaleLeft() << 4) |
               (_mixer->isVinRightEnabled() << 3) | _mixer->getVolumeScaleRight();
    }
    else if (addr == SOUND_PANNING_ADDR)
    {
        return (_mixer->getPanningControlLeft() << 4) | _mixer->getPanningControlRight();
    }

    // Unmapped register should return FF
    return 0xFF;
}

void APU::writeRegister(const word& addr, const byte& value)
{
    if (_enabled)
    {
        if (addr == CH1_SWEEP_REG_ADDR)
        {
            _channel1->setSweepControl(value);
        }
        else if (addr == CH1_LENGTH_TIMER_AND_DUTY || addr == CH2_LENGTH_TIMER_AND_DUTY)
        {
            ChannelWave& channel = getChannelWaveFromRegAddr(addr);
            channel.getWave().setDutyPattern(value >> 6);
            setLengthTimer(channel, value & 0b00111111);
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
            _mixer->enableVinLeft(utils::isNthBitSet(value, 7));
            _mixer->enableVinRight(utils::isNthBitSet(value, 3));
            _mixer->setVolumeScaleRight(value & 0x7);
            _mixer->setVolumeScaleLeft((value >> 4) & 0x7);
        }
        else if (addr == SOUND_PANNING_ADDR)
        {
            _mixer->setPanningControlRight(value & 0xF);
            _mixer->setPanningControlLeft((value >> 4) & 0xF);
        }
        else if (addr == CH3_DAC_REG_ADDR)
        {
            bool dacValue = utils::isNthBitSet(value, 7);
            _channel3->enableDAC(dacValue);
            // Disabling DAC also disables the channel
            if (!dacValue)
            {
                _channel3->enable(false);
            }
        }
        else if (addr == CH3_LENGTH_TIMER_REG_ADDR)
        {
            setLengthTimer(*_channel3, value);
        }
        else if (addr == CH3_OUTPUT_LEVEL_ADDR)
        {
            _channel3->setVolumeControl((value >> 5) & 0x03);
        }
        else if (addr == CH3_WAVELENGTH_LOW_REG_ADDR)
        {
            int wavelengthHigh = _channel3->getWavelength() & 0xFF00;
            _channel3->setWavelength(wavelengthHigh | value);
        }
        else if (addr == CH3_WAVELENGTH_AND_CONTROL_REG_ADDR)
        {
            _channel3->enableLengthTimer(utils::isNthBitSet(value, 6));

            int wavelengthLow = _channel3->getWavelength() & 0xFF;
            _channel3->setWavelength(((value & 0b00000111) << 8) | wavelengthLow);

            if (utils::isNthBitSet(value, 7) && _channel3->isDACEnabled())
            {
                _channel3->trigger();
            }
        }
        else if (addr == CH4_LENGTH_TIMER)
        {
            setLengthTimer(*_channel4, value & 0b00111111);
        }
        else if (addr == CH4_VOLUME_CTRL_ADDR)
        {
            if ((value & 0xF8) == 0)
            {
                _channel4->enableDAC(false);
                _channel4->enable(false);
            }
            else
            {
                _channel4->enableDAC(true);
            }
            _channel4->setVolumeControl(value);
        }
        else if (addr == CH4_NOISE_CTRL_ADDR)
        {
            _channel4->setNoiseControl(value);
        }
        else if (addr == CH4_CHANNEL_CTRL_ADDR)
        {
            _channel4->enableLengthTimer(utils::isNthBitSet(value, 6));
            if (utils::isNthBitSet(value, 7) && _channel4->isDACEnabled())
            {
                _channel4->trigger();
            }
        }
    }

    if (addr == SOUND_CTRL_ADDR)
    {
        _enabled = (value >> 7);
        if (!_enabled)
        {
            for (auto& channel : _channels)
            {
                channel->enable(false);
                channel->reset();
            }
            _mixer->reset();
        }
    }
    else if (CH3_WAVE_PATTERN_ADDR.contains(addr))
    {
        int relativeAddr = CH3_WAVE_PATTERN_ADDR.relative(addr);
        int sampleIndex = relativeAddr * 2;
        _channel3->getWave().setSample(sampleIndex, (value >> 4) & 0x0F);
        _channel3->getWave().setSample(sampleIndex + 1, value & 0x0F);
    }
}

ChannelWave& APU::getChannelWaveFromRegAddr(word addr)
{
    if (CH1_ADDR_RANGE.contains(addr))
    {
        return *_channel1;
    }

    else if (CH2_ADDR_RANGE.contains(addr))
    {
        return *_channel2;
    }

    throw std::runtime_error("Couldn't infer APU channel from register address");
}

void APU::setLengthTimer(Channel& channel, int duration)
{
    // Quirky behavior, if the value is the max duration, then the timer is set to 0
    if (duration == channel.getLengthTimerDuration())
    {
        duration = 0;
    }
    channel.setLengthTimer(duration);
}
