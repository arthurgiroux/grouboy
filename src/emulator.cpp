#include "emulator.hpp"
#include <iostream>

Emulator::Emulator() : cpu(mmu), ppu(mmu)
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
}

void Emulator::reset()
{
    ppu.reset();
    cpu.reset();
    mmu.reset();
    currentTicks = 0;
}