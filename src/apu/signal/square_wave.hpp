#ifndef GROUBOY_SQUARE_WAVE_HPP
#define GROUBOY_SQUARE_WAVE_HPP

#include "memory/mmu.hpp"
#include <array>
#include <bitset>

class SquareWave
{
  public:
    explicit SquareWave();
    void step(int cycles);
    int getAmplitude() const;
    void setFrequency(int frequency);
    void setDutyPattern(int pattern);
    int getDutyPattern() const;

  private:
    static constexpr std::array<std::bitset<8>, 4> _wavePatterns = {0b00000001, 0b10000001, 0b10000111, 0b01111110};
    void resetFrequencyTimer();
    void nextWaveValue();
    int _waveDutyPosition = 0;
    int _dutyPattern = 0;
    int _frequencyTimerValue = 0;
    int _frequency = 0;
};

#endif // GROUBOY_SQUARE_WAVE_HPP
