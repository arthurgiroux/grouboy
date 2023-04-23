#ifndef GROUBOY_APU_HPP
#define GROUBOY_APU_HPP

#include "apu/channels/channel1.hpp"
#include "memory/mmu.hpp"
#include "timer/timer.hpp"

class APU
{
  public:
    using AudioBuffer = std::vector<float>;

    explicit APU(MMU* mmu, Timer* timer, int samplingFrequency);
    void step(int cycles);
    const AudioBuffer& getAudioBuffer();
    void resetAudioBuffer();
    void reset();

  private:
    void addSampleToAudioBuffer();

    MMU* _mmu;
    Timer* _timer;
    Channel1 _channel1;
    int _samplingFrequency;
    int _numberOfCyclesPerAudioSample;
    int _cycleCounter = 0;
    AudioBuffer _audioBuffer = {};
    float _time = 0.f;
    int _lastDivValue = 0;
};

#endif // GROUBOY_APU_HPP
