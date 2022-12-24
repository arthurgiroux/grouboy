#include "interrupt_handler.hpp"
#include "cpu.hpp"

using namespace utils;

bool InterruptHandler::handle()
{
    if (_interruptManager->isInterruptEnabled(_interruptType) && _interruptManager->isInterruptPending(_interruptType))
    {
        cpu->callInterruptRoutine(interruptRoutineAddr);
        _interruptManager->clearInterrupt(_interruptType);
        return true;
    }

    return false;
}
