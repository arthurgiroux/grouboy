#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include "mmu.hpp"

#define WINDOW_WIDTH 160
#define WINDOW_HEIGHT 144

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
	Graphics(MMU* _mmu);
	~Graphics();
	void renderCurrentFrame();
	byte getScanline();
	void setScanline(byte value);
	void updateScreen();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* currentFrame;

	SDL_Window* windowDebug;
	SDL_Renderer* rendererDebug;
	SDL_Surface* currentFrameDebug;
	MMU* mmu;

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
