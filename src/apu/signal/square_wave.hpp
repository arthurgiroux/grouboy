#ifndef GROUBOY_SQUARE_WAVE_HPP
#define GROUBOY_SQUARE_WAVE_HPP

#include "common/types.hpp"
#include <array>
#include <bitset>

/**
 * A class to generate a square wave based on 4x predefined wave pattern.
 * The generated wave will be a digital signal with 0 being the low-value and 1 the high-value.
 * It's possible to control the frequency of the wave, and which pattern to use.
 */
class SquareWave
{
  public:
    /**
     * Create a new square wave generator
     */
    explicit SquareWave();

    /**
     * Reset the generator to its initial state.
     */
    void reset();

    /**
     * Step the generator by a certain amount of CPU cycles.
     * Depending on the selected frequency, this might generate a new wave sample.
     *
     * @param cycles the number of CPU cycles to step.
     */
    void step(int cycles);

    /**
     * Get the amplitude of the
     * @return either 0 or 1
     */
    int getAmplitude() const;

    /**
     * Set the frequency of the wave, in CPU cycles.
     * @param frequency a value in CPU cycles.
     */
    void setFrequency(int frequency);

    /**
     * Get the frequency of the wave
     * @return a value in CPU cycles.
     */
    int getFrequency() const;

    /**
     *
     * Select which wave is used for the generation.
     * There's 4 different options with different duty cycles and waveform:
     *  0: duty-cycle = 12.5%
     *  1: duty-cycle = 25%
     *  2: duty-cycle = 50%
     *  3: duty-cycle = 75%
     *
     * @param index The pattern to select, value between [0, 3]
     */
    void setDutyPattern(byte index);

    /**
     * Get the id of the selected wave pattern
     * @return A value between [0, 3]
     */
    byte getDutyPattern() const;

    /**
     * Reset the internal frequency counter.
     */
    void resetFrequencyTimer();

    /**
     * Set the current index inside the wave pattern.
     * @param index A value between [0, 7]
     */
    void setDutyPosition(int index);

  private:
    /**
     * The different pre-defined wave pattern, they will be used as bitfield.
     */
    static constexpr std::array<int, 4> _wavePatterns = {0b00000001, 0b10000001, 0b10000111, 0b01111110};

    /**
     * Move the signal to the next wave position.
     */
    void nextWaveValue();

    /**
     * Which position of the wave we are currently in
     */
    int _waveDutyPosition = 0;

    /**
     * The selected wave pattern.
     */
    byte _dutyPattern = 0;

    /**
     * The internal frequency counter, when it reaches the selected frequency a new sample is generated.
     */
    int _frequencyTimerValue = 0;

    /**
     * The frequency of the wave, in CPU cycles.
     */
    int _frequency = 0;
};

#endif // GROUBOY_SQUARE_WAVE_HPP
