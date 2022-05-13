
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

    byte* frameScanlineStartAddr = temporaryFrame.data() + scanline * SCREEN_WIDTH * 3;

    TileMap background = getTileMap(useBackgroundTileMap0 ? 0 : 1);

    // Retrieve the lines in the Tilemap that corresponds to the current scanline
    int lineInTileMap = (((scanline + scrollY) / Tile::TILE_HEIGHT) * TILEMAP_WIDTH);

    /*
     * For each pixels in the line, we are going to retrieve the corresponding tile and copy the
     * pixels that corresponds in the current frame buffer.
     */
    for (int i = 0; i < SCREEN_WIDTH; ++i) {
        int xIndexOffset = scrollX + i;
        // The background map is not clamped, if we go "too far right",
        // it will display tiles that are on the left.
        if (xIndexOffset > TILEMAP_WIDTH * Tile::TILE_WIDTH) {
            xIndexOffset = (TILEMAP_WIDTH * Tile::TILE_WIDTH) - xIndexOffset;
        }

        int tileIndex = lineInTileMap + (xIndexOffset / Tile::TILE_WIDTH);
        const Tile::TileRGBArray& tileRGBArray = background[tileIndex].toRGB();

        int xOffsetTile = (scanline + scrollY) % Tile::TILE_HEIGHT;
        int yOffsetTile = (xIndexOffset % Tile::TILE_WIDTH);

        // TODO: Encapsulate in a Frame class.
        frameScanlineStartAddr[i * 3] = tileRGBArray[xOffsetTile * Tile::TILE_WIDTH * 3 + yOffsetTile];
        frameScanlineStartAddr[i * 3 + 1] = tileRGBArray[xOffsetTile * Tile::TILE_WIDTH * 3 + yOffsetTile + 1];
        frameScanlineStartAddr[i * 3 + 2] = tileRGBArray[xOffsetTile * Tile::TILE_WIDTH * 3 + yOffsetTile + 2];
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