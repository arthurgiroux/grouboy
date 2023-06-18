#ifndef GROUBOY_WAVE_HPP
#define GROUBOY_WAVE_HPP

#include "common/types.hpp"
#include <array>

template <int SZ>
class Wave
{
  public:
    explicit Wave();
    void reset();
    void step(int cycles);
    int getAmplitude() const;
    void setFrequency(int frequency);
    int getFrequency() const;
    void setSample(int index, int value);
    byte getSample(int index) const;
    void resetFrequencyTimer();

  private:
    void nextSample();
    int _wavePosition = 0;
    int _frequencyTimerValue = 0;
    int _frequency = 0;
    std::array<byte, SZ> _waveSamples = {};
};

#endif // GROUBOY_WAVE_HPP
