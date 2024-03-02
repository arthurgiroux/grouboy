#ifndef GROUBOY_VRAM_HPP
#define GROUBOY_VRAM_HPP

#include "common/utils.hpp"
#include "graphics/tile.hpp"
#include "switchable_memory_bank.hpp"

class VRAM : public SwitchableMemoryBank<2, 8_KiB>
{
  public:
    /**
     * Retrieve a tile with a given index from a given tile set.
     *
     * @param tileId The index of the tile to retrieve
     * @param tileSetId The tile set to use
     * @param isStacked Is the tile we want to retrieve a stacked 8x16 tile
     * @return The requested tile
     */
    Tile getTileById(byte tileId, sbyte tileSetId, unsigned int bankId, bool isStacked = false);
    Tile getTileById(byte tileId, sbyte tileSetId, bool isStacked = false);

    const utils::AddressRange addressRange = utils::AddressRange(0x8000, 0x9FFF);

  private:
    /**
     * The address of the tile set with index 0.
     */
    static constexpr word ADDR_TILE_SET_0 = 0x0000;

    /**
     * The address of the tile set with index 1.
     */
    static constexpr word ADDR_TILE_SET_1 = 0x1000;
};

#endif // GROUBOY_VRAM_HPP
