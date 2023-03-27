#ifndef GROUBOY_APU_HPP
#define GROUBOY_APU_HPP

#include "memory/mmu.hpp"

class APU
{
  public:
    using AudioBuffer = std::vector<float>;

    explicit APU(MMU* mmu, int samplingFrequency);
    void step(int cycles);
    const AudioBuffer& getAudioBuffer();
    void resetAudioBuffer();
    void reset();

  private:
    void addSampleToAudioBuffer();

    MMU* _mmu;
    int _samplingFrequency;
    int _numberOfCyclesPerAudioSample;
    int _cycleCounter = 0;
    AudioBuffer _audioBuffer = {};
    float _time = 0.f;
};

#endif // GROUBOY_APU_HPP
