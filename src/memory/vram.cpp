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

    int tileBytesPerTile = isStacked ? StackedTile::BYTES_PER_TILE : SingleTile::BYTES_PER_TILE;
    Tile::TileDataArray dataArray = {};
    dataArray.resize(tileBytesPerTile);

    for (int i = 0; i < tileBytesPerTile; ++i)
    {
        dataArray[i] = readFromBank(static_cast<word>(tileBaseAddr + i), bankId);
    }

    if (isStacked)
    {
        return StackedTile(dataArray);
    }
    else
    {
        return SingleTile(dataArray);
    }
}