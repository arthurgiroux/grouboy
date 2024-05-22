#include "oam.hpp"

byte OAM::read(word addr) const
{
    return _memory[addr];
}

void OAM::write(word addr, byte value)
{
    _memory[addr] = value;
}
