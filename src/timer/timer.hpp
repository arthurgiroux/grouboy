#ifndef GBEMULATOR_TIMER_HPP
#define GBEMULATOR_TIMER_HPP

#include "memory/mmu.hpp"

class MMU;

class Timer
{
  public:
    Timer(MMU* mmu) : _mmu(mmu){};
    ~Timer() = default;
    void tick(int ticks);
    bool isTimerEnabled();
    int getClockDivider();
    static const int DIVIDER_REGISTER_ADDR = 0xFF04;

  private:
    static const int TIMER_COUNTER_ADDR = 0xFF05;
    static const int TIMER_MODULO_ADDR = 0xFF06;
    static const int TIMER_CONTROL_ADDR = 0xFF07;
    static const int TICKS_TO_CPU_CYCLES = 4;
    static const int DIV_TIMER_CLOCK_DIVIDER = 256;

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

    MMU* _mmu;
    int currentDivValue = 0;
    int currentTimerValue = 0;
    int currentClockDivider = 0;
};

#endif // GBEMULATOR_TIMER_HPP
