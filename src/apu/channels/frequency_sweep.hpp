#ifndef GROUBOY_FREQUENCY_SWEEP_HPP
#define GROUBOY_FREQUENCY_SWEEP_HPP

#include <functional>

/**
 * A frequency sweep will adjust the frequency of a wave periodically.
 * It's possible to control the period at which the update will take place,
 * the direction of the update (increase of decrease), and the amplitude of the update (shift).
 * If an overflow happens, it will notify the user through a callback.
 */
class FrequencySweep
{
  public:
    /**
     * A function that will be called when the frequency has been updated
     */
    typedef std::function<void(int)> FrequencyChangedCallback;

    /**
     * A function that will be called if the frequency overflowed.
     * A frequency overflows when the new value exceeds 2048.
     */
    typedef std::function<void()> FrequencyOverflowCallback;

    /**
     * Create a new frequency sweep.
     */
    explicit FrequencySweep();

    /**
     * Tick the frequency sweep.
     * If the period elapsed, the frequency will be either increased or decreased.
     * The new computed frequency is the current frequency ± the frequency shifted by a certain amount.
     * If the frequency is changed, it will call the FrequencyChangedCallback.
     * If the computation of the new frequency overflows, it will call FrequencyOverflowCallback.
     */
    void tick();

    /**
     * Set the period of the sweep.
     * Everytime the number of ticks of the period elapses, the frequency is adjusted.
     * @param period a period in ticks
     */
    void setPeriod(int period);

    /**
     * Get the period
     * @return the period in number of ticks
     */
    int getPeriod() const;

    /**
     * Set the direction of the sweep
     * @param sign 1 for increasing, -1 for decreasing
     */
    void setDirection(int sign);

    /**
     * Get the direction of the sweep.
     * @return 1 for increasing, -1 for decreasing
     */
    int getDirection() const;

    /**
     * Set the frequency value that will be adjusted by the sweep.
     * @param frequency the start frequency
     */
    void setFrequency(int frequency);

    /**
     * Get the current value of the frequency
     * @return the frequency
     */
    int getFrequency() const;

    /**
     * Set the number of times the frequency will be shifted right during an adjustment.
     * The greater this value, the smaller the update will be.
     * @param shift a positive value
     */
    void setShift(int shift);

    /**
     * Get the number of times the frequency will be shifted right during an adjustment.
     * @return a positive value
     */
    int getShift() const;

    /**
     * Set the function that will be called when a new frequency value has been computed
     * @param cb The function to call
     */
    void setFrequencyChangedCallback(FrequencyChangedCallback cb);

    /**
     * Set the function to will be called when the frequency computation overflowed
     * @param cb The function to call
     */
    void setFrequencyOverflowCallback(FrequencyOverflowCallback cb);

    /**
     * Resets the sweep to its default state
     */
    void reset();

    /**
     * Trigger the sweep, should be called by the channel when it's triggered too
     */
    void trigger();

    /**
     * Enable or disable the sweep
     * @param value true to enable, false to disable
     */
    void setEnabled(bool value);

    /**
     * Reset the number of ticks elapsed
     */
    void resetTimer();

  private:
    FrequencyChangedCallback _frequencyChangedCallback;
    FrequencyOverflowCallback _frequencyOverflowCallback;
    /**
     * Generate a new frequency value based on the current one
     * @return The new generated frequency
     */
    int generateNewFrequency() const;

    /**
     * Check if a computed frequency value overflowed
     * @param frequency The value to check
     * @return true if it overflowed, false otherwise
     */
    bool didFrequencyOverflow(int frequency) const;

    /**
     * Number of ticks elapsed
     */
    int _timer = 0;

    /**
     * The number of ticks after which an update should take place
     */
    int _period = 0;

    /**
     * The current value of the frequency
     */
    int _frequency = 0;

    /**
     * The direction of the frequency update
     */
    int _sign = 1;

    /**
     * The amount of shift to apply to the frequency
     */
    int _shift = 0;

    /**
     * Whether or not the sweep is enabled
     */
    bool _enabled = false;
};

#endif // GROUBOY_FREQUENCY_SWEEP_HPP
