#ifndef GROUBOY_LENGTH_TIMER_HPP
#define GROUBOY_LENGTH_TIMER_HPP

class LengthTimer
{
  public:
    LengthTimer();
    void tick();
    void restartTimer();
    void setStartValue(int startValue);
    int getStartValue() const;
    bool isTimerElapsed() const;

  private:
    static const int DURATION_IN_TICKS = 64;
    int _startValue = 0;
    int _ticksElapsed = 0;
};

#endif // GROUBOY_LENGTH_TIMER_HPP
