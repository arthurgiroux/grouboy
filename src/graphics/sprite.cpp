#include "sprite.hpp"

Sprite::Sprite(MMU& mmu, int spriteId) : _mmu(mmu), _id(spriteId)
{
}

int Sprite::getId() const
{
    return _id;
}

int Sprite::getTileId() const
{
    return readDataFromPayload(PAYLOAD_DATA_TILEID_IDX);
}

bool Sprite::isFlippedVertically() const
{
    return utils::isNthBitSet(readDataFromPayload(PAYLOAD_DATA_FLAG_ATTR_IDX), DATA_FLAG_BIT_VFLIP);
}

bool Sprite::isFlippedHorizontally() const
{
    return utils::isNthBitSet(readDataFromPayload(PAYLOAD_DATA_FLAG_ATTR_IDX), DATA_FLAG_BIT_HFLIP);
}

int Sprite::readDataFromPayload(int idx) const
{
    return _mmu.read(SPRITE_ATTR_TABLE_ADDR + PAYLOAD_PER_SPRITE * _id + idx);
}

int Sprite::getXPositionOnScreen() const
{
    return readDataFromPayload(PAYLOAD_DATA_X_IDX) - X_SCREEN_OFFSET;
}

int Sprite::getYPositionOnScreen() const
{
    return readDataFromPayload(PAYLOAD_DATA_Y_IDX) - Y_SCREEN_OFFSET;
}
