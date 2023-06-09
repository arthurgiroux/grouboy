#ifndef GROUBOY_LENGTH_TIMER_HPP
#define GROUBOY_LENGTH_TIMER_HPP

class LengthTimer
{
  public:
    LengthTimer(int durationInTicks);
    void tick();
    void restartTimer();
    void setStartValue(int startValue);
    int getStartValue() const;
    bool isTimerElapsed() const;

  private:
    int _startValue = 0;
    int _ticksElapsed = 0;
    int _durationInTicks = 0;
};

#endif // GROUBOY_LENGTH_TIMER_HPP
