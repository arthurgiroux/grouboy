#ifndef GROUBOY_NOISE_SIGNAL_HPP
#define GROUBOY_NOISE_SIGNAL_HPP

/**
 * Class that generates a white noise signal using an LFSR.
 * The noise softness can be adjusted through the frequency.
 */
class NoiseSignal
{
  public:
    /**
     * Create a new noise signal
     */
    explicit NoiseSignal();

    /**
     * Reset the nois generator to its original state
     */
    void reset();

    /**
     * Step the generator by a certain amount of CPU cycles.
     * This will continue the generation of the noise at the set frequency.
     *
     * @param cycles the number of cycles
     */
    void step(int cycles);

    /**
     * Get the current amplitude of the generated noise.
     * @return a value between [0, 1]
     */
    int getAmplitude() const;

    /**
     * Set the frequency of the generated noise.
     * @param frequency A frequency in CPU cycles.
     */
    void setFrequency(int frequency);

    /**
     * Get the frequency of the generated noise.
     * @return A frequency in CPU cycles.
     */
    int getFrequency() const;

    /**
     * Enable or disable "wide mode".
     * When "wide mode" is enabled, the generated noise signal
     * has different characteristics, notably timbre and texture.
     *
     * @param enabled true to enable wide mode, false otherwise
     */
    void enableWideMode(bool enabled);

    /**
     * Get if wide mode is enabled or not.
     * @return true if it's enabled, false otherwise
     */
    bool isWideModeEnabled() const;

  private:
    /**
     * Compute the next noise audio sample
     */
    void nextSample();

    /**
     * Reset the internal counter of the frequency timer.
     */
    void resetFrequencyTimer();

    /**
     * The internal counter of the frequency timer, when this is equal
     * to the frequency then a new sample is generated.
     */
    int _frequencyTimerValue = 0;

    /**
     * The frequency of the noise channel
     */
    int _frequency = 0;

    /**
     * The current value of the LFSR register.
     */
    int _lfsrValue = 0;

    /**
     * Whether or not wide mode is enabled.
     */
    bool _wideModeEnabled = false;
};

#endif // GROUBOY_NOISE_SIGNAL_HPP
