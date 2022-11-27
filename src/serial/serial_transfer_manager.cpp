#include "serial_transfer_manager.hpp"

bool SerialTransferManager::isTransferRequestedOrInProgress()
{
    byte transferControl = _mmu->read(SERIAL_TRANSFER_CONTROL_ADDR);
    return utils::isNthBitSet(transferControl, TRANSFER_START_FLAG_BIT);
}

byte SerialTransferManager::getTransferData()
{
    return _mmu->read(SERIAL_TRANSFER_DATA_ADDR);
}

void SerialTransferManager::finalizeTransfer()
{
    // We clear the transfer flag to allow for next transfer
    int transferControl = _mmu->read(SERIAL_TRANSFER_CONTROL_ADDR);
    utils::setNthBit(transferControl, TRANSFER_START_FLAG_BIT, false);
    _mmu->write(SERIAL_TRANSFER_CONTROL_ADDR, transferControl);
}