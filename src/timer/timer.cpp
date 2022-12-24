#include "timer.hpp"
#include "cpu/cpu.hpp"

const int Timer::DIV_TIMER_CLOCK_DIVIDER = CPU::CLOCK_FREQUENCY_HZ / DIV_REGISTER_FREQUENCY_HZ;

void Timer::updateTimerCounterClockDivider()
{
    int divider = getClockDivider();
    if (timerCounterClockDivider != divider)
    {
        // When there's a change of divider, the internal counting needs to be reset.
        timerCounterCycles = 0;
        timerCounterClockDivider = divider;
    }
}

void Timer::tick(int ticks)
{
    int cycles = ticks * TICKS_TO_CPU_CYCLES;
    dividerRegisterCycles += cycles;
    if (dividerRegisterCycles >= DIV_TIMER_CLOCK_DIVIDER)
    {
        // TODO: Check if we need to reset to 0
        dividerRegisterCycles -= DIV_TIMER_CLOCK_DIVIDER;
        _mmu->memory[DIVIDER_REGISTER_ADDR] = _mmu->read(DIVIDER_REGISTER_ADDR) + 1;
    }

    if (isTimerCounterEnabled())
    {
        updateTimerCounterClockDivider();

        timerCounterCycles += cycles;

        while (timerCounterCycles >= timerCounterClockDivider)
        {
            timerCounterCycles -= timerCounterClockDivider;
            int value = _mmu->read(TIMER_COUNTER_ADDR);
            int maxTimerValue = 0xFF;
            if (value == maxTimerValue)
            {
                _mmu->write(TIMER_COUNTER_ADDR, _mmu->read(TIMER_MODULO_ADDR));
                _interruptManager->raiseInterrupt(InterruptType::TIMER);
            }
            else
            {
                _mmu->write(TIMER_COUNTER_ADDR, value + 1);
            }
        }
    }
}

bool Timer::isTimerCounterEnabled()
{
    return utils::isNthBitSet(_mmu->read(TIMER_CONTROL_ADDR), 2);
}

int Timer::getClockDivider()
{
    int clockSelected = _mmu->read(TIMER_CONTROL_ADDR) & 0b00000011;
    if (clockSelected < CLOCK_DIVIDER_VALUES.size())
    {
        return CLOCK_DIVIDER_VALUES[clockSelected];
    }
    else
    {
        throw std::runtime_error("Unhandled clock select value");
    }
}

int Timer::getDividerRegisterValue() const
{
    return _mmu->read(DIVIDER_REGISTER_ADDR);
}
