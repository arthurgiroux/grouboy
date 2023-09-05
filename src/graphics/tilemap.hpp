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
     * Create a new tilemap from a given memory address.
     *
     * @param mmu   The MMU to use to access the memory
     * @param tilemapAddr   The address where the tilemap data is stored
     */
    Tilemap(MMU* mmu, word tilemapAddr);
    ~Tilemap() = default;

    /**
     * Retrieve the tile index for a given entry in the tilemap
     *
     * @param index the index inside the tilemap, between 0 and WIDTH*HEIGHT
     * @return a signed tile id
     */
    sbyte getTileIdForIndex(int index) const;

    /**
     * Retrieve the tile index for a given entry in the tilemap, using xy coordinate
     *
     * @param x The x coordinate inside the tilemap, between 0 and WIDTH
     * @param y The y coordinate inside the tilemap, between 0 and HEIGHT
     * @return a signed tile id
     */
    int getTileIdForCoord(int x, int y) const;

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
     * The array where we store the tile id for each entry in the tilemap
     */
    std::array<sbyte, HEIGHT* WIDTH> _tileIds = {};

    /**
     * The MMU to use to access the memory.
     */
    MMU* _mmu = nullptr;
};

#endif // GBEMULATOR_TILEMAP_HPP
