#ifndef GBEMULATOR_TIMER_HPP
#define GBEMULATOR_TIMER_HPP

#include "memory/mmu.hpp"

class MMU;
class InterruptManager;

/**
 * Abstraction class for the GameBoy internal timers.
 * The GameBoy has two main timing mechanism.
 *      The divider register:
 *          A timer running at 16384Hz that doesn't trigger any interrupt.
 *          This timer can't be deactivated but can be reset on demand.
 *
 *      The timer counter:
 *          A programmable timer that can be enabled/disabled.
 *          This timer is counting at a specific frequency and increasing an 8-bit counter.
 *          Once this counter overflows, a "Timer interrupt" is triggered.
 *          After overflowing, the counter will start counting again from the value set in
 *          the "timer modulo", this "timer modulo" allows to lower the frequency even further.
 *
 *          its base frequency can be chosen from the following options:
 *          4096 Hz, 262144 Hz, 65536 Hz, 16384 Hz.
 *
 */
class Timer
{
  public:
    /**
     * Create a new Timer wrapper.
     * Once created, use tick() to update the timer based on elapsed CPU ticks.
     *
     * @param interruptManager   The manager to raise interrupts
     */
    explicit Timer(InterruptManager* interruptManager) : _interruptManager(interruptManager){};
    ~Timer() = default;

    /**
     * Inform the timer that a certain number of CPU ticks has elapsed.
     * This will update the timers according to their internal frequency and parameters.
     *
     * @param ticks     the number of CPU ticks that elapsed
     */
    void tick(int ticks);

    /**
     * Get the current value of the divider register.
     *
     * @return The byte value of the divider register.
     */
    int getDividerRegisterValue() const;

    /**
     * Reset the divider register and associated internal counter to 0.
     */
    void resetDividerRegisterValue();

    /**
     * Get the current value of the timer counter.
     * This value is incremented during by the timer counter logic.
     *
     * @return timer counter value
     */
    int getTimerCounterValue() const;

    /**
     * Set the timer counter to a specific value.
     * @param value A value between 0 and 255
     */
    void setTimerCounterValue(int value);

    /**
     * Get the value that the counter will be set to when overflowing.
     *
     * @return the timer modulo value
     */
    int getTimerModuloValue() const;

    /**
     * Set the value that the counter will be set to when overflowing.
     *
     * @param value The new timer modulo value
     */
    void setTimerModuloValue(int value);

    /**
     * Enable or disable the timer counter.
     */
    void enableTimerCounter(bool state);

    /**
     * Return if the Timer Counter is enabled or not.
     *
     * @return  The state of the timer counter.
     */
    bool isTimerCounterEnabled();

    /**
     * Get the current clock divider that is used by the Timer Counter.
     * This is a divider of the main CPU clocks, which means that every
     * N (where N is the divider) cpu cycles, the timer counter will be increased.
     *
     * @return  How many CPU cycles are needed to increase the timer counter
     */
    int getClockDivider();

    /**
     * Set the clock divider to be used by the timer counter.
     *
     * @param value Possible values are:
     * 0: 16
     * 1: 64
     * 2: 256
     * 3: 1024
     */
    void setClockDivider(unsigned int value);

    /**
     * The frequency in Hz of the divider register timer.
     */
    static const int DIV_REGISTER_FREQUENCY_HZ = 16384;

  private:
    /**
     * Conversion factor from CPU ticks to CPU cycles.
     */
    static const int TICKS_TO_CPU_CYCLES = 4;

    /**
     * The clock divider for the Divider Register.
     */
    static const int DIV_TIMER_CLOCK_DIVIDER;

    /**
     * The different values for the clock divider that can be set in the Timer Control Register.
     */
    // TODO: use constexpr function to compute divider from frequency
    static constexpr std::array<int, 4> CLOCK_DIVIDER_VALUES = {
        // 4096 Hz
        1024,
        // 262144 Hz
        16,
        // 65536 Hz
        64,
        // 16384 Hz
        256};

    /**
     * Increment the Divider Register value based on the number of CPU cycles elapsed.
     * @param cycles    The number of CPU cycles elapsed
     */
    void incrementDivRegister(int cycles);

    /**
     * Increment the Timer Counter value based on the number of CPU cycles elapsed.
     * @param cycles    The number of CPU cycles elapsed
     */
    void incrementTimerCounter(int cycles);

    /**
     * The number of CPU cycles accumulated for the Divider Register since the last increment of the counter.
     */
    int _dividerRegisterCycles = 0;

    /**
     * The number of CPU cycles accumulated for the Timer Counter since the last increment of the counter.
     */
    int _timerCounterCycles = 0;

    /**
     * The value of the clock divider for the Timer Counter.
     */
    int _timerCounterClockDivider = 0;

    /**
     * The value of the Divider Register.
     */
    byte _dividerRegisterValue = 0;

    /**
     * The value of the Timer Counter.
     */
    byte _timerCounter = 0;

    /**
     * The value of the Timer Modulo.
     */
    int _timerModulo = 0;

    /**
     * The state of the Timer Counter.
     */
    bool _timerCounterEnabled = false;

    /**
     * The interrupt manager that will be used to retrieve information about interrupts.
     */
    InterruptManager* _interruptManager = nullptr;
};

#endif // GBEMULATOR_TIMER_HPP
