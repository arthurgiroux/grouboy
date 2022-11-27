#include "emulator.hpp"
#include <iostream>

Emulator::Emulator() : cpu(mmu), ppu(mmu), serialTransferManager(&mmu)
{
    mmu.setInputController(&inputController);
}

Emulator::~Emulator() = default;

void Emulator::exec()
{
    // Execute current cpu instruction and retrieve the number of ticks
    int lastInstructionTicks = cpu.fetchDecodeAndExecute();

    ppu.step(lastInstructionTicks);

    currentTicks += lastInstructionTicks;

    if (serialTransferManager.isTransferRequestedOrInProgress())
    {
        byte serialData = serialTransferManager.getTransferData();
        serialTransferManager.finalizeTransfer();

        // TODO: Make this a CLI option
        std::cout << static_cast<char>(serialData);
    }
}

void Emulator::reset()
{
    ppu.reset();
    cpu.reset();
    mmu.reset();
    currentTicks = 0;
}