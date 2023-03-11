#include "emulator.hpp"
#include <gtest/gtest.h>

class MoonEyeTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        serialTransferManager = std::make_unique<SerialTransferManager>(&emulator.getMMU());
    }

    void assertTestForRomArePassing(const std::string& romName)
    {
        std::string rom = std::string(DATADIR) + "/roms/mooneye/" + romName;
        ASSERT_TRUE(emulator.getMMU().loadCartridge(rom));
        bool infiniteJRDetected = false;
        while (!infiniteJRDetected)
        {

            emulator.exec();
            readSerialOutput();

            infiniteJRDetected = isNextInstructionInfiniteJR();
        }

        // A successful test will write a fibonacci sequence to the serial port
        std::vector<byte> successSequence = {3, 5, 8, 13, 21, 34};
        ASSERT_EQ(successSequence, output);
    }

  private:
    void readSerialOutput()
    {
        if (serialTransferManager->isTransferRequestedOrInProgress())
        {
            byte serialData = serialTransferManager->getTransferData();
            output.push_back(serialData);
            serialTransferManager->finalizeTransfer();
        }
    }

    bool isNextInstructionInfiniteJR()
    {
        int nextInstruction = emulator.getMMU().read(emulator.getCPU().getProgramCounter());
        if (nextInstruction == standardInstructions::JR_n)
        {
            int offset = emulator.getMMU().read(emulator.getCPU().getProgramCounter() + 1);
            return (static_cast<sbyte>(offset) == -2);
        }

        return false;
    }

    Emulator emulator;
    std::unique_ptr<SerialTransferManager> serialTransferManager;
    std::vector<byte> output = {};
};

TEST_F(MoonEyeTest, TestForBitsMemOamShouldBeSuccessful)
{
    assertTestForRomArePassing("acceptance/bits/mem_oam.gb");
}

TEST_F(MoonEyeTest, TestForBitsRegFShouldBeSuccessful)
{
    assertTestForRomArePassing("acceptance/bits/reg_f.gb");
}

TEST_F(MoonEyeTest, TestForUnusedHWIOBitsShouldBeSuccessful)
{
    assertTestForRomArePassing("acceptance/bits/unused_hwio-GS.gb");
}

TEST_F(MoonEyeTest, TestForDaaInstrShouldBeSuccessful)
{
    assertTestForRomArePassing("acceptance/instr/daa.gb");
}

TEST_F(MoonEyeTest, TestForDMABasicShouldBeSuccessful)
{
    assertTestForRomArePassing("acceptance/oam_dma/basic.gb");
}
