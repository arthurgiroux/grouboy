#ifndef GBEMULATOR_INTERRUPT_HANDLER_HPP
#define GBEMULATOR_INTERRUPT_HANDLER_HPP

#include "../common/types.hpp"

class CPU;
class MMU;

class InterruptHandler
{
  public:
    InterruptHandler(CPU* cpu, MMU* mmu) : cpu(cpu), mmu(mmu){};
    virtual ~InterruptHandler() = default;
    virtual bool handle();

  protected:
    word interruptRoutineAddr;
    int interruptFlagBit;

  private:
    CPU* cpu;
    MMU* mmu;

    static const int INTERRUPT_ENABLE_ADDR = 0xFFFF;
    static const int INTERRUPT_FLAG_ADDR = 0xFF0F;
};

class InterruptHandlerVBlank : public InterruptHandler
{
  public:
    InterruptHandlerVBlank(CPU* cpu, MMU* mmu) : InterruptHandler(cpu, mmu)
    {
        interruptRoutineAddr = 0x40;
        interruptFlagBit = 0;
    }
};

class InterruptHandlerLCDStat : public InterruptHandler
{
  public:
    InterruptHandlerLCDStat(CPU* cpu, MMU* mmu) : InterruptHandler(cpu, mmu)
    {
        interruptRoutineAddr = 0x48;
        interruptFlagBit = 1;
    }
};

class InterruptHandlerTimer : public InterruptHandler
{
  public:
    InterruptHandlerTimer(CPU* cpu, MMU* mmu) : InterruptHandler(cpu, mmu)
    {
        interruptRoutineAddr = 0x50;
        interruptFlagBit = 2;
    }
};

class InterruptHandlerSerial : public InterruptHandler
{
  public:
    InterruptHandlerSerial(CPU* cpu, MMU* mmu) : InterruptHandler(cpu, mmu)
    {
        interruptRoutineAddr = 0x58;
        interruptFlagBit = 3;
    }
};

class InterruptHandlerJoypad : public InterruptHandler
{
  public:
    InterruptHandlerJoypad(CPU* cpu, MMU* mmu) : InterruptHandler(cpu, mmu)
    {
        interruptRoutineAddr = 0x60;
        interruptFlagBit = 4;
    }
};
#endif // GBEMULATOR_INTERRUPT_HANDLER_HPP
