#include "emulator.hpp"

Emulator::Emulator() {
	mmu = new MMU();
    graphics = new Graphics(mmu);
	cpu = new CPU(mmu);
    tickCounter = 0;
    gpuMode = HBLANK;
}

Emulator::~Emulator() {
    delete graphics;
	delete cpu;
	delete mmu;
}

void Emulator::start() {
    while (true) {
        int lastInstructionTicks = cpu->exec();
        
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
                    graphics->renderCurrentFrame();
                    
                }
                break;
                
            case HBLANK:
                if (tickCounter >= HBLANK_TICKS) {
                    tickCounter = 0;
                    byte scanline = graphics->getScanline() + 1;
                    graphics->setScanline(scanline);
                    if (scanline == (WINDOW_HEIGHT - 1)) {
                        gpuMode = VBLANK;
                        graphics->updateScreen();
                    }
                    else {
                        gpuMode = OAM_ACCESS;
                    }
                }
                break;
                
            case VBLANK:
                if (tickCounter >= VBLANK_TICKS) {
                    tickCounter = 0;
                    
                    byte scanline = graphics->getScanline() + 1;
                    
                    
                    if (scanline > (WINDOW_HEIGHT - 1) + 10) {
                        gpuMode = OAM_ACCESS;
                        scanline = 0;
                    }
                    
                    graphics->setScanline(scanline);
                    
                }
                break;
        
            default:
                break;
        }
        
        
    }
}
