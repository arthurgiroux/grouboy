#include "emulator.hpp"
#include <gtest/gtest.h>

class BlarggTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        serialTransferManager = std::make_unique<SerialTransferManager>(&emulator.getMMU());
    }

    void assertTestForRomArePassing(const std::string& romName)
    {
        std::string rom = std::string(DATADIR) + "/roms/blargg/" + romName;
        ASSERT_TRUE(emulator.getMMU().loadCartridgeFromFile(rom));
        bool infiniteJRDetected = false;
        while (!infiniteJRDetected)
        {

            emulator.exec();
            readSerialOutput();

            infiniteJRDetected = isNextInstructionInfiniteJR();
        }

        std::string failureKeyword = "Failed";
        ASSERT_EQ(output.find(failureKeyword), std::string::npos) << output;
    }

  private:
    void readSerialOutput()
    {
        if (serialTransferManager->isTransferRequestedOrInProgress())
        {
            byte serialData = serialTransferManager->getTransferData();
            output += serialData;
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
    std::string output = "";
};

TEST_F(BlarggTest, TestForSpecialInstructionShouldBeSuccessful)
{
    assertTestForRomArePassing("01-special.gb");
}

TEST_F(BlarggTest, TestForInterruptsShouldBeSuccessful)
{
    assertTestForRomArePassing("02-interrupts.gb");
}

TEST_F(BlarggTest, TestForOperationsSPHLShouldBeSuccessful)
{
    assertTestForRomArePassing("03-op sp,hl.gb");
}

TEST_F(BlarggTest, TestForOperationsRegisterImmediateShouldBeSuccessful)
{
    assertTestForRomArePassing("04-op r,imm.gb");
}

TEST_F(BlarggTest, TestForOperationsRPShouldBeSuccessful)
{
    assertTestForRomArePassing("05-op rp.gb");
}

TEST_F(BlarggTest, TestForLoadRRShouldBeSuccessful)
{
    assertTestForRomArePassing("06-ld r,r.gb");
}

TEST_F(BlarggTest, TestForJumpRelativeJumpCallRetRstShouldBeSuccessful)
{
    assertTestForRomArePassing("07-jr,jp,call,ret,rst.gb");
}

TEST_F(BlarggTest, TestForMiscInstructionsShouldBeSuccessful)
{
    assertTestForRomArePassing("08-misc instrs.gb");
}

TEST_F(BlarggTest, TestForOperationsRRShouldBeSuccessful)
{
    assertTestForRomArePassing("09-op r,r.gb");
}

TEST_F(BlarggTest, TestForBitOperationsShouldBeSuccessful)
{
    assertTestForRomArePassing("10-bit ops.gb");
}

TEST_F(BlarggTest, TestForBitOperationsAHLShouldBeSuccessful)
{
    assertTestForRomArePassing("11-op a,(hl).gb");
}

TEST_F(BlarggTest, TestForInstrTimingShouldBeSuccessful)
{
    assertTestForRomArePassing("instr_timing.gb");
}

TEST_F(BlarggTest, TestForSoundRegistersShouldBeSuccessful)
{
    assertTestForRomArePassing("sound/01-registers.gb");
}

TEST_F(BlarggTest, TestForSoundLengthTimerShouldBeSuccessful)
{
    assertTestForRomArePassing("sound/02-len ctr.gb");
}

TEST_F(BlarggTest, TestForSoundWavelengthSweepShouldBeSuccessful)
{
    assertTestForRomArePassing("sound/04-sweep.gb");
}

TEST_F(BlarggTest, TestForSoundOverflowOnTriggerShouldBeSuccessful)
{
    assertTestForRomArePassing("sound/06-overflow on trigger.gb");
}