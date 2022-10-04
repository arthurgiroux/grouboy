#include "interrupt_handler.hpp"

#include "mmu.hpp"
#include "cpu.hpp"
#include "utils.hpp"

using namespace utils;

bool InterruptHandler::handle()
{
    uint8_t IE = mmu->read(INTERRUPT_ENABLE_ADDR);
    uint8_t IF = mmu->read(INTERRUPT_FLAG_ADDR);

    if (isNthBitSet(IE, interruptFlagBit) &&
        isNthBitSet(IF, interruptFlagBit)) {
        uint16_t pc = cpu->getProgramCounter();
        uint16_t sp = cpu->getStackPointer();
        mmu->write(sp, getLsbFromWord(pc));
        mmu->write(sp + 1, getMsbFromWord(pc));

        setNthBit(reinterpret_cast<int&>(IF), interruptFlagBit, false);
		mmu->write(INTERRUPT_FLAG_ADDR, IF);
        cpu->setProgramCounter(interruptRoutineAddr);
		return true;
    }

	return false;
}
