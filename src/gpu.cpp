
#include "gpu.hpp"
#include <bitset>

void GPU::step(int nbrTicks)
{
	// TODO: Check if we really need this multiplier
	ticksSpentInCurrentMode += nbrTicks * CPU_TICKS_TO_GPU_TICKS;

	if (currentMode == OAM_ACCESS && ticksSpentInCurrentMode >= OAM_ACCESS_TICKS)
	{
		setMode(VRAM_ACCESS);
	}
	else if (currentMode == VRAM_ACCESS && ticksSpentInCurrentMode >= VRAM_ACCESS_TICKS)
	{
		setMode(HBLANK);
		renderScanline(currentScanline);
	}
	else if (currentMode == HBLANK && ticksSpentInCurrentMode >= HBLANK_TICKS)
	{
		currentScanline++;

		if (currentScanline == SCREEN_HEIGHT)
		{
			setMode(VBLANK);
			renderFrame();
		}
		else
		{
			setMode(OAM_ACCESS);
		}
	}
	else if (currentMode == VBLANK && ticksSpentInCurrentMode >= VBLANK_TICKS)
	{
		currentScanline++;
		ticksSpentInCurrentMode = 0;
		// TODO: Check if it's really necessary to increase scanline value during vblank
		if (currentScanline > MAX_SCANLINE_VALUE)
		{
			setMode(OAM_ACCESS);
			currentScanline = 0;
		}
	}
	mmu.write(ADDR_SCANLINE, currentScanline);
}

void GPU::renderScanline(int scanline)
{
	updateParameters();

	// We retrieve the offset of the line we want to render in the RGB frame
	byte* frameScanlineStartAddr = temporaryFrame.data() + scanline * SCREEN_WIDTH * BYTES_PER_PIXEL;

	// Retrieve the tilemap we are going to use
	TileMap background = getTileMap(useBackgroundTileMap1 ? 1 : 0);

	/*
	 * The tilemap is a 32x32 map of tiles of 8x8 pixels.
	 * Since we are rendering only a line here, we can already compute which
	 * line of the tilemap we are going to render.
	 * We can compute the line by seeing how many tiles we span vertically.
	 */
	int lineInTileMap = (scanline + scrollY) / Tile::TILE_HEIGHT;

	int offsetInTileMap = lineInTileMap * TILEMAP_WIDTH;

	/*
	 * For each pixels in the line, we are going to retrieve the corresponding tile and copy the
	 * pixels that corresponds in the current frame buffer.
	 */
	for (int x = 0; x < SCREEN_WIDTH; ++x)
	{
		int xIndexOffset = scrollX + x;

		// The background map is not clamped, if we go "too far right",
		// it should display tiles that are on the left.
		if (xIndexOffset > TILEMAP_WIDTH * Tile::TILE_WIDTH)
		{
			xIndexOffset = (TILEMAP_WIDTH * Tile::TILE_WIDTH) - xIndexOffset;
		}

		// We see how many tiles we span horizontally and add it to our offset to find the tile index
		int tileIndex = offsetInTileMap + (xIndexOffset / Tile::TILE_WIDTH);
		const Tile::TileRGBArray& tileRGBArray = background[tileIndex].toRGB();

		/*
		 * Now that we retrieve the tile corresponding to the pixel, we need to copy the pixel
		 * that corresponds to the one we are rendering.
		 * Since scrollX have a 1-pixel resolution it means that we can sometimes render only part of a tile for a line.
		 */
		int xOffsetTile = xIndexOffset % Tile::TILE_WIDTH;
		int yOffsetTile = (scanline + scrollY) % Tile::TILE_HEIGHT;

		// Finally we copy the correct pixel in our temporary frame.
		// TODO: Encapsulate in a Frame class.
		frameScanlineStartAddr[x * BYTES_PER_PIXEL] =
		    tileRGBArray[(yOffsetTile * Tile::TILE_WIDTH + xOffsetTile) * BYTES_PER_PIXEL + 0];
		frameScanlineStartAddr[x * BYTES_PER_PIXEL + 1] =
		    tileRGBArray[(yOffsetTile * Tile::TILE_WIDTH + xOffsetTile) * BYTES_PER_PIXEL + 1];
		frameScanlineStartAddr[x * BYTES_PER_PIXEL + 2] =
		    tileRGBArray[(yOffsetTile * Tile::TILE_WIDTH + xOffsetTile) * BYTES_PER_PIXEL + 2];
	}
}

void GPU::renderFrame()
{
	currentFrame = temporaryFrame;
	frameId++;
}

GPU::GPU(MMU& mmu_) : mmu(mmu_)
{
}

void GPU::updateParameters()
{
	scrollX = mmu.read(ADDR_SCROLL_X);
	scrollY = mmu.read(ADDR_SCROLL_Y);
	std::bitset<8> lcdGpuControl = mmu.read(ADDR_LCD_GPU_CONTROL);
	paramBackgroundStatus = lcdGpuControl[0];
	paramSpritesStatus = lcdGpuControl[1];
	paramSpriteSize = lcdGpuControl[2];
	useBackgroundTileMap1 = lcdGpuControl[3];
	paramBackgroundTileSet = lcdGpuControl[4];
	paramWindowStatus = lcdGpuControl[5];
	paramWindowTileMap = lcdGpuControl[6];
	paramDisplayStatus = lcdGpuControl[7];
}

Tile GPU::getTileById(int16_t tileId, int8_t tileSetId)
{
	int tileSetOffset = ADDR_TILE_SET_0;
	if (tileSetId == 1)
	{
		tileSetOffset = ADDR_TILE_SET_1;
	}

	uint16_t tileBaseAddr = tileSetOffset + Tile::BYTES_PER_TILE * tileId;
	Tile::TileDataArray dataArray = {};
	// TODO: Read word
	for (int i = 0; i < Tile::BYTES_PER_TILE; ++i)
	{
		dataArray[i] = mmu.read(tileBaseAddr + i);
	}

	return Tile(dataArray);
}

GPU::TileMap GPU::getTileMap(int index)
{
	int tileMapAddr = (index == 0) ? ADDR_MAP_0 : ADDR_MAP_1;
	TileMap map = {};

	for (int i = 0; i < TILEMAP_WIDTH * TILEMAP_HEIGHT; ++i)
	{
		sbyte tileId = mmu.read(tileMapAddr + i);
		map.push_back(getTileById(tileId, index));
	}

	return map;
}