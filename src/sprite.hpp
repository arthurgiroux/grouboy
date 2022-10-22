#ifndef GBEMULATOR_SPRITE_HPP
#define GBEMULATOR_SPRITE_HPP

#include "mmu.hpp"
class Sprite
{
  public:
	Sprite(MMU& mmu, int spriteId);
	~Sprite() = default;

	int getId() const;
	int getXCoordinate() const;
	int getYCoordinate() const;
	int getXPositionOnScreen() const;
	int getYPositionOnScreen() const;
	int getTileId() const;
	bool isFlippedVertically() const;
	bool isFlippedHorizontally() const;

  private:
	int readPayload(int idx) const;

	static const int SPRITE_ATTR_TABLE_ADDR = 0xFE00;
	static const int PAYLOAD_PER_SPRITE = 4;
	static const int PAYLOAD_DATA_Y_IDX = 0;
	static const int PAYLOAD_DATA_X_IDX = 1;
	static const int PAYLOAD_DATA_TILEID_IDX = 2;
	static const int PAYLOAD_DATA_FLAG_ATTR_IDX = 3;
	static const int Y_SCREEN_OFFSET = 16;
	static const int X_SCREEN_OFFSET = 8;
	MMU& _mmu;
	int _id;
};

#endif // GBEMULATOR_SPRITE_HPP
