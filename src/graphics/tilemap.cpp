#include "tilemap.hpp"

Tilemap::Tilemap(VRAM* vram, word tilemapAddr) : _vram(vram), _address(tilemapAddr)
{
}

sbyte Tilemap::getTileIdForIndex(int index) const
{
    return static_cast<sbyte>(_vram->readFromBank(_vram->addressRange.relative(_address + index), 0));
}

Tilemap::TileInfo Tilemap::getTileInfoForIndex(int index) const
{
    byte attr = _vram->readFromBank(_vram->addressRange.relative(_address + index), 1);
    return TileInfo(attr);
}

Tilemap::TileInfo::TileInfo(int attributes)
{
    _colorPaletteId = attributes & 0x07;
    _vramBankId = (attributes >> 3) & 0x01;
    _isFlippedHorizontally = utils::isNthBitSet(attributes, 5);
    _isFlippedVertically = utils::isNthBitSet(attributes, 6);
    _isRenderedAboveSprites = utils::isNthBitSet(attributes, 7);
}

bool Tilemap::TileInfo::isFlippedHorizontally() const
{
    return _isFlippedHorizontally;
}

bool Tilemap::TileInfo::isFlippedVertically() const
{
    return _isFlippedVertically;
}

int Tilemap::TileInfo::getVRAMBankId() const
{
    return _vramBankId;
}

int Tilemap::TileInfo::getColorPaletteId() const
{
    return _colorPaletteId;
}

bool Tilemap::TileInfo::isRenderedAboveSprites() const
{
    return _isRenderedAboveSprites;
}
