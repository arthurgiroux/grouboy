#ifndef GBEMULATOR_SERIAL_TRANSFER_MANAGER_HPP
#define GBEMULATOR_SERIAL_TRANSFER_MANAGER_HPP

#include "common/types.hpp"
#include "memory/mmu.hpp"

/**
 * This class is responsible for managing outgoing serial transfer.
 * It's making data available to external clients and
 * handling the logic for consuming a transfer.
 */
class SerialTransferManager
{
  public:
    /**
     * Create a new serial transfer manager
     * @param mmu   The memory unit to retrieve the transfer data
     */
    SerialTransferManager(MMU* mmu) : _mmu(mmu){};
    ~SerialTransferManager() = default;

    /**
     * Returns if a transfer has been requested or is currently in progress
     * @return  True if a transfer is active, false otherwise
     */
    bool isTransferRequestedOrInProgress();

    /**
     * Retrieve the data of the current transfer.
     *
     * @return  The byte that was transferred
     */
    byte getTransferData();

    /**
     * Finalize the transfer, this involves clearing the transfer flag and
     * calling the serial interrupt.
     * A transfer has to be finalized before a new transfer can take place.
     */
    void finalizeTransfer();

  private:
    static const int SERIAL_TRANSFER_CONTROL_ADDR = 0xFF02;
    static const int TRANSFER_START_FLAG_BIT = 7;
    static const int SERIAL_TRANSFER_DATA_ADDR = 0xFF01;
    MMU* _mmu;
};

#endif // GBEMULATOR_SERIAL_TRANSFER_MANAGER_HPP
