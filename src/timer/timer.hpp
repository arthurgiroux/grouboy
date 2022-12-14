#ifndef GBEMULATOR_TIMER_HPP
#define GBEMULATOR_TIMER_HPP

#include "memory/mmu.hpp"

class MMU;

/**
 * Abstraction class for the GameBoy internal timers.
 * The GameBoy has two main timing mechanism.
 *      The divider register:
 *          A timer running at 16384Hz that doesn't trigger any interrupt.
 *          This timer can't be deactivated but can be reset on demand.
 *
 *      The timer counter:
 *          A programmable timer that can enabled/disabled.
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
     * @param mmu   The MMU to use.
     */
    explicit Timer(MMU* mmu) : _mmu(mmu){};
    ~Timer() = default;

    /**
     * Inform the timer that a certain number of CPU ticks elapsed.
     * This will update the timers according to their internal frequency and parameters.
     *
     * @param ticks     the number of CPU ticks that elapsed
     */
    void tick(int ticks);

    /**
     * The address where the value of the Divider Register is stored.
     */
    static const int DIVIDER_REGISTER_ADDR = 0xFF04;

  private:
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
     * Update the stored value of the clock divider based on the selected clock divider
     * in the Timer Control register.
     */
    void updateTimerCounterClockDivider();

    /**
     * The address where the value of the Timer Counter is stored.
     */
    static const int TIMER_COUNTER_ADDR = 0xFF05;

    /**
     * The address where the value of the Timer Modulo is stored.
     */
    static const int TIMER_MODULO_ADDR = 0xFF06;

    /**
     * The address where the parameters of the Timer Counter are stored.
     */
    static const int TIMER_CONTROL_ADDR = 0xFF07;

    /**
     * Conversion factor from CPU ticks to CPU cycles.
     */
    static const int TICKS_TO_CPU_CYCLES = 4;

    /**
     * The clock divider for the Divider Register.
     */
    static const int DIV_TIMER_CLOCK_DIVIDER = 256;

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
     * The number of CPU cycles accumulated for the Divider Register since the last increment of the counter.
     */
    int dividerRegisterCycles = 0;

    /**
     * The number of CPU cycles accumulated for the Timer Counter since the last increment of the counter.
     */
    int timerCounterCycles = 0;

    /**
     * The value of the clock divider for the Timer Counter.
     */
    int timerCounterClockDivider = 0;

    /**
     * The MMU to use for accessing the register and incrementing counters.
     */
    MMU* _mmu = nullptr;
};

#endif // GBEMULATOR_TIMER_HPP
