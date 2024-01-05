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
    class TileInfo
    {
      public:
        TileInfo(int attributes);

        bool isFlippedHorizontally() const;
        bool isFlippedVertically() const;
        int getVRAMBankId() const;
        int getColorPaletteId() const;
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
