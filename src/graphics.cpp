#include <iostream>
#include <cassert>
#include <SDL2/SDL.h>
#include <cstdint>
#include <cassert>
#include <chrono>

#include "graphics.hpp"

Graphics::Graphics(MMU& _mmu) :
	scanline(0), mmu(_mmu) {

}

Graphics::~Graphics() {
}

void Graphics::renderScanline() {

	updateParameters();

	byte line[WINDOW_WIDTH * 4] = { 255 };
	uint16_t y = scanline + scrollY;

	uint16_t map = paramBackgroundTileMap ? ADDR_MAP_0 : ADDR_MAP_1;

	for (int tile = 0; tile < TILES_PER_LINE; ++tile) {
		byte tileId = mmu.read((map + y + scrollX) / 8);

		uint16_t tileAddr;
		if (!paramBackgroundTileMap) {
			tileAddr = ADDR_TILE_MAP_1 + tileId * 8;
		}
		else if (tileId >= 128) {
			tileAddr = ADDR_TILE_MAP_0 - (tileId - 127) * 8;
		}
		else {
			tileAddr = ADDR_TILE_MAP_0 + tileId * 8;
		}


		//for (int i = 0; i < 8; ++i) {
		uint16_t colors = mmu.readWord(tileAddr + 2 * (y / 8));

		/* Colors are stored on the two adjacents bytes,
		   the first byte is the LSB and the second the MSB
		   Each bit represents the color of a pixel for the row

			Ex:
				0x00:   01001010
				0x01:   11010100
				Color:  23021210
		 0x01 0x00
			11010100 01001010
		 */

		for (int j = 7; j >= 0; --j) {
			/*byte color = ((((colors >> 8) & j) >> (j - 2)) |
				((colors & j) >> (j - 1)));*/

			byte color = ((colors >> 8 >> (j - 1)) & 0x02) | ((colors >> j) & 0x01);

			// TODO: palette
			byte colorChosen = 0;
			switch (color) {
			case 0:
				colorChosen = 255;
				break;

			case 1:
				colorChosen = 192;
				break;

			case 2:
				colorChosen = 96;
				break;

			case 3:
				colorChosen = 0;
				break;

			default:
				std::cout << "something went wrong during color computation" << std::endl;
				exit(EXIT_FAILURE);
				break;

			}

			line[4 * (tile * 8 + j)] = colorChosen;
			line[4 * (tile * 8 + j) + 1] = colorChosen;
			line[4 * (tile * 8 + j) + 2] = colorChosen;

			//}
		}
	}

}

byte Graphics::getScanline() {
	return scanline;
}

void Graphics::setScanline(byte value) {
	scanline = value;
	mmu.write(ADDR_SCANLINE, value);
}

void Graphics::updateParameters() {
	scrollX = mmu.read(ADDR_SCROLL_X);
	scrollY = mmu.read(ADDR_SCROLL_Y);
	scanline = mmu.read(ADDR_SCANLINE);

	byte lcdGpuControl = mmu.read(ADDR_LCD_GPU_CONTROL);
	paramBackgroundStatus = ((lcdGpuControl & 1) == 1);
	paramBackgroundStatus = ((lcdGpuControl & 2) == 2);
	paramSpriteSize = ((lcdGpuControl & 3) == 3);
	paramBackgroundTileMap = ((lcdGpuControl & 4) == 4);
	paramBackgroundTileSet = ((lcdGpuControl & 5) == 5);
	paramWindowStatus = ((lcdGpuControl & 6) == 6);
	paramWindowTileMap = ((lcdGpuControl & 7) == 7);
	paramDisplayStatus = ((lcdGpuControl & 8) == 8);
}