#ifndef GROUBOY_APU_HPP
#define GROUBOY_APU_HPP

#include "apu/channels/channel1.hpp"
#include "common/types.hpp"
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
    static const int CH1_WAVELENGTH_LOW_REG_ADDR = 0xFF13;
    static const int CH1_WAVELENGTH_AND_CONTROL_REG_ADDR = 0xFF14;

    Timer* _timer;
    Channel1 _channel1{};
    int _samplingFrequency;
    int _numberOfCyclesPerAudioSample;
    int _cycleCounter = 0;
    AudioBuffer _audioBuffer = {};
    float _time = 0.f;
    int _lastDivValue = 0;
};

#endif // GROUBOY_APU_HPP
