#include "timer.hpp"
#include "cpu/cpu.hpp"

const int Timer::DIV_TIMER_CLOCK_DIVIDER = CPU::CLOCK_FREQUENCY_HZ / DIV_REGISTER_FREQUENCY_HZ;

void Timer::tick(int ticks)
{
    int cycles = ticks * TICKS_TO_CPU_CYCLES;
    incrementDivRegister(cycles);

    if (isTimerCounterEnabled())
    {
        incrementTimerCounter(cycles);
    }
}

void Timer::incrementTimerCounter(int cycles)
{
    _timerCounterCycles += cycles;
    int clockDivider = CLOCK_DIVIDER_VALUES[_timerCounterClockDivider];
    while (_timerCounterCycles >= clockDivider)
    {
        _timerCounterCycles -= clockDivider;
        int maxTimerValue = 0xFF;
        if (_timerCounter == maxTimerValue)
        {
            _timerCounter = _timerModulo;
            _interruptManager->raiseInterrupt(InterruptType::TIMER);
        }
        else
        {
            _timerCounter++;
        }
    }
}

void Timer::incrementDivRegister(int cycles)
{
    _dividerRegisterCycles += cycles;
    if (_dividerRegisterCycles >= DIV_TIMER_CLOCK_DIVIDER)
    {
        _dividerRegisterCycles -= DIV_TIMER_CLOCK_DIVIDER;
        _dividerRegisterValue++;
    }
}

bool Timer::isTimerCounterEnabled()
{
    return _timerCounterEnabled;
}

byte Timer::getClockDivider()
{
    return _timerCounterClockDivider;
}

byte Timer::getDividerRegisterValue() const
{
    return _dividerRegisterValue;
}

byte Timer::getTimerCounterValue() const
{
    return _timerCounter;
}

byte Timer::getTimerModuloValue() const
{
    return _timerModulo;
}

void Timer::setTimerModuloValue(byte value)
{
    _timerModulo = value;
}

void Timer::enableTimerCounter(bool enable)
{
    _timerCounterEnabled = enable;
}

void Timer::setClockDivider(byte value)
{
    if (value >= CLOCK_DIVIDER_VALUES.size())
    {
        return;
    }
    else
    {
        _timerCounterClockDivider = value;
        // When there's a change of divider, the internal counting needs to be reset.
        _timerCounterCycles = 0;
    }
}
void Timer::setTimerCounterValue(byte value)
{
    _timerCounter = value;
}

void Timer::resetDividerRegisterValue()
{
    _dividerRegisterValue = 0;
    _timerCounterCycles = 0;
}
