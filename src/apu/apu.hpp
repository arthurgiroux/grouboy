#ifndef GROUBOY_APU_HPP
#define GROUBOY_APU_HPP

#include "apu/channels/audio_mixer.hpp"
#include "apu/channels/channel1.hpp"
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
    void addSampleToAudioBuffer();
    static const int CH1_SWEEP_REG_ADDR = 0xFF10;
    static const int CH1_LENGTH_TIMER_AND_DUTY = 0xFF11;
    static const int CH1_VOLUME_CTRL_ADDR = 0xFF12;
    static const int CH1_WAVELENGTH_LOW_REG_ADDR = 0xFF13;
    static const int CH1_WAVELENGTH_AND_CONTROL_REG_ADDR = 0xFF14;

    static const int MASTER_VOLUME_ADDR = 0xFF24;
    static const int SOUND_PANNING_ADDR = 0xFF25;
    static const int SOUND_CTRL_ADDR = 0xFF26;

    Timer* _timer;
    Channel1 _channel1{};
    AudioMixer _mixer;
    int _samplingFrequency;
    int _numberOfCyclesPerAudioSample;
    int _cycleCounter = 0;
    AudioBuffer _audioBuffer = {};
    FallingEdgeDetector _fallingEdgeDetector;
    bool _enabled = false;
};

#endif // GROUBOY_APU_HPP
