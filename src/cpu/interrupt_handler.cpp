#include "interrupt_handler.hpp"

#include "../common/utils.hpp"
#include "../memory/mmu.hpp"
#include "cpu.hpp"

using namespace utils;

bool InterruptHandler::handle()
{
    uint8_t IE = mmu->read(INTERRUPT_ENABLE_ADDR);
    uint8_t IF = mmu->read(INTERRUPT_FLAG_ADDR);

    if (isNthBitSet(IE, interruptFlagBit) && isNthBitSet(IF, interruptFlagBit))
    {
        word pc = cpu->getProgramCounter();
        word sp = cpu->getStackPointer();
        sp -= 2;
        cpu->setStackPointer(sp);
        mmu->writeWord(sp, pc);

        setNthBit(reinterpret_cast<int&>(IF), interruptFlagBit, false);
        mmu->write(INTERRUPT_FLAG_ADDR, IF);
        cpu->setProgramCounter(interruptRoutineAddr);
        return true;
    }

    return false;
}
