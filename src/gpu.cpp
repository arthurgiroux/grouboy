
#include "gpu.hpp"
#include <bitset>
#include <cassert>
#include <iostream>

void GPU::step(int nbrTicks)
{
	ticksSpentInCurrentMode += nbrTicks;

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
	if (!isDisplayEnabled())
	{
		return;
	}

	if (areBackgroundAndWindowEnabled())
	{
		renderScanlineBackground(scanline);

		if (isWindowEnabled())
		{
			renderScanlineWindow(scanline);
		}
	}

	if (areSpritesEnabled())
	{
		renderScanlineSprite(scanline);
	}
}

void GPU::renderScanlineBackground(int scanline)
{
	byte scrollX = mmu.read(ADDR_SCROLL_X);
	byte scrollY = mmu.read(ADDR_SCROLL_Y);

	// Retrieve the tilemap we are going to use
	TileMap background = getTileMap(backgroundTileMapIndex());

	/*
	 * The tilemap is a 32x32 map of tiles of 8x8 pixels.
	 * Since we are rendering only a line here, we can already compute which
	 * line of the tilemap we are going to render.
	 * We can compute the line by seeing how many tiles we span vertically.
	 */
	int lineInTileMap = (scanline + scrollY) / Tile::TILE_HEIGHT;
	lineInTileMap %= TILEMAP_HEIGHT;
	assert(lineInTileMap < TILEMAP_HEIGHT);

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
			xIndexOffset %= TILEMAP_WIDTH * Tile::TILE_WIDTH;
		}
		assert(xIndexOffset < TILEMAP_WIDTH * Tile::TILE_WIDTH);

		// We see how many tiles we span horizontally and add it to our offset to find the tile index
		int tileIndex = offsetInTileMap + (xIndexOffset / Tile::TILE_WIDTH);
		const RGBImage& tileImage = background[tileIndex].getImage();

		/*
		 * Now that we retrieve the tile corresponding to the pixel, we need to copy the pixel
		 * that corresponds to the one we are rendering.
		 * Since scrollX have a 1-pixel resolution it means that we can sometimes render only part of a tile for a line.
		 */
		int xOffsetTile = xIndexOffset % Tile::TILE_WIDTH;
		int yOffsetTile = (scanline + scrollY) % Tile::TILE_HEIGHT;

		// Finally we copy the correct pixel in our temporary frame.
		temporaryFrame.copyPixel(x, scanline, tileImage, xOffsetTile, yOffsetTile);
	}
}

void GPU::renderScanlineWindow(int scanline)
{
	// TODO: render window
}

void GPU::renderScanlineSprite(int scanline)
{
	int nbrSpritesInScanline = 0;

	for (int i = 0; i < NBR_SPRITES && nbrSpritesInScanline < MAX_NBR_SPRITES_PER_SCANLINE; ++i)
	{
		// TODO: Fix ugly code
		byte y = mmu.read(SPRITE_ATTR_TABLE_ADDR + 4 * i + 0);
		byte x = mmu.read(SPRITE_ATTR_TABLE_ADDR + 4 * i + 1);
		byte idx = mmu.read(SPRITE_ATTR_TABLE_ADDR + 4 * i + 2);
		int spriteSz = spriteSize();
		int spriteStartVerticalPos = y - 16;
		int spriteEndVerticalPos = spriteStartVerticalPos + spriteSz;
		if (scanline >= spriteStartVerticalPos && scanline < spriteEndVerticalPos)
		{
			nbrSpritesInScanline++;
			if (x == 0 || x >= 168)
			{
				continue;
			}

			Tile tile = getTileById(idx, 0);
			auto tileImage = tile.getImage();
			int yOffsetTile = scanline - spriteStartVerticalPos;
			int xOffset = x - 8;
			for (int j = 0; j < 8; j++)
			{
				if (xOffset + j < 0)
				{
					continue;
				}
				if (xOffset + j > SCREEN_WIDTH)
				{
					break;
				}

				// A white pixel is considered as transparent, we are not copying it in that case
				if (!tileImage.isPixelWhite(j, yOffsetTile))
				{
					temporaryFrame.copyPixel(xOffset + j, scanline, tileImage, j, yOffsetTile);
				}
			}

			std::cout << "SPRITE " << (int)idx << "size=" << spriteSz << " display at (" << (int)x << "," << (int)y
			          << ")" << std::endl;
		}
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

Tile GPU::getTileById(byte tileId, int8_t tileSetId)
{
	int tileSetOffset = ADDR_TILE_SET_0;
	int tileIdCorrected = tileId;
	if (tileSetId == 1)
	{
		tileSetOffset = ADDR_TILE_SET_1;
		tileIdCorrected = static_cast<sbyte>(tileId);
	}

	uint16_t tileBaseAddr = tileSetOffset + Tile::BYTES_PER_TILE * tileIdCorrected;
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

bool GPU::isDisplayEnabled() const
{
	return utils::isNthBitSet(mmu.read(ADDR_LCD_GPU_CONTROL), 7);
}

int GPU::tileMapIndexForWindow() const
{
	return utils::isNthBitSet(mmu.read(ADDR_LCD_GPU_CONTROL), 6);
}

bool GPU::isWindowEnabled() const
{
	return utils::isNthBitSet(mmu.read(ADDR_LCD_GPU_CONTROL), 5);
}

int GPU::backgroundAndWindowTileDataAreaIndex() const
{
	return utils::isNthBitSet(mmu.read(ADDR_LCD_GPU_CONTROL), 4);
}

int GPU::backgroundTileMapIndex() const
{
	return utils::isNthBitSet(mmu.read(ADDR_LCD_GPU_CONTROL), 3);
}

int GPU::spriteSize() const
{
	return utils::isNthBitSet(mmu.read(ADDR_LCD_GPU_CONTROL), 2) ? 16 : 8;
}

bool GPU::areSpritesEnabled() const
{
	return utils::isNthBitSet(mmu.read(ADDR_LCD_GPU_CONTROL), 1);
}

bool GPU::areBackgroundAndWindowEnabled() const
{
	return utils::isNthBitSet(mmu.read(ADDR_LCD_GPU_CONTROL), 0);
}
