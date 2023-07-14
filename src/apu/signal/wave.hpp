#ifndef GROUBOY_WAVE_HPP
#define GROUBOY_WAVE_HPP

#include "common/types.hpp"
#include <array>

/**
 * A wave of a predefined length that can be reconfigured on the fly.
 * Each sample of the wave can be set to a certain value, and the frequency can be chosen.
 *
 * @tparam SZ The size of the wave in samples.
 */
template <int SZ>
class Wave
{
  public:
    /**
     * Create a new wave
     */
    explicit Wave();

    /**
     * Reset the wave to its initial state
     */
    void reset();

    /**
     * Step the wave by a certain amount of CPU cycles, it might generate a new wave sample
     * if the frequency is reached.
     * @param cycles Number of CPU cycles to step
     */
    void step(int cycles);

    /**
     * Get the amplitude of the current wave sample
     * @return The amplitude of the current sample
     */
    int getAmplitude() const;

    /**
     * Set the frequency of the wave
     * @param frequency A value in CPU cycles
     */
    void setFrequency(int frequency);

    /**
     * Get the frequency of the wave
     * @return A value in CPU cycles
     */
    int getFrequency() const;

    /**
     * Set the value of a sample in the wave
     *
     * @param index A value between 0 and the size of the wave
     * @param value The new value of the sample
     */
    void setSample(int index, byte value);

    /**
     * Get the value of a specific sample in the wave
     * @param index A value between 0 and the size of the wave
     * @return The value of the sample
     */
    byte getSample(int index) const;

    /**
     * Reset the internal frequency counter
     */
    void resetFrequencyTimer();

    /**
     * Set the current position of the wave
     * @param index a value between 0 and the size of the wave
     */
    void setPosition(int index);

  private:
    /**
     * Move the wave to the next sample, this will loop back when reaching the end
     */
    void nextSample();

    /**
     * The index of the current sample in the wave
     */
    int _wavePosition = 0;

    /**
     * The internal frequency counter, when it reaches 0 the wave will move forward by 1 sample
     */
    int _frequencyTimerValue = 0;

    /**
     * The frequency of the wave, in CPU cycles
     */
    int _frequency = 0;

    /**
     * The value of the wave
     */
    std::array<byte, SZ> _waveSamples = {};
};

#endif // GROUBOY_WAVE_HPP
