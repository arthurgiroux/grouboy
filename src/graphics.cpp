#include <iostream>
#include <cassert>
#include <SDL2/SDL.h>
#include <stdint.h>
#include <cassert>
#include <chrono>

#include "graphics.hpp"

Graphics::Graphics(MMU* _mmu) :
	scanline(0) {
	assert(_mmu != nullptr);
	mmu = _mmu;

	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		std::cout << "error while initiliazing sdl library : " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	window = SDL_CreateWindow("Gameboy Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	windowDebug = SDL_CreateWindow("Gameboy Emulator Debug", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 8, 8, SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED);

	if (window == nullptr) {
		std::cout << "error while creating window : " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	rendererDebug = SDL_CreateRenderer(windowDebug, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == nullptr) {
		SDL_DestroyWindow(window);
		std::cout << "error while creating renderer : " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	currentFrame = SDL_CreateRGBSurface(SDL_SWSURFACE, WINDOW_WIDTH, WINDOW_HEIGHT, 32, 0, 0, 0, 0);
	currentFrameDebug = SDL_CreateRGBSurface(SDL_SWSURFACE, 8, 8, 32, 0, 0, 0, 0);

}

Graphics::~Graphics() {
	if (renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
	}

	if (window != nullptr) {
		SDL_DestroyWindow(window);
	}

	if (currentFrame != nullptr) {
		SDL_FreeSurface(currentFrame);
	}

	SDL_Quit();
}

void Graphics::renderCurrentFrame() {

	updateParameters();

	byte line[WINDOW_WIDTH * 4] = { 255 };
	uint16_t y = scanline + scrollY;

	uint16_t map = paramBackgroundTileMap ? ADDR_MAP_0 : ADDR_MAP_1;

	for (int tile = 0; tile < TILES_PER_LINE; ++tile) {
		byte tileId = mmu->read((map + y + scrollX) / 8);

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
		uint16_t colors = mmu->readWord(tileAddr + 2 * (y / 8));

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

	SDL_LockSurface(currentFrame);
	//memset(currentFrame->pixels, 0, currentFrame->pitch * currentFrame->w);
	memcpy(static_cast<Uint8*>(currentFrame->pixels) + (scanline * currentFrame->pitch), line, currentFrame->pitch);
	SDL_UnlockSurface(currentFrame);

}

void Graphics::updateScreen() {



	byte tileDebug[8 * 8 * 4] = { 255 };

	//for (int a = 0; a < 256; ++a) {
	int a = 0;
	uint16_t tileAddr = ADDR_TILE_MAP_0 + a * 8 * 2;

	for (int i = 0; i < 8; i++) {
		/*byte color = ((((colors >> 8) & j) >> (j - 2)) |
		((colors & j) >> (j - 1)));*/

		uint16_t colors = mmu->readWord(tileAddr + i * 2);

		for (int j = 7; j >= 0; j--) {
			byte color = (((colors >> j) << 1) & 0x02) | ((colors >> 8 >> j) & 0x01);

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

			tileDebug[4 * (8 * i + (7 - j))] = colorChosen;
			tileDebug[4 * (8 * i + (7 - j)) + 1] = colorChosen;
			tileDebug[4 * (8 * i + (7 - j)) + 2] = colorChosen;


			//tileDebug[4 * (8 * i + j)] = j / 7.0f * 255;
			//tileDebug[4 * (8 * i + j) + 1] = i / 7.0f * 255;
			//tileDebug[4 * (8 * i + j) + 2] = 0;
		}
	}

	SDL_LockSurface(currentFrameDebug);
	//memset(currentFrame->pixels, 0, currentFrame->pitch * currentFrame->w);
	memcpy(static_cast<Uint8*>(currentFrameDebug->pixels), tileDebug, sizeof(tileDebug));
	SDL_UnlockSurface(currentFrameDebug);
	//}

	static std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point curTime = std::chrono::high_resolution_clock::now();

	std::cout << "last update was : " << std::chrono::duration_cast<std::chrono::milliseconds>(curTime - last).count() << std::endl;
	last = curTime;

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, currentFrame);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(texture);

	SDL_Texture *textureDebug = SDL_CreateTextureFromSurface(rendererDebug, currentFrameDebug);
	SDL_RenderClear(rendererDebug);
	SDL_RenderCopy(rendererDebug, textureDebug, NULL, NULL);
	SDL_RenderPresent(rendererDebug);
	SDL_DestroyTexture(textureDebug);

	SDL_Event events = { 0 };
	SDL_WaitEvent(&events);
}

byte Graphics::getScanline() {
	return scanline;
}

void Graphics::setScanline(byte value) {
	scanline = value;
	mmu->write(ADDR_SCANLINE, value);
}

void Graphics::updateParameters() {
	scrollX = mmu->read(ADDR_SCROLL_X);
	scrollY = mmu->read(ADDR_SCROLL_Y);
	scanline = mmu->read(ADDR_SCANLINE);

	byte lcdGpuControl = mmu->read(ADDR_LCD_GPU_CONTROL);
	paramBackgroundStatus = ((lcdGpuControl & 1) == 1);
	paramBackgroundStatus = ((lcdGpuControl & 2) == 2);
	paramSpriteSize = ((lcdGpuControl & 3) == 3);
	paramBackgroundTileMap = ((lcdGpuControl & 4) == 4);
	paramBackgroundTileSet = ((lcdGpuControl & 5) == 5);
	paramWindowStatus = ((lcdGpuControl & 6) == 6);
	paramWindowTileMap = ((lcdGpuControl & 7) == 7);
	paramDisplayStatus = ((lcdGpuControl & 8) == 8);

}