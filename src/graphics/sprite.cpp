#include "sprite.hpp"

Sprite::Sprite(MMU& mmu, int spriteId) : _mmu(mmu), _id(spriteId)
{
}

int Sprite::getId() const
{
    return _id;
}

byte Sprite::getTileId() const
{
    return readDataFromPayload(PAYLOAD_DATA_TILEID_IDX);
}

int Sprite::getGrayscalePaletteId() const
{
    return utils::isNthBitSet(readDataFromPayload(PAYLOAD_DATA_FLAG_ATTR_IDX), DATA_FLAG_BIT_PALETTEID);
}

bool Sprite::isFlippedVertically() const
{
    return utils::isNthBitSet(readDataFromPayload(PAYLOAD_DATA_FLAG_ATTR_IDX), DATA_FLAG_BIT_VFLIP);
}

bool Sprite::isFlippedHorizontally() const
{
    return utils::isNthBitSet(readDataFromPayload(PAYLOAD_DATA_FLAG_ATTR_IDX), DATA_FLAG_BIT_HFLIP);
}

bool Sprite::isRenderedOverBackgroundAndWindow() const
{
    return !utils::isNthBitSet(readDataFromPayload(PAYLOAD_DATA_FLAG_ATTR_IDX), DATA_FLAG_BIT_BGWINDOW_OVER_OBJ);
}

byte Sprite::readDataFromPayload(int idx) const
{
    return _mmu.read(static_cast<word>(SPRITE_ATTR_TABLE_ADDR + PAYLOAD_PER_SPRITE * _id + idx));
}

int Sprite::getXPositionOnScreen() const
{
    return readDataFromPayload(PAYLOAD_DATA_X_IDX) - X_SCREEN_OFFSET;
}

int Sprite::getYPositionOnScreen() const
{
    return readDataFromPayload(PAYLOAD_DATA_Y_IDX) - Y_SCREEN_OFFSET;
}

int Sprite::getBankId() const
{
    return utils::isNthBitSet(readDataFromPayload(PAYLOAD_DATA_FLAG_ATTR_IDX), DATA_FLAG_BIT_BANKID);
}

int Sprite::getColorPaletteId() const
{
    return readDataFromPayload(PAYLOAD_DATA_FLAG_ATTR_IDX) & 0x07;
}
