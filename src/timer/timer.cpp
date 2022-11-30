#include "timer.hpp"
#include <iostream>

void Timer::tick(int cycles)
{
    currentDivValue += cycles;
    if (currentDivValue >= 256)
    {
        currentDivValue -= 256;
        _mmu->write(DIVIDER_REGISTER_ADDR, _mmu->read(DIVIDER_REGISTER_ADDR) + 1);
    }

    if (isTimerEnabled())
    {
        currentTimerValue += cycles;

        if (currentTimerValue >= getClockDivider())
        {
            currentTimerValue -= getClockDivider();
            int value = _mmu->read(TIMER_COUNTER_ADDR);
            if (value == 0xFF)
            {
                _mmu->write(TIMER_COUNTER_ADDR, _mmu->read(TIMER_MODULO_ADDR));
                int interruptValue = _mmu->read(0xFF0F);
                utils::setNthBit(interruptValue, 2, true);
                _mmu->write(0xFF0F, interruptValue);
            }
            else
            {
                _mmu->write(TIMER_COUNTER_ADDR, value + 1);
            }
        }
    }
}

bool Timer::isTimerEnabled()
{
    return utils::isNthBitSet(_mmu->read(TIMER_CONTROL_ADDR), 2);
}

int Timer::getClockDivider()
{
    int clockSelected = _mmu->read(TIMER_CONTROL_ADDR) & 0x03;
    switch (clockSelected)
    {
    case 0:
        return 1024;
    case 1:
        return 16;
    case 2:
        return 64;
    case 3:
        return 256;
    default:
        throw std::runtime_error("Unhandled clock select value");
    }
}
