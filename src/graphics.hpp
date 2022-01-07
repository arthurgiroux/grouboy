#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "mmu.hpp"
#include <array>

#define ADDR_LCD_GPU_CONTROL    0xFF40
#define ADDR_SCROLL_Y           0xFF42
#define ADDR_SCROLL_X           0xFF43
#define ADDR_SCANLINE           0xFF44
#define ADDR_BG_PALETTE         0xFF47

#define ADDR_MAP_0              0x9800
#define ADDR_MAP_1              0x9C00

#define ADDR_TILE_MAP_0         0x8000
#define ADDR_TILE_MAP_1         0x9000

#define TILES_PER_LINE          20

class Graphics {

public:
	Graphics(MMU& _mmu);
	~Graphics();
	void renderScanline();
	byte getScanline();
	void setScanline(byte value);

    static const int SCREEN_WIDTH = 160;
    static const int SCREEN_HEIGHT = 144;
    static const int BYTE_PER_PIXEL = 4;
    static const int SCREEN_SIZE_BYTES = SCREEN_WIDTH * SCREEN_HEIGHT * BYTE_PER_PIXEL;

private:
    MMU& mmu;
    std::array<byte, SCREEN_SIZE_BYTES> screenPixels;

	void updateParameters();

	byte scrollY;
	byte scrollX;
	byte scanline;
	int backgroundPalette;

	bool paramBackgroundStatus;
	bool paramSpritesStatus;
	bool paramSpriteSize;
	bool paramBackgroundTileMap;
	bool paramBackgroundTileSet;
	bool paramWindowStatus;
	bool paramWindowTileMap;
	bool paramDisplayStatus;

};

#endif
