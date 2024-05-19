#include "tile.hpp"
#include "memory/vram.hpp"

Tile::Tile(int height, int width, VRAM* vram, int bankId, word startAddr)
    : _height(height), _width(width), _vram(vram), _bankId(bankId), _startAddr(startAddr)
{
}

int Tile::getHeight() const
{
    return _height;
}

int Tile::getWidth() const
{
    return _width;
}

void Tile::lazyLoadTileData(int line)
{
    if (!_data.count(line))
    {
        byte msb = _vram->readFromBank(static_cast<word>(_startAddr + line * BYTES_PER_TILE_VALUE + 1), _bankId);
        byte lsb = _vram->readFromBank(static_cast<word>(_startAddr + line * BYTES_PER_TILE_VALUE), _bankId);
        _data[line] = (msb << 8 | lsb);
    }
}

byte Tile::getColorData(int x, int y)
{
    // We load tile data if needed
    lazyLoadTileData(y);

    /*
     * The color of a pixel for a tile is encoded on 2 bits of two adjacent bytes.
     * Example:
     *      [0x803E] [ 0 | 1 | 0 | 0 | 1 | 1 | 1 | 0 ]
     *      [0x803F] [ 1 | 0 | 0 | 0 | 1 | 0 | 1 | 1 ]
     *      data  =    2   1   0   0   3   1   3   2
     *
     *  This data  will then be converted to a color through a palette giving a grayscale value.
     *
     */
    // Extract the two adjacent bytes
    byte msb = _data[y] >> 8;
    byte lsb = _data[y] & 0xFF;

    // The pixels are ordered from left to right, the highest bit is the leftmost pixel.
    int bitPosition = (7 - x);

    return ((msb >> bitPosition) & 0x01) << 1 | ((lsb >> bitPosition) & 0x01);
}

SingleTile::SingleTile(VRAM* vram, int bankId, word startAddr) : Tile(TILE_HEIGHT, TILE_WIDTH, vram, bankId, startAddr)
{
}

StackedTile::StackedTile(VRAM* vram, int bankId, word startAddr)
    : Tile(TILE_HEIGHT, TILE_WIDTH, vram, bankId, startAddr)
{
}