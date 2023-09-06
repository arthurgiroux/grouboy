#ifndef GBEMULATOR_SPRITE_HPP
#define GBEMULATOR_SPRITE_HPP

#include "memory/mmu.hpp"

/**
 * Represents a graphical sprite, sprites are displayed on top of the background,
 * and the window by the PPU.
 * Sprites have a coordinate (that can be off-screen) and a tile index.
 * All information about the sprite will be fetched from memory, based on the id of the sprite.
 */
class Sprite
{
  public:
    /**
     * Create a sprite object for a specific id.
     *
     * @param mmu	reference to the MMU, information about the sprite will be fetched from it
     * @param spriteId	the id of the sprite
     */
    Sprite(MMU& mmu, int spriteId);
    ~Sprite() = default;

    /**
     * Get the ID of the sprite.
     *
     * @return	the id of the sprite
     */
    int getId() const;

    /**
     * Get the X position of the sprite on the screen.
     * This coordinate can be outside of what the screen can
     * actually render (less than 0 or bigger than screen width).
     * In that case the sprite can be fully or partially hidden.
     *
     * @return	the x position of the sprite on screen
     */
    int getXPositionOnScreen() const;

    /**
     * Get the Y position of the sprite on the screen.
     * This coordinate can be outside of what the screen can
     * actually render (less than 0 or bigger than screen height).
     * In that case the sprite can be fully or partially hidden.
     *
     * @return the y position of the sprite on screen
     */
    int getYPositionOnScreen() const;

    /**
     * Get the id of the tile that should be rendered for the sprite.
     *
     * @return 	the id of the tile to render from the tilemap
     */
    byte getTileId() const;

    /**
     * Should the tile be flipped vertically when rendering this sprite.
     *
     * @return True if the tile should be flipped vertically, false otherwise.
     */
    bool isFlippedVertically() const;

    /**
     * Should the tile be flipped horizontally when rendering this sprite.
     *
     * @return True if the tile should be flipped horizontally, false otherwise.
     */
    bool isFlippedHorizontally() const;

    /**
     * Should the opaque pixels of the tile be rendered over the background and window.
     *
     * @return True if tile should be rendered over the other layers, false otherwise.
     */
    bool isRenderedOverBackgroundAndWindow() const;

    /**
     * Compare the render priority of the current sprite with another one.
     *
     * @param other the sprite priority to compare it
     * @return True if the sprite priority is bigger than the other one, false otherwise
     */
    bool isPriorityBiggerThanOtherSprite(const Sprite& other) const;

    /**
     * Get the id of the palette to use to render the Sprite.
     *
     * @return  either 0 or 1, for the palette id.
     */
    int getPaletteId() const;

  private:
    /**
     * Read a certain data from the payload containing sprite information.
     *
     * @param idx	the index of the data to read
     * @return	the data read from the payload
     */
    byte readDataFromPayload(int idx) const;

    static const int SPRITE_ATTR_TABLE_ADDR = 0xFE00;
    static const int PAYLOAD_PER_SPRITE = 4;
    static const int PAYLOAD_DATA_Y_IDX = 0;
    static const int PAYLOAD_DATA_X_IDX = 1;
    static const int PAYLOAD_DATA_TILEID_IDX = 2;
    static const int PAYLOAD_DATA_FLAG_ATTR_IDX = 3;
    static const int Y_SCREEN_OFFSET = 16;
    static const int X_SCREEN_OFFSET = 8;
    static const int DATA_FLAG_BIT_BGWINDOW_OVER_OBJ = 7;
    static const int DATA_FLAG_BIT_VFLIP = 6;
    static const int DATA_FLAG_BIT_HFLIP = 5;
    static const int DATA_FLAG_BIT_PALETTEID = 4;

    MMU& _mmu;
    int _id;
};

#endif // GBEMULATOR_SPRITE_HPP
