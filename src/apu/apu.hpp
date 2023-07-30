#ifndef GROUBOY_APU_HPP
#define GROUBOY_APU_HPP

#include "apu/channels/audio_mixer.hpp"
#include "apu/channels/channel1.hpp"
#include "apu/channels/channel2.hpp"
#include "apu/channels/channel3.hpp"
#include "apu/channels/channel4.hpp"
#include "common/types.hpp"
#include "signal/falling_edge_detector.hpp"
#include "timer/timer.hpp"

class APU
{
  public:
    using AudioBuffer = std::vector<float>;

    explicit APU(Timer* timer, int samplingFrequency);
    void step(int cycles);
    const AudioBuffer& getAudioBuffer();
    void resetAudioBuffer();
    void reset();
    byte readRegister(const word& addr);
    void writeRegister(const word& addr, const byte& value);

  private:
    static const utils::AddressRange CH1_ADDR_RANGE;
    static const utils::AddressRange CH2_ADDR_RANGE;

    static const int CH1_SWEEP_REG_ADDR = 0xFF10;
    static const int CH1_LENGTH_TIMER_AND_DUTY = 0xFF11;
    static const int CH1_VOLUME_CTRL_ADDR = 0xFF12;
    static const int CH1_FREQUENCY_LOW_REG_ADDR = 0xFF13;
    static const int CH1_FREQUENCY_AND_CONTROL_REG_ADDR = 0xFF14;

    static const int CH2_LENGTH_TIMER_AND_DUTY = 0xFF16;
    static const int CH2_VOLUME_CTRL_ADDR = 0xFF17;
    static const int CH2_FREQUENCY_LOW_REG_ADDR = 0xFF18;
    static const int CH2_FREQUENCY_AND_CONTROL_REG_ADDR = 0xFF19;

    static const int CH3_DAC_REG_ADDR = 0xFF1A;
    static const int CH3_LENGTH_TIMER_REG_ADDR = 0xFF1B;
    static const int CH3_OUTPUT_LEVEL_ADDR = 0xFF1C;
    static const int CH3_FREQUENCY_LOW_REG_ADDR = 0xFF1D;
    static const int CH3_FREQUENCY_AND_CONTROL_REG_ADDR = 0xFF1E;
    static const utils::AddressRange CH3_WAVE_PATTERN_ADDR;

    static const int CH4_LENGTH_TIMER = 0xFF20;
    static const int CH4_VOLUME_CTRL_ADDR = 0xFF21;
    static const int CH4_NOISE_CTRL_ADDR = 0xFF22;
    static const int CH4_CHANNEL_CTRL_ADDR = 0xFF23;

    static const int MASTER_VOLUME_ADDR = 0xFF24;
    static const int SOUND_PANNING_ADDR = 0xFF25;
    static const int SOUND_CTRL_ADDR = 0xFF26;

    /**
     * High-pass filter coefficient that is close to real hardware behavior.
     * See https://gbdev.gg8.se/wiki/articles/Gameboy_sound_hardware#Obscure_Behavior
     */
    static constexpr float HIGHPASS_BASE_COEFF = 0.999958f;

    void addSampleToAudioBuffer();
    ChannelWave& getChannelWaveFromRegAddr(word addr);
    void setLengthTimer(Channel& channel, int duration);

    Timer* _timer;
    std::unique_ptr<Channel1> _channel1;
    std::unique_ptr<Channel2> _channel2;
    std::unique_ptr<Channel3> _channel3;
    std::unique_ptr<Channel4> _channel4;
    std::vector<Channel*> _channels = {};
    std::unique_ptr<AudioMixer> _mixer;
    int _samplingFrequency;
    int _numberOfCyclesPerAudioSample;
    int _cycleCounter = 0;
    AudioBuffer _audioBuffer = {0};
    FallingEdgeDetector _fallingEdgeDetector;
    bool _enabled = false;
};

#endif // GROUBOY_APU_HPP
