#include "tilemap.hpp"

Tilemap::Tilemap(MMU* mmu, word tilemapAddr) : _mmu(mmu)
{
    for (int i = 0; i < HEIGHT * WIDTH; ++i)
    {
        sbyte tileId = _mmu->read(tilemapAddr + i);
        _tileIds[i] = tileId;
    }
}

int Tilemap::getTileIdForIndex(int index) const
{
    return _tileIds[index];
}

int Tilemap::getTileIdForCoord(int x, int y) const
{
    return _tileIds[y * Tilemap::WIDTH + x];
}