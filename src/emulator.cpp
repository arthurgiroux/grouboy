#include "emulator.hpp"

Emulator::Emulator() {
	mmu = new MMU();
	cpu = new CPU(mmu);
}

Emulator::~Emulator() {
	delete cpu;
	delete mmu;
}
