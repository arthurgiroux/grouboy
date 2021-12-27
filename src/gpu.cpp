
#include "gpu.hpp"

void GPU::step(int nbrTicks) {
    ticksSpentInCurrentMode += nbrTicks;

    if (currentMode == OAM_ACCESS && ticksSpentInCurrentMode >= OAM_ACCESS_TICKS) {
        setMode(VRAM_ACCESS);
    }
    else if (currentMode == VRAM_ACCESS && ticksSpentInCurrentMode >= VRAM_ACCESS_TICKS) {
        setMode(HBLANK);
        renderScanline();
    }
    else if (currentMode == HBLANK && ticksSpentInCurrentMode >= HBLANK_TICKS) {
        currentScanline++;

        if (currentScanline == SCREEN_HEIGHT) {
            setMode(VBLANK);
            renderFrame();
        }
        else {
            setMode(OAM_ACCESS);
        }
    }
    else if (currentMode == VBLANK && ticksSpentInCurrentMode >= VBLANK_TICKS) {
        setMode(OAM_ACCESS);
        currentScanline = 0;
    }
}

void GPU::renderScanline() {

}

void GPU::renderFrame() {

}

GPU::GPU() {
}
