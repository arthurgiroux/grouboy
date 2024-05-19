#include "vram.hpp"

Tile VRAM::getTileById(byte tileId, sbyte tileSetId, bool isStacked)
{
    return getTileById(tileId, tileSetId, getBankId(), isStacked);
}

Tile VRAM::getTileById(byte tileId, sbyte tileSetId, unsigned int bankId, bool isStacked)
{
    word tileSetOffset = ADDR_TILE_SET_0;
    int tileIdCorrected = tileId;
    if (tileSetId == 1)
    {
        tileSetOffset = ADDR_TILE_SET_1;
        tileIdCorrected = static_cast<sbyte>(tileId);
    }

    word tileBaseAddr = static_cast<word>(tileSetOffset + SingleTile::BYTES_PER_TILE * tileIdCorrected);

    if (isStacked)
    {
        return StackedTile(this, bankId, tileBaseAddr);
    }
    else
    {
        return SingleTile(this, bankId, tileBaseAddr);
    }
}