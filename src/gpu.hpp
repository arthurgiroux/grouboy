#ifndef GBEMULATOR_GPU_HPP
#define GBEMULATOR_GPU_HPP

#include "mmu.hpp"
#include "tile.hpp"
#include <array>
#include <vector>

class GPU
{
  public:
	static const int SCREEN_WIDTH = 160;
	static const int SCREEN_HEIGHT = 144;
    static const int BYTES_PER_PIXEL = 3;
	using FrameRGB = std::array<byte, SCREEN_WIDTH * SCREEN_HEIGHT * BYTES_PER_PIXEL>;

	static const int TILEMAP_HEIGHT = 32;
	static const int TILEMAP_WIDTH = 32;
	using TileMap = std::vector<Tile>;
	TileMap getTileMap(int index);

	GPU(MMU& mmu);
	~GPU() = default;

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

	const FrameRGB& getCurrentFrame() const
	{
		return currentFrame;
	}

	int getFrameId() const
	{
		return frameId;
	}

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
	void renderFrame();
	void updateParameters();

	int frameId = 0;
	int ticksSpentInCurrentMode = 0;
	Mode currentMode = OAM_ACCESS;
	int currentScanline = 0;

    static const int M_CYCLES_TO_T_CYCLES = 4;
    static const int OAM_ACCESS_TICKS = 80 * M_CYCLES_TO_T_CYCLES;
	static const int VRAM_ACCESS_TICKS = 172 * M_CYCLES_TO_T_CYCLES;
	static const int HBLANK_TICKS = 204 * M_CYCLES_TO_T_CYCLES;
	static const int VBLANK_TICKS = 456 * M_CYCLES_TO_T_CYCLES;
    static const int MAX_SCANLINE_VALUE = 153;

	MMU& mmu;

    FrameRGB temporaryFrame = {};
    FrameRGB currentFrame = {};

	static const int ADDR_MAP_0 = 0x9800;
	static const int ADDR_MAP_1 = 0x9C00;
	static const int ADDR_TILE_SET_0 = 0x8000;
	static const int ADDR_TILE_SET_1 = 0x9000;

	static const int ADDR_LCD_GPU_CONTROL = 0xFF40;
	static const int ADDR_SCROLL_Y = 0xFF42;
	static const int ADDR_SCROLL_X = 0xFF43;
	static const int ADDR_SCANLINE = 0xFF44;
	static const int ADDR_BG_PALETTE = 0xFF47;
	static const int TILES_PER_LINE = 20;
	static const int TILE_MAP_SIZE = 32;

	byte scrollY;
	byte scrollX;
	int backgroundPalette;

	bool paramBackgroundStatus;
	bool paramSpritesStatus;
	bool paramSpriteSize;
	bool useBackgroundTileMap1;
	bool paramBackgroundTileSet;
	bool paramWindowStatus;
	bool paramWindowTileMap;
	bool paramDisplayStatus;
};

#endif // GBEMULATOR_GPU_HPP
