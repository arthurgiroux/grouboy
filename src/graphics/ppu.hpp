#ifndef GBEMULATOR_PPU_HPP
#define GBEMULATOR_PPU_HPP

#include "graphics/palette/grayscale_palette.hpp"
#include "memory/mmu.hpp"
#include "pixel.hpp"
#include "pixel_fifo.hpp"
#include "rgb_image.hpp"
#include "sprite.hpp"
#include "tile.hpp"
#include "tilemap.hpp"
#include <array>
#include <vector>

class InterruptManager;
class LCDStatusRegister;

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
     * Retrieve the tile map with the given index
     * @param index the id of the tilemap to use, possible values [0, 1]
     * @return  the Tile map
     */
    Tilemap getTileMap(int index);

    /**
     * Create a new PPU object.
     * @param mmu the MMU to use to access VRAM and OAM.
     * @param interruptManager the interrupt manager to use to raise graphical interrupts
     */
    PPU(MMU& mmu, InterruptManager* interruptManager);
    ~PPU();

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
    int windowTileMapIndex() const;

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
    sbyte backgroundAndWindowTileDataAreaIndex() const;

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
     * /!\ COLOR MODE ONLY /!\
     * Return if the objects will always be rendered on top the of background / window
     *
     * @return true if background and window never have priority, false otherwise
     */
    bool areBackgroundAndWindowDeprioritized() const;

    /**
     * Reset the PPU to its initial state.
     */
    void reset();

    /**
     * The screen width in pixels.
     */
    static const int SCREEN_WIDTH;

    /**
     * The screen height in pixels.
     */
    static const int SCREEN_HEIGHT;

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
    /**
     * Set the mode that the PPU is currently in.
     *
     * @param value the new mode of the PPU.
     */
    void setMode(Mode value);

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
     * Common logic to render either the background or window part of the given scanline.
     *
     * @param scanline  the index of the scanline to render, between [0, MAX_SCANLINE_VALUE]
     * @param scrollX   The scroll value to use for x coordinate
     * @param scrollY   The scroll value to use for y coordinate
     * @param tilemap   The tilemap to retrieve the tile
     * @param isWindow  Whether or not we are rendering the window or backgroung
     */
    void renderScanlineBackgroundOrWindow(int scanline, byte scrollX, byte scrollY, Tilemap& tilemap, bool isWindow);

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
     * Scan the OAM and retrieve the sprites that should be rendered for a specific scanline.
     * The sprites will be ordered by increasing priority so that a sprite with lower priority will
     * be overridden by the next one.
     *
     * @param scanline The scanline that the sprites will be rendered on
     * @return a list of sprites to render, ordered by increasing priority
     */
    std::vector<Sprite*> getSpritesThatShouldBeRendered(int scanline);

    void renderPixel();

    /**
     * The address of the tile map with index 0.
     */
    static constexpr word ADDR_MAP_0 = 0x9800;

    /**
     * The address of the tile map with index 1.
     */
    static constexpr word ADDR_MAP_1 = 0x9C00;

    /**
     * The address of the LCD control register.
     */
    static constexpr word ADDR_LCD_PPU_CONTROL = 0xFF40;

    /**
     * The address of the scroll-y register.
     */
    static constexpr word ADDR_SCROLL_Y = 0xFF42;

    /**
     * The address of the scroll-x register.
     */
    static constexpr word ADDR_SCROLL_X = 0xFF43;

    /**
     * The address where we store the information of the scanline being rendered.
     */
    static constexpr word ADDR_SCANLINE = 0xFF44;
    static constexpr word WINDOW_ADDR_SCROLL_Y = 0xFF4A;
    static constexpr word WINDOW_ADDR_SCROLL_X = 0xFF4B;

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
    byte _currentScanline = 0;

    /**
     * An internal line counter used to render the window.
     * It's incremented only when the window is rendered for a scanline and reset on VBLANK.
     * It's used instead of the scanlineId to access the tilemap while rendering the window.
     */
    int _windowLineCounter = 0;

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
    std::vector<std::unique_ptr<Sprite>> _sprites = std::vector<std::unique_ptr<Sprite>>(NBR_SPRITES);

    /**
     * The interrupt manager to use to raise graphical interrupts.
     */
    InterruptManager* _interruptManager = nullptr;

    /**
     * LCD Status register, used to exposed PPU state via memory.
     */
    std::unique_ptr<LCDStatusRegister> _lcdStatusRegister;

    /**
     * Was a LCD_STAT, LCY=LY interrupt already fired for this scanline
     */
    bool _LYCInterruptRaisedDuringScanline = false;

    /**
     * The address of the palette to use for background sprites.
     */
    static const word ADDR_PALETTE_BG = 0xFF47;

    /**
     * The address of the palette with index 0 to use for sprites (object rendering).
     */
    static const word ADDR_PALETTE_OBJ0 = 0x0FF48;

    /**
     * The address of the palette with index 1 to use for sprites (object rendering).
     */
    static const word ADDR_PALETTE_OBJ1 = 0x0FF49;

    /**
     * The palette to use for background sprites.
     */
    GrayscalePalette _paletteBackground;

    /**
     * The palette with index 0 to use for sprites (object rendering).
     */
    GrayscalePalette _paletteObj0;

    /**
     * The palette with index 1 to use for sprites (object rendering).
     */
    GrayscalePalette _paletteObj1;

    /**
     * The list of sprites that should be render for the current scanline
     */
    std::vector<Sprite*> _spritesToRender = {};

    /**
     * A map of the pixel x-coordinate and pixel to render for the current scanline.
     */
    std::map<int, Pixel> _scanline;

    PixelFIFO _backgroundWindowFIFO;

    PixelFIFO _spritesFIFO;
};

#endif // GBEMULATOR_PPU_HPP
