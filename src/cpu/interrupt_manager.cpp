#include "cpu/interrupt_manager.hpp"
#include "cpu/interrupt_handler.hpp"

bool InterruptManager::isInterruptEnabled(InterruptType type)
{
    return utils::isNthBitSet(_interruptEnableFlag, bitPositionInFlagForInterruptType[type]);
}

bool InterruptManager::isInterruptPending(InterruptType type)
{
    return utils::isNthBitSet(_interruptFlag, bitPositionInFlagForInterruptType[type]);
}

void InterruptManager::raiseInterrupt(InterruptType type)
{
    utils::setNthBit(_interruptFlag, bitPositionInFlagForInterruptType[type], true);
}

void InterruptManager::clearInterrupt(InterruptType type)
{
    utils::setNthBit(_interruptFlag, bitPositionInFlagForInterruptType[type], false);
}

bool InterruptManager::isAnyInterruptEnabled()
{
    return _interruptEnableFlag > 0;
}

bool InterruptManager::isAnyInterruptPending()
{
    return (_interruptFlag & 0x1F) > 0;
}

InterruptManager::InterruptManager(CPU* cpu)
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

byte InterruptManager::getInterruptEnableFlag() const
{
    return _interruptEnableFlag;
}

void InterruptManager::setInterruptEnableFlag(byte interruptEnableFlag)
{
    _interruptEnableFlag = interruptEnableFlag;
}

byte InterruptManager::getInterruptFlag() const
{
    return _interruptFlag;
}

void InterruptManager::setInterruptFlag(byte interruptFlag)
{
    _interruptFlag = interruptFlag;
}
