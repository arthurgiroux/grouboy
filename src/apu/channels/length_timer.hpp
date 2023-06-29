#ifndef GROUBOY_LENGTH_TIMER_HPP
#define GROUBOY_LENGTH_TIMER_HPP

/**
 * A length timer is a timer of a fixed duration that will increase from a starting value,
 * until it reaches the duration.
 * When it reaches the duration then the timer is considered as elapsed.
 */
class LengthTimer
{
  public:
    /**
     * Create a new timer where the duration in ticks is the maximum duration possible
     * @param durationInTicks Maximum duration of the timer.
     */
    LengthTimer(int durationInTicks);

    /**
     * Tick the timer, this will increase the counter by 1.
     */
    void tick();

    /**
     * Reset the counter to the starting value.
     */
    void restartTimer();

    /**
     * Set the initial value of the timer, the counter will start counting from this value.
     * @param startValue The starting value.
     */
    void setStartValue(int startValue);

    /**
     * Get the starting value of the timer
     * @return a value in ticks between 0 and the max duration
     */
    int getStartValue() const;

    /**
     * Whether or not the counter reached the duration of the timer.
     * @return True if timer is elapsed, false otherwise
     */
    bool isTimerElapsed() const;

    /**
     * Get the duration of the timer
     * @return the duration set in the constructor
     */
    int getDuration() const;

    /**
     * Reset the timer to the initial state.
     */
    void reset();

  private:
    /**
     * The value that the timer will start counting from.
     */
    int _startValue = 0;

    /**
     * The counter of the number of ticks elapsed.
     */
    int _ticksElapsed = 0;

    /**
     * The duration of the timer, when _ticksElapsed reaches this number
     * Then the timer is elapsed.
     */
    int _durationInTicks = 0;
};

#endif // GROUBOY_LENGTH_TIMER_HPP
