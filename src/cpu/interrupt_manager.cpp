#include "cpu/interrupt_manager.hpp"
#include "cpu/interrupt_handler.hpp"

bool InterruptManager::isInterruptEnabled(InterruptType type)
{
    return utils::isNthBitSet(_mmu->read(INTERRUPT_ENABLE_ADDR), bitPositionInFlagForInterruptType[type]);
}

bool InterruptManager::isInterruptPending(InterruptType type)
{
    return utils::isNthBitSet(_mmu->read(INTERRUPT_FLAG_ADDR), bitPositionInFlagForInterruptType[type]);
}

void InterruptManager::raiseInterrupt(InterruptType type)
{
    int interruptFlag = _mmu->read(INTERRUPT_FLAG_ADDR);
    utils::setNthBit(interruptFlag, bitPositionInFlagForInterruptType[type], true);
    _mmu->write(INTERRUPT_FLAG_ADDR, interruptFlag);
}

void InterruptManager::clearInterrupt(InterruptType type)
{
    int interruptFlag = _mmu->read(INTERRUPT_FLAG_ADDR);
    utils::setNthBit(interruptFlag, bitPositionInFlagForInterruptType[type], false);
    _mmu->write(INTERRUPT_FLAG_ADDR, interruptFlag);
}

bool InterruptManager::isAnyInterruptEnabled()
{
    return _mmu->read(INTERRUPT_ENABLE_ADDR) > 0;
}

bool InterruptManager::isAnyInterruptPending()
{
    return _mmu->read(INTERRUPT_FLAG_ADDR) > 0;
}

InterruptManager::InterruptManager(CPU* cpu, MMU* mmu) : _mmu(mmu)
{
    _interruptHandlers.push_back(std::make_unique<InterruptHandlerVBlank>(cpu, this));
    _interruptHandlers.push_back(std::make_unique<InterruptHandlerLCDStat>(cpu, this));
    _interruptHandlers.push_back(std::make_unique<InterruptHandlerTimer>(cpu, this));
    _interruptHandlers.push_back(std::make_unique<InterruptHandlerSerial>(cpu, this));
    _interruptHandlers.push_back(std::make_unique<InterruptHandlerJoypad>(cpu, this));
}

bool InterruptManager::handleInterrupts()
{
    for (auto& handler : _interruptHandlers)
    {
        if (handler->handle())
        {
            return true;
        }
    }

    return false;
}
