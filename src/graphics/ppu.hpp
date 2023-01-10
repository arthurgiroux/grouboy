#ifndef GBEMULATOR_PPU_HPP
#define GBEMULATOR_PPU_HPP

#include "memory/mmu.hpp"
#include "palette.hpp"
#include "rgb_image.hpp"
#include "sprite.hpp"
#include "tile.hpp"
#include <array>
#include <vector>

class InterruptManager;

/**
 * This class is responsible for rendering the frame that will be displayed by the screen.
 * The rendering of the GameBoy is done per scanline and is tied to the CPU timing.
 * The PPU renders an image of 160x144 pixels.
 *
 * The rendering works as follows:
 *      For the 144 lines, the PPU will first block the OAM to read then content, then block the
 *      VRAM and render the scanline at the end of the Horizontal Blank.
 *      When all the 144 lines are rendered, a Vertical Blank is triggered and the image is ready.
 */
class PPU
{
  public:
    /**
     * A tile map is a 32x32 map of tiles.
     */
    using TileMap = std::vector<Tile>;

    /**
     * Retrieve the tile map with the given index
     * @param index the id of the tilemap to use, possible values [0, 1]
     * @param tileSetId the id of the tile set to use, possible values [0, 1]
     * @return  the Tile map
     */
    TileMap getTileMap(int index, int tileSetId);

    /**
     * Create a new PPU object.
     * @param mmu the MMU to use to access VRAM and OAM.
     * @param interruptManager the interrupt manager to use to raise graphical interrupts
     */
    PPU(MMU& mmu, InterruptManager* interruptManager);
    ~PPU() = default;

    /**
     * The different modes that the PPU can be in.
     */
    enum Mode
    {
        /**
         * The PPU is accessing the content of the OAM and the CPU shouldn't access it.
         */
        OAM_ACCESS,
        /**
         * The PPU is accessing the content of the VRAM and the OAM and the CPU shouldn't access them.
         */
        VRAM_ACCESS,
        /**
         * Horizontal blank, a line will be rendered.
         */
        HBLANK,
        /**
         * Vertical blank, all lines have been rendered and the frame is ready.
         */
        VBLANK
    };

    /**
     * Make the PPU work for a certain number of ticks.
     *
     * @param nbrTicks  The number of CPU ticks the PPU should work for.
     */
    void step(int nbrTicks);

    /**
     * Get the current mode of the PPU.
     *
     * @return the current PPU mode.
     */
    Mode getMode() const
    {
        return _currentMode;
    }

    /**
     * Get the scanline that is currently rendered.
     *
     * @return an int between 0 and MAX_SCANLINE_VALUE
     */
    int getCurrentScanline() const
    {
        return _currentScanline;
    }

    /**
     * Retrieve a tile with a given index from a given tile set.
     *
     * @param tileId The index of the tile to retrieve
     * @param tileSetId The tile set to use
     * @return The requested tile
     */
    Tile getTileById(byte tileId, int8_t tileSetId);

    /**
     * Get a reference to the last rendered frame.
     *
     * @return The frame.
     */
    const RGBImage& getLastRenderedFrame() const
    {
        return _lastRenderedFrame;
    }

