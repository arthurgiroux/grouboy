#ifndef EMULATOR_H
#define EMULATOR_H

#include "cpu.hpp"
#include "mmu.hpp"
#include "graphics.hpp"

#define OAM_ACCESS_TICKS		80
#define VRAM_ACCESS_TICKS		172
#define HBLANK_TICKS			204
#define VBLANK_TICKS			456

enum GPUMode { OAM_ACCESS, VRAM_ACCESS, HBLANK, VBLANK };

class Emulator {
public:
	Emulator();
	~Emulator();
	void start();

private:
	CPU* cpu;
	MMU* mmu;
	Graphics* graphics;
	GPUMode gpuMode;
	int tickCounter;
};

#endif
