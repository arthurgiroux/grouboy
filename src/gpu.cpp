
#include "gpu.hpp"

void GPU::step(int nbrTicks) {
    ticksSpentInCurrentMode += nbrTicks;

    if (currentMode == OAM_ACCESS && ticksSpentInCurrentMode >= OAM_ACCESS_TICKS) {
        setMode(VRAM_ACCESS);
    }
    else if (currentMode == VRAM_ACCESS && ticksSpentInCurrentMode >= VRAM_ACCESS_TICKS) {
        setMode(HBLANK);
        renderScanline(currentScanline);
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

void GPU::renderScanline(int scanline) {
    updateParameters();

    uint16_t y = scanline + scrollY;

    uint16_t map = useBackgroundTileMap0 ? ADDR_MAP_0 : ADDR_MAP_1;

    uint16_t baseAddr = map + (scrollX + scanline) * TILE_MAP_SIZE + scrollX;

    for (int tile = 0; tile < TILES_PER_LINE; ++tile) {
        byte tileId = mmu.read( baseAddr + tile);
        // TODO: update pixels
    }
}

void GPU::renderFrame() {
    currentFrame = temporaryFrame;
    frameId++;
}

GPU::GPU(MMU& mmu_) : mmu(mmu_) {
}

void GPU::updateParameters() {
    scrollX = mmu.read(ADDR_SCROLL_X);
    scrollY = mmu.read(ADDR_SCROLL_Y);
    //scanline = mmu.read(ADDR_SCANLINE);

    byte lcdGpuControl = mmu.read(ADDR_LCD_GPU_CONTROL);
    paramBackgroundStatus = ((lcdGpuControl & 1) == 1);
    paramBackgroundStatus = ((lcdGpuControl & 2) == 2);
    paramSpriteSize = ((lcdGpuControl & 3) == 3);
    useBackgroundTileMap0 = ((lcdGpuControl & 4) == 4);
    paramBackgroundTileSet = ((lcdGpuControl & 5) == 5);
    paramWindowStatus = ((lcdGpuControl & 6) == 6);
    paramWindowTileMap = ((lcdGpuControl & 7) == 7);
    paramDisplayStatus = ((lcdGpuControl & 8) == 8);
}

Tile GPU::getTileById(int16_t tileId, int8_t tileSetId) {
    int tileSetOffset = ADDR_TILE_SET_0;
    if (tileSetId == 1) {
        tileSetOffset = ADDR_TILE_SET_1;
    }

    uint16_t tileBaseAddr = tileSetOffset + Tile::BYTES_PER_TILE * tileId;
    Tile::TileDataArray dataArray = {};
    // TODO: Read word
    for (int i = 0; i < Tile::BYTES_PER_TILE; ++i) {
        dataArray[i] = mmu.read(tileBaseAddr + i);
    }

    return Tile(dataArray);
}

GPU::TileMap GPU::getTileMap(int index) {
    int tileMapAddr = (index == 0) ? ADDR_MAP_0 : ADDR_MAP_1;
    TileMap map = {};

    for (int i = 0; i < TILEMAP_WIDTH*TILEMAP_HEIGHT; ++i) {
        sbyte tileId = mmu.read(tileMapAddr + i);
        map.push_back(getTileById(tileId, index));
    }

    return map;
}