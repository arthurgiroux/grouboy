#ifndef EMULATOR_H
#define EMULATOR_H

#include "cpu.hpp"
#include "mmu.hpp"
#include "gpu.hpp"

class Emulator {
public:
	Emulator();
	~Emulator();
	void exec();

private:
    MMU mmu;
	CPU cpu;
	GPU gpu;
};

#endif