    /**
     * Retrieve the id of the frame being rendered.
     *
     * @return a 0-based incremental id.
     */
    int getFrameId() const
    {
        return _frameId;
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

    /**
     * Reset the PPU to its initial state.
     */
    void reset();

    /**
     * The screen width in pixels.
     */
    static const int SCREEN_WIDTH = 160;

    /**
     * The screen height in pixels.
     */
    static const int SCREEN_HEIGHT = 144;

    /**
     * The tile map height in pixels.
     */
    // TODO: Move this to tilemap class
    static const int TILEMAP_HEIGHT = 32;

    /**
     * The tile map width in pixels.
     */
    // TODO: Move this to tilemap class
    static const int TILEMAP_WIDTH = 32;

    /**
     * Number of ticks to spend in OAM Access mode.
     */
    static const int OAM_ACCESS_TICKS = 80;

    /**
     * Number of ticks to spend in VRAM Access mode.
     */
    static const int VRAM_ACCESS_TICKS = 172;

    /**
     * Number of ticks to spend in Horizontal Blank mode.
     */
    static const int HBLANK_TICKS = 204;

    /**
     * Number of ticks to spend in Vertical Blank mode.
     */
    static const int VBLANK_TICKS = 456;

    /**
     * Number of ticks to spend in Horizontal Blank mode.
     */
    static const int MAX_SCANLINE_VALUE = 153;

  private:
    // TODO: Clean-up and document
    class LCDStatusRegister
    {
      public:
        LCDStatusRegister(MMU& mmu) : _mmu(mmu){};
        void updateFlagMode(Mode value)
        {
            int status = _mmu.read(ADDR_LCD_STATUS) & 0b11111100;
            if (value == HBLANK)
            {
                status |= 0x00;
            }
            else if (value == VBLANK)
            {
                status |= 0x01;
            }
            else if (value == OAM_ACCESS)
            {
                status |= 0x02;
            }

            else if (value == VRAM_ACCESS)
            {
                status |= 0x03;
            }

            _mmu.write(ADDR_LCD_STATUS, status);
        }

        bool areLYCAndLYEqual()
        {
            return _mmu.read(LY_COMPARE_ADDR) == _mmu.read(ADDR_SCANLINE);
        }

        void setLYCompareFlag(bool value)
        {
            int status = _mmu.read(ADDR_LCD_STATUS);
            utils::setNthBit(status, 2, value);
            _mmu.write(ADDR_LCD_STATUS, status);
        }

        bool isLYCompareStatInterruptEnabled()
        {
            return utils::isNthBitSet(_mmu.read(ADDR_LCD_STATUS), 6);
        }

        bool isOAMStatInterruptEnabled()
        {
            return utils::isNthBitSet(_mmu.read(ADDR_LCD_STATUS), 5);
        }

        bool isVBLANKStatInterruptEnabled()
        {
            return utils::isNthBitSet(_mmu.read(ADDR_LCD_STATUS), 4);
        }

        bool isHBLANKStatInterruptEnabled()
        {
            return utils::isNthBitSet(_mmu.read(ADDR_LCD_STATUS), 3);
        }

      private:
        MMU& _mmu;
        static const int LY_COMPARE_ADDR = 0xFF45;

        /**
         * The address of the LCD status register.
         */
        static const int ADDR_LCD_STATUS = 0xFF41;
    };
    /**
     * Set the mode that the PPU is currently in.
     *
     * @param value the new mode of the PPU.
     */
    void setMode(Mode value)
    {
        _lcdStatusRegister.updateFlagMode(value);
        _currentMode = value;
        // TODO: Check if we should take into account modulo of ticks
        _ticksSpentInCurrentMode = 0;
    };

    /**
     * Render the given scanline.
     *
     * @param scanline the index of the scanline to render, between [0, MAX_SCANLINE_VALUE]
     */
    void renderScanline(int scanline);

    /**
     * Render the background part of the given scanline.
     *
     * @param scanline the index of the scanline to render, between [0, MAX_SCANLINE_VALUE]
     */
    void renderScanlineBackground(int scanline);

    /**
     * Render the window part of the given scanline.
     *
     * @param scanline the index of the scanline to render, between [0, MAX_SCANLINE_VALUE]
     */
    void renderScanlineWindow(int scanline);

    /**
     * Render the sprite part of the given scanline.
     *
     * @param scanline the index of the scanline to render, between [0, MAX_SCANLINE_VALUE]
     */
    void renderScanlineSprite(int scanline);

    /**
     * Swap the frame buffers, this will "freeze" the frame being rendered and make it
     * available for being displayed.
     * A new frame can then be rendered in the background.
     */
    void swapFrameBuffers();

    /**
     * The address of the tile map with index 0.
     */
    static const int ADDR_MAP_0 = 0x9800;

    /**
     * The address of the tile map with index 1.
     */
    static const int ADDR_MAP_1 = 0x9C00;

    /**
     * The address of the tile set with index 0.
     */
    static const int ADDR_TILE_SET_0 = 0x8000;

    /**
     * The address of the tile set with index 1.
     */
    static const int ADDR_TILE_SET_1 = 0x9000;

    /**
     * The address of the LCD control register.
     */
    static const int ADDR_LCD_PPU_CONTROL = 0xFF40;

    /**
     * The address of the scroll-y register.
     */
    static const int ADDR_SCROLL_Y = 0xFF42;

    /**
     * The address of the scroll-x register.
     */
    static const int ADDR_SCROLL_X = 0xFF43;

    /**
     * The address where we store the information of the scanline being rendered.
     */
    static const int ADDR_SCANLINE = 0xFF44;
    static const int TILES_PER_LINE = 20;
    static const int TILE_MAP_SIZE = 32;
    static const int WINDOW_ADDR_SCROLL_Y = 0xFF4A;
    static const int WINDOW_ADDR_SCROLL_X = 0xFF4B;

    /**
     * The number of sprites in the OAM.
     */
    static const int NBR_SPRITES = 40;

    /**
     * The maximum number of sprites that can be rendered for 1 scanline.
     */
    static const int MAX_NBR_SPRITES_PER_SCANLINE = 10;

    /**
     * The current frame id, incremented every frame.
     */
    int _frameId = 0;

    /**
     * Number of ticks already spent in the current mode.
     */
    int _ticksSpentInCurrentMode = 0;

    /**
     * Current mode of the PPU.
     */
    Mode _currentMode = OAM_ACCESS;

    /**
     * Scanline currently being rendered.
     */
    int _currentScanline = 0;

    /**
     * The MMU to use to access VRAM and OAM.
     */
    MMU& _mmu;

    /**
     * The frame currently being rendered.
     */
    RGBImage _temporaryFrame = RGBImage(SCREEN_HEIGHT, SCREEN_WIDTH);

    /**
     * The last frame that was fully rendered.
     */
    RGBImage _lastRenderedFrame = RGBImage(SCREEN_HEIGHT, SCREEN_WIDTH);

    /**
     * The sprites in the OAM.
     */
    std::array<std::unique_ptr<Sprite>, NBR_SPRITES> _sprites;

    /**
     * The interrupt manager to use to raise graphical interrupts.
     */
    InterruptManager* _interruptManager = nullptr;

    /**
     * LCD Status register, used to exposed PPU state via memory.
     */
    LCDStatusRegister _lcdStatusRegister = LCDStatusRegister(_mmu);

    /**
     * Was a LCD_STAT, LCY=LY interrupt already fired for this scanline
     */
    bool _LYCInterruptRaisedDuringScanline = false;

    /**
     * The address of the palette to use for background sprites.
     */
    static const int ADDR_PALETTE_BG = 0xFF47;

    /**
     * The address of the palette with index 0 to use for sprites (object rendering).
     */
    static const int ADDR_PALETTE_OBJ0 = 0x0FF48;

    /**
     * The address of the palette with index 1 to use for sprites (object rendering).
     */
    static const int ADDR_PALETTE_OBJ1 = 0x0FF49;

    /**
     * The palette to use for background sprites.
     */
    Palette _paletteBackground;

    /**
     * The palette with index 0 to use for sprites (object rendering).
     */
    Palette _paletteObj0;

    /**
     * The palette with index 1 to use for sprites (object rendering).
     */
    Palette _paletteObj1;
};

#endif // GBEMULATOR_PPU_HPP
