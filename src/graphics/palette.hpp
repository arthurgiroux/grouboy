#ifndef GBEMULATOR_PALETTE_HPP
#define GBEMULATOR_PALETTE_HPP

#include "memory/mmu.hpp"

class Palette
{
  public:
    Palette(MMU& mmu, word paletteAddr);
    byte convertColorId(byte colorId);

  private:
    MMU& _mmu;
    word _paletteAddr;
};

#endif // GBEMULATOR_PALETTE_HPP
