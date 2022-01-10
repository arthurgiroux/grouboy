#include "emulator.hpp"

Emulator::Emulator() :
	cpu(mmu),
	gpu(mmu) {
}

Emulator::~Emulator() = default;

void Emulator::exec() {
    // Execute current cpu instruction and retrieve the number of ticks
    int lastInstructionTicks = cpu.exec();

    gpu.step(lastInstructionTicks);
}
