#include "emulator.hpp"
#include <thread>
#include <iostream>

Emulator::Emulator() :
	graphics(mmu),
	cpu(mmu) {
	tickCounter = 0;
	gpuMode = HBLANK;
}

Emulator::~Emulator() {

}

void Emulator::start() {

	std::chrono::high_resolution_clock::time_point lastEval = std::chrono::high_resolution_clock::now();
	while (true) {

		std::chrono::high_resolution_clock::time_point curTime = std::chrono::high_resolution_clock::now();

		if (std::chrono::duration_cast<std::chrono::nanoseconds>(curTime - lastEval).count() <= 25000) {
			//std::cerr << "waiting " << std::chrono::duration_cast<std::chrono::nanoseconds>(curTime - lastEval).count() << std::endl;
			continue;
		}
		//std::cerr << "executing 1 step : " << std::chrono::duration_cast<std::chrono::nanoseconds>(curTime - lastEval).count() << std::endl;

		lastEval = curTime;

		int lastInstructionTicks = cpu.exec();

		tickCounter += lastInstructionTicks;

		switch (gpuMode) {
		case OAM_ACCESS:
			if (tickCounter >= OAM_ACCESS_TICKS) {
				gpuMode = VRAM_ACCESS;
				tickCounter = 0;
			}
			break;

		case VRAM_ACCESS:
			if (tickCounter >= VRAM_ACCESS_TICKS) {
				gpuMode = HBLANK;
				tickCounter = 0;
				graphics.renderCurrentFrame();
			}
			break;

		case HBLANK:
			if (tickCounter >= HBLANK_TICKS) {
				tickCounter = 0;
				byte scanline = graphics.getScanline();
				if (scanline == (WINDOW_HEIGHT - 1)) {
					gpuMode = VBLANK;
					graphics.updateScreen();
				}
				else {
					gpuMode = OAM_ACCESS;
				}
				graphics.setScanline(scanline + 1);
			}
			break;

		case VBLANK:
			if (tickCounter >= VBLANK_TICKS) {
				tickCounter = 0;

				byte scanline = graphics.getScanline() + 1;


				if (scanline > (WINDOW_HEIGHT - 1) + 10) {
					gpuMode = OAM_ACCESS;
					scanline = 0;
				}

				graphics.setScanline(scanline);

			}
			break;

		default:
			break;
		}

	}
}
