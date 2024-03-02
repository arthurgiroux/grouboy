#ifndef GBEMULATOR_TILEMAP_HPP
#define GBEMULATOR_TILEMAP_HPP

#include "memory/mmu.hpp"
#include <array>

/**
 * A tilemap is a 32x32 map of tiles.
 * The tilemaps are used when rendering the background and window.
 */
class Tilemap
{
  public:
    /**
     * On GameBoy Color, each tile in the tilemap is linked to an attribute map,
     * where attributes about how this tile should be rendered are stored.
     */
    class TileInfo
    {
      public:
        /**
         * Create the tile info from the attribute value
         * @param attributes the value from the attribute memory value
         */
        TileInfo(int attributes);

        /**
         * Should the tile be rendered from right to left
         * @return true if it should be flipped, false otherwise
         */
        bool isFlippedHorizontally() const;

        /**
         * Should the tile be rendered upside-down
         * @return true if it should be flipped, false otherwise
         */
        bool isFlippedVertically() const;

        /**
         * Get the VRAM Bank from which the tile should be retrieve
         * @return a bank id, 0 or 1
         */
        int getVRAMBankId() const;

        /**
         * Get the id of the color palette that should be used to render the tile
         * @return an id between 0 and 7
         */
        int getColorPaletteId() const;

        /**
         * Should this tile have highest priority and be rendered above everything
         * @return true if it should have highest priority, false otherwise
         */
        bool isRenderedAboveSprites() const;

      private:
        bool _isFlippedHorizontally = false;
        bool _isFlippedVertically = false;
        bool _isRenderedAboveSprites = false;
        int _vramBankId = 0;
        int _colorPaletteId = 0;
    };

    /**
     * Create a new tilemap from a given memory address.
     *
     * @param vram   The VRAM to use to access the memory
     * @param tilemapAddr   The address where the tilemap data is stored
     */
    Tilemap(VRAM* vram, word tilemapAddr);
    ~Tilemap() = default;

    /**
     * Retrieve the tile index for a given entry in the tilemap
     *
     * @param index the index inside the tilemap, between 0 and WIDTH*HEIGHT
     * @return a signed tile id
     */
    sbyte getTileIdForIndex(int index) const;
    TileInfo getTileInfoForIndex(int index) const;

    /**
     * The tilemap height in pixels.
     */
    static const int HEIGHT = 32;

    /**
     * The tilemap width in pixels.
     */
    static const int WIDTH = 32;

  private:
    /**
     * The VRAM to use to access the memory.
     */
    VRAM* _vram = nullptr;

    word _address = 0;
};

#endif // GBEMULATOR_TILEMAP_HPP
