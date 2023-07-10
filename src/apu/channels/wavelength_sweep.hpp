#ifndef GROUBOY_WAVELENGTH_SWEEP_HPP
#define GROUBOY_WAVELENGTH_SWEEP_HPP

#include <functional>

/**
 * A wavelength sweep will adjust the wavelength (and thus frequency) of a wave periodically.
 * It's possible to control the period at which the update will take place,
 * the direction of the update (increase of decrease), and the amplitude of the update (shift).
 * If an overflow happens, it will notify the user through a callback.
 */
class WavelengthSweep
{
  public:
    /**
     * A function that will be called when the wavelength has been updated
     */
    typedef std::function<void(int)> WavelengthChangedCallback;

    /**
     * A function that will be called if the wavelength overflowed.
     * A wavelength overflows when the new value exceeds 2048.
     */
    typedef std::function<void()> WavelengthOverflowCallback;

    /**
     * Create a new wavelength sweep.
     */
    explicit WavelengthSweep();

    /**
     * Tick the wavelength sweep.
     * If the period elapsed, the wavelength will be either increased or decreased.
     * The new computed wavelength is the current wavelength ± the wavelength shifted by a certain amount.
     * If the wavelength is changed, it will call the WavelengthChangedCallback.
     * If the computation of the new wavelength overflows, it will call WavelengthOverflowCallback.
     */
    void tick();

    /**
     * Set the period of the sweep.
     * Everytime the number of ticks of the period elapses, the wavelength is adjusted.
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
     * Set the wavelength value that will be adjusted by the sweep.
     * @param wavelength the wavelength starting value
     */
    void setWavelength(int wavelength);

    /**
     * Get the current value of the wavelength
     * @return the wavelength
     */
    int getWavelength() const;

    /**
     * Set the number of times the wavelength will be shifted right during an adjustment.
     * The Greater this value, the smaller the update will be.
     * @param shift a positive value
     */
    void setShift(int shift);

    /**
     * Get the number of times the wavelength will be shifted right during an adjustment.
     * @return a positive value
     */
    int getShift() const;

    /**
     * Set the function that will be called when a new wavelength value has been computed
     * @param cb The function to call
     */
    void setWavelengthChangedCallback(WavelengthChangedCallback cb);

    /**
     * Set the function to will be called when the wavelength computation overflowed
     * @param cb The function to call
     */
    void setWavelengthOverflowCallback(WavelengthOverflowCallback cb);

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
    WavelengthChangedCallback _wavelengthChangedCallback;
    WavelengthOverflowCallback _wavelengthOverflowCallback;
    /**
     * Generate a new wavelength value based on the current one
     * @return The new generated wavelength
     */
    int generateNewWavelength() const;

    /**
     * Check if a computed wavelength value overflowed
     * @param wavelength The value to check
     * @return true if it overflowed, false otherwise
     */
    bool didWavelengthOverflow(int wavelength) const;
    /**
     * Number of ticks elapsed
     */
    int _timer = 0;

    /**
     * The number of ticks after which an update should take place
     */
    int _period = 0;

    /**
     * The current value of the wavelength
     */
    int _wavelength = 0;

    /**
     * The direction of the wavelength update
     */
    int _sign = 1;

    /**
     * The amount of shift to apply to the wavelength
     */
    int _shift = 0;

    /**
     * Whether or not the sweep is enabled
     */
    bool _enabled = false;
};

#endif // GROUBOY_WAVELENGTH_SWEEP_HPP
