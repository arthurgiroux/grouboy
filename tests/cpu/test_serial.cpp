#include "emulator.hpp"
#include "serial/serial_transfer_manager.hpp"

#include <gtest/gtest.h>

TEST(SerialTransferManagerTest, IsTransferRequestedOrInProgressShouldReturnFalseAtInit)
{
    Emulator emulator;
    SerialTransferManager serialTransferManager(&emulator.getMMU());

    ASSERT_FALSE(serialTransferManager.isTransferRequestedOrInProgress());
}

TEST(SerialTransferManagerTest, IsTransferRequestedOrInProgressShouldReturnTrueWhenTransferActive)
{
    Emulator emulator;
    SerialTransferManager serialTransferManager(&emulator.getMMU());
    emulator.getMMU().write(0xFF02, 0x81);
    ASSERT_TRUE(serialTransferManager.isTransferRequestedOrInProgress());
}

TEST(SerialTransferManagerTest, GetTransferDataReturnsCorrectTransferData)
{
    Emulator emulator;
    SerialTransferManager serialTransferManager(&emulator.getMMU());
    byte data = 0x42;
    emulator.getMMU().write(0xFF02, 0x81);
    emulator.getMMU().write(0xFF01, data);
    ASSERT_TRUE(serialTransferManager.isTransferRequestedOrInProgress());
    ASSERT_EQ(serialTransferManager.getTransferData(), data);
}

TEST(SerialTransferManagerTest, FinalizeTransferShouldClearTransferInProgressFlag)
{
    Emulator emulator;
    SerialTransferManager serialTransferManager(&emulator.getMMU());
    emulator.getMMU().write(0xFF02, 0x81);
    ASSERT_TRUE(serialTransferManager.isTransferRequestedOrInProgress());
    serialTransferManager.finalizeTransfer();
    ASSERT_FALSE(serialTransferManager.isTransferRequestedOrInProgress());
}