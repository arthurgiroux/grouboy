#ifndef GROUBOY_PIXEL_FIFO_RENDERER_HPP
#define GROUBOY_PIXEL_FIFO_RENDERER_HPP

#include "background_window_pixel_fetcher.hpp"
#include "pixel_fifo.hpp"
#include "sprite_pixel_fetcher.hpp"
#include <set>
#include <vector>

class MMU;
class PPU;
class Sprite;

class PixelFifoRenderer
{
  public:
    PixelFifoRenderer(MMU* mmu, PPU* ppu);
    ~PixelFifoRenderer() = default;
    void step();
    void reset();
    int getX() const;

    /**
     * Set the list of sprites to render for the current scanline.
     * @param sprites Vector of sprite pointers, sorted by priority (lowest priority first)
     */
    void setSpritesToRender(const std::vector<Sprite*>& sprites);

    /**
     * Get a reference to the background/window FIFO.
     */
    PixelFIFO& getBackgroundWindowFIFO();

    /**
     * Get a reference to the OAM (sprite) FIFO.
     */
    PixelFIFO& getOAMFIFO();

    /**
     * Check if the window was rendered during this scanline.
     * Used to determine if the window line counter should be incremented.
     * @return true if window was triggered on this scanline
     */
    bool wasWindowTriggeredThisScanline() const;

  private:
    /**
     * Check if there's a sprite that needs to be fetched at the current X position.
     * @return Pointer to the sprite to fetch, or nullptr if none
     */
    Sprite* checkForSpriteHit();

    /**
     * Check if the window should start rendering at the current position.
     * Window starts when:
     * - Window is enabled (LCDC bit 5)
     * - Current scanline >= WY
     * - Current X >= WX - 7
     * @return true if window should start
     */
    bool checkForWindowTrigger();

    /**
     * Start rendering the window - clears FIFO and switches fetcher to window mode.
     */
    void startWindowRendering();

    /**
     * Mix a pixel from the BG FIFO with the corresponding OAM FIFO pixel
     * and determine which one should be rendered.
     * @return The pixel that should be rendered
     */
    Pixel mixPixels();

    MMU* _mmu;
    PPU* _ppu;
    BackgroundWindowPixelFetcher _bgWindowPixelFetcher;
    SpritePixelFetcher _spritePixelFetcher;
    PixelFIFO _backgroundWindowFIFO;
    PixelFIFO _oamFIFO; // Separate FIFO for sprite pixels
    int _x = 0;

    // Sprites to render for the current scanline
    std::vector<Sprite*> _spritesToRender;

    // Track which sprites have already been fetched (by their OAM index)
    std::set<int> _fetchedSpriteIds;

    // Is the background fetcher paused while fetching a sprite?
    bool _bgFetcherPaused = false;

    // Is the window currently being rendered on this scanline?
    bool _windowActive = false;

    // Has the window been triggered at all during this scanline?
    // This is needed because the window can be shown, hidden (by moving WX off-screen),
    // and shown again within the same frame
    bool _windowTriggeredThisScanline = false;

    // Number of pixels to discard at the start of the scanline (SCX % 8)
    // Per Pan Docs: "At the start of each scanline, pixels from the first fetched
    // BG tile equal to SCX mod 8 are discarded"
    int _pixelsToDiscard = 0;
};

#endif // GROUBOY_PIXEL_FIFO_RENDERER_HPP
