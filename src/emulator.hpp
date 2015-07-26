#ifndef EMULATOR_H
#define EMULATOR_H

#include "cpu.hpp"
#include "mmu.hpp"

class Emulator {
public:
	Emulator();
	~Emulator();

private:
	CPU* cpu;
	MMU* mmu;
};

#endif
