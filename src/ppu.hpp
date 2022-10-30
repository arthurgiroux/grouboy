#ifndef GBEMULATOR_PPU_HPP
#define GBEMULATOR_PPU_HPP

#include "mmu.hpp"
#include "rgb_image.hpp"
#include "sprite.hpp"
#include "tile.hpp"
#include <array>
#include <vector>

class PPU
{
  public:
	static const int SCREEN_WIDTH = 160;
	static const int SCREEN_HEIGHT = 144;
	static const int TILEMAP_HEIGHT = 32;
	static const int TILEMAP_WIDTH = 32;
	using TileMap = std::vector<Tile>;
	TileMap getTileMap(int index);

	PPU(MMU& mmu);
	~PPU() = default;

	enum Mode
	{
		OAM_ACCESS,
		VRAM_ACCESS,
		HBLANK,
		VBLANK
	};

	void step(int nbrTicks);

	Mode getMode() const
	{
		return currentMode;
	}

	int getCurrentScanline() const
	{
		return currentScanline;
	}

	Tile getTileById(byte tileId, int8_t tileSetId);

	const RGBImage& getCurrentFrame() const
	{
		return currentFrame;
	}

	int getFrameId() const
	{
		return frameId;
	}

	/**
	 * Return if the display is enabled in the LCD control
	 *
	 * @return true if the display is enabled, false otherwise
	 */
	bool isDisplayEnabled() const;

	/**
	 * Return the index of the tilemap to use for tile in the "window"
	 *
	 * @return index of the tilemap to use
	 */
	int tileMapIndexForWindow() const;

	/**
	 * Return if the "window" is enabled in the LCD control
	 *
	 * @return true if "window" is enabled, false otherwise
	 */
	bool isWindowEnabled() const;

	/**
	 * Return the index of the tile data area to use for background and window
	 *
	 * @return index of the tile data area to use
	 */
	int backgroundAndWindowTileDataAreaIndex() const;

	/**
	 * Return the index of the tile map to use for the background
	 *
	 * @return index of the tile map to use
	 */
	int backgroundTileMapIndex() const;

	/**
	 * Return the size of the sprites to use
	 *
	 * @return 8 for 8x8, 16 for 8x16
	 */
	int spriteSize() const;

	/**
	 * Return if sprite rendering is enabled in the LCD control
	 *
	 * @return true if sprite rendering is enabled, false otherwise
	 */
	bool areSpritesEnabled() const;

	/**
	 * Return if background and window rendering are enabled in the LCD control
	 *
	 * @return true if background and window rendering are enabled, false otherwise
	 */
	bool areBackgroundAndWindowEnabled() const;

#ifndef UNIT_TESTING
  private:
#endif
	void setMode(Mode value)
	{
		currentMode = value;
		// TODO: Check if we should take into account modulo of ticks
		ticksSpentInCurrentMode = 0;
	};

	void renderScanline(int scanline);

	/**
	 * Render the background part of the given scanline
	 * @param scanline 	the scanline to render
	 */
	void renderScanlineBackground(int scanline);

	/**
	 * Render the window part of the given scanline
	 * @param scanline 	the scanline to render
	 */
	void renderScanlineWindow(int scanline);

	/**
	 * Render the sprite part of the given scanline
	 * @param scanline 	the scanline to render
	 */
	void renderScanlineSprite(int scanline);

	void renderFrame();
	void updateParameters();

	int frameId = 0;
	int ticksSpentInCurrentMode = 0;
	Mode currentMode = OAM_ACCESS;
	int currentScanline = 0;

	static const int OAM_ACCESS_TICKS = 80;
	static const int VRAM_ACCESS_TICKS = 172;
	static const int HBLANK_TICKS = 204;
	static const int VBLANK_TICKS = 456;
	static const int MAX_SCANLINE_VALUE = 153;

	MMU& mmu;

	RGBImage temporaryFrame = RGBImage(SCREEN_HEIGHT, SCREEN_WIDTH);
	RGBImage currentFrame = RGBImage(SCREEN_HEIGHT, SCREEN_WIDTH);

	static const int ADDR_MAP_0 = 0x9800;
	static const int ADDR_MAP_1 = 0x9C00;
	static const int ADDR_TILE_SET_0 = 0x8000;
	static const int ADDR_TILE_SET_1 = 0x9000;

	static const int ADDR_LCD_PPU_CONTROL = 0xFF40;
	static const int ADDR_SCROLL_Y = 0xFF42;
	static const int ADDR_SCROLL_X = 0xFF43;
	static const int ADDR_SCANLINE = 0xFF44;
	static const int ADDR_BG_PALETTE = 0xFF47;
	static const int TILES_PER_LINE = 20;
	static const int TILE_MAP_SIZE = 32;
	static const int WINDOW_ADDR_SCROLL_Y = 0xFF4A;
	static const int WINDOW_ADDR_SCROLL_X = 0xFF4B;
	static const int NBR_SPRITES = 40;
	static const int MAX_NBR_SPRITES_PER_SCANLINE = 10;
	std::array<std::unique_ptr<Sprite>, NBR_SPRITES> _sprites;
};

#endif // GBEMULATOR_PPU_HPP
