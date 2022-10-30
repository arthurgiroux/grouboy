
#include "ppu.hpp"
#include <bitset>
#include <cassert>
#include <iostream>

void PPU::step(int nbrTicks)
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

void PPU::renderScanline(int scanline)
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

void PPU::renderScanlineBackground(int scanline)
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

void PPU::renderScanlineWindow(int scanline)
{
	// TODO: render window
}

void PPU::renderScanlineSprite(int scanline)
{
	int nbrSpritesInScanline = 0;

	std::vector<Sprite*> spritesToRender = {};
	/*
	 * In order to find which sprites to render, we need to check the coordinate of every sprites.
	 * There's a maximum number of sprites that can be rendered per scanline.
	 */
	for (int i = 0; i < NBR_SPRITES && nbrSpritesInScanline < MAX_NBR_SPRITES_PER_SCANLINE; ++i)
	{
		auto& sprite = _sprites[i];
		int spriteSz = spriteSize();
		int spriteStartVerticalPos = sprite->getYPositionOnScreen();
		int spriteEndVerticalPos = spriteStartVerticalPos + spriteSz;
		if (scanline >= spriteStartVerticalPos && scanline < spriteEndVerticalPos)
		{
			nbrSpritesInScanline++;
			/*
			 * If the sprite is outside the screen bound then it's not going to be effectively rendered
			 * but it still counts as if it was rendered.
			 */
			if (sprite->getXPositionOnScreen() < 0 || sprite->getXPositionOnScreen() >= PPU::SCREEN_WIDTH)
			{
				continue;
			}

			spritesToRender.push_back(sprite.get());
		}
	}

	// TODO: Implement sprite ordering

	for (auto* sprite : spritesToRender)
	{
		Tile tile = getTileById(sprite->getTileId(), 0);
		auto tileImage = tile.getImage();

		int yCoordinateInTile = scanline - sprite->getYPositionOnScreen();
		if (sprite->isFlippedVertically())
		{
			yCoordinateInTile = Tile::TILE_HEIGHT - 1 - yCoordinateInTile;
		}

		for (int xOffset = 0; xOffset < Tile::TILE_WIDTH; xOffset++)
		{
			int xCoordinateOnScreen = sprite->getXPositionOnScreen() + xOffset;
			// If we are outside the screen on the left we just skip the current pixel
			if (xCoordinateOnScreen < 0)
			{
				continue;
			}

			// If we are outside the screen on the right we can stop rendering the tile
			if (xCoordinateOnScreen > SCREEN_WIDTH)
			{
				break;
			}

			int xCoordinateInTile = xOffset;
			if (sprite->isFlippedHorizontally())
			{
				xCoordinateInTile = Tile::TILE_WIDTH - 1 - xCoordinateInTile;
			}

			// We are copying the pixel if it's not white, white is treated as transparent
			if (!tileImage.isPixelWhite(xCoordinateInTile, yCoordinateInTile))
			{
				temporaryFrame.copyPixel(xCoordinateOnScreen, scanline, tileImage, xCoordinateInTile,
				                         yCoordinateInTile);
			}
		}
	}
}

void PPU::renderFrame()
{
	currentFrame = temporaryFrame;
	frameId++;
}

PPU::PPU(MMU& mmu_) : mmu(mmu_)
{
	for (int i = 0; i < _sprites.size(); ++i)
	{
		_sprites[i] = std::make_unique<Sprite>(mmu, i);
	}
}

Tile PPU::getTileById(byte tileId, int8_t tileSetId)
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

PPU::TileMap PPU::getTileMap(int index)
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

bool PPU::isDisplayEnabled() const
{
	return utils::isNthBitSet(mmu.read(ADDR_LCD_PPU_CONTROL), 7);
}

int PPU::tileMapIndexForWindow() const
{
	return utils::isNthBitSet(mmu.read(ADDR_LCD_PPU_CONTROL), 6);
}

bool PPU::isWindowEnabled() const
{
	return utils::isNthBitSet(mmu.read(ADDR_LCD_PPU_CONTROL), 5);
}

int PPU::backgroundAndWindowTileDataAreaIndex() const
{
	return utils::isNthBitSet(mmu.read(ADDR_LCD_PPU_CONTROL), 4);
}

int PPU::backgroundTileMapIndex() const
{
	return utils::isNthBitSet(mmu.read(ADDR_LCD_PPU_CONTROL), 3);
}

int PPU::spriteSize() const
{
	return utils::isNthBitSet(mmu.read(ADDR_LCD_PPU_CONTROL), 2) ? 16 : 8;
}

bool PPU::areSpritesEnabled() const
{
	return utils::isNthBitSet(mmu.read(ADDR_LCD_PPU_CONTROL), 1);
}

bool PPU::areBackgroundAndWindowEnabled() const
{
	return utils::isNthBitSet(mmu.read(ADDR_LCD_PPU_CONTROL), 0);
}
