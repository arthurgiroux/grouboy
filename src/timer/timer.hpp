#ifndef GBEMULATOR_TIMER_HPP
#define GBEMULATOR_TIMER_HPP

#include "memory/mmu.hpp"

class Timer
{
  public:
    Timer(MMU* mmu) : _mmu(mmu){};
    ~Timer() = default;
    void tick(int cycles);
    bool isTimerEnabled();
    int getClockDivider();
    static const int DIVIDER_REGISTER_ADDR = 0xFF04;

  private:
    static const int TIMER_COUNTER_ADDR = 0xFF05;
    static const int TIMER_MODULO_ADDR = 0xFF06;
    static const int TIMER_CONTROL_ADDR = 0xFF07;

    MMU* _mmu;
    int currentDivValue = 0;
    int currentTimerValue = 0;
};

#endif // GBEMULATOR_TIMER_HPP
