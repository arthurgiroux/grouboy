#include "mbc_rom_only.hpp"
#include <stdexcept>

MBCRomOnly::MBCRomOnly(Cartridge* cartridge) : MemoryBankController(cartridge)
{
}

byte MBCRomOnly::readROM(const word& addr)
{
    return _cartridge->getData()[addr];
}

void MBCRomOnly::writeROM(const word& addr, const byte& value)
{
    throw std::runtime_error("Writing to ROM is not allowed.");
}

byte MBCRomOnly::readRAM(const word& addr)
{
    return _ram[addr];
}

void MBCRomOnly::writeRAM(const word& addr, const byte& value)
{
    _ram[addr] = value;
}
