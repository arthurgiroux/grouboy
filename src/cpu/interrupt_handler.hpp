#ifndef GBEMULATOR_INTERRUPT_HANDLER_HPP
#define GBEMULATOR_INTERRUPT_HANDLER_HPP

#include "common/types.hpp"
#include "cpu/interrupt_manager.hpp"

// Forward declaration
class CPU;

/**
 * Class responsible for handling a specific interrupt type.
 * If the interrupt is enabled and pending, it will call the interrupt routine and clear it.
 */
class InterruptHandler
{
  public:
    InterruptHandler(CPU* cpu, InterruptManager* interruptManager) : cpu(cpu), _interruptManager(interruptManager){};
    virtual ~InterruptHandler() = default;

    /**
     * Handle the interrupt, this will check if the interrupt is enabled and pending.
     * If it is, it will call the interrupt routine and clear the interrupt.
     *
     * @return true if the interrupt was treated, false otherwise
     */
    virtual bool handle();

  protected:
    /**
     * The memory address of the interrupt routine to call.
     */
    word interruptRoutineAddr;

    /**
     * The type of the interrupt to treat.
     */
    InterruptType _interruptType;

  private:
    /**
     * Pointer towards the CPU, to call the interrupt routine.
     */
    CPU* cpu;

    /**
     * Pointer towards the interrupt manager, to retrieve information about interrupts.
     */
    InterruptManager* _interruptManager;
};

class InterruptHandlerVBlank : public InterruptHandler
{
  public:
    InterruptHandlerVBlank(CPU* cpu, InterruptManager* interruptManager) : InterruptHandler(cpu, interruptManager)
    {
        interruptRoutineAddr = 0x40;
        _interruptType = InterruptType::VBLANK;
    }
};

class InterruptHandlerLCDStat : public InterruptHandler
{
  public:
    InterruptHandlerLCDStat(CPU* cpu, InterruptManager* interruptManager) : InterruptHandler(cpu, interruptManager)
    {
        interruptRoutineAddr = 0x48;
        _interruptType = InterruptType::LCD_STAT;
    }
};

class InterruptHandlerTimer : public InterruptHandler
{
  public:
    InterruptHandlerTimer(CPU* cpu, InterruptManager* interruptManager) : InterruptHandler(cpu, interruptManager)
    {
        interruptRoutineAddr = 0x50;
        _interruptType = InterruptType::TIMER;
    }
};

class InterruptHandlerSerial : public InterruptHandler
{
  public:
    InterruptHandlerSerial(CPU* cpu, InterruptManager* interruptManager) : InterruptHandler(cpu, interruptManager)
    {
        interruptRoutineAddr = 0x58;
        _interruptType = InterruptType::SERIAL;
    }
};

class InterruptHandlerJoypad : public InterruptHandler
{
  public:
    InterruptHandlerJoypad(CPU* cpu, InterruptManager* interruptManager) : InterruptHandler(cpu, interruptManager)
    {
        interruptRoutineAddr = 0x60;
        _interruptType = InterruptType::JOYPAD;
    }
};
#endif // GBEMULATOR_INTERRUPT_HANDLER_HPP
