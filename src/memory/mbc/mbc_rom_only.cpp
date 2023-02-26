#include "mbc_rom_only.hpp"
#include <stdexcept>

MBCRomOnly::MBCRomOnly(Cartridge* cartridge) : MemoryBankController(cartridge)
{
    _ram.resize(cartridge->getRAMSize());
}

byte MBCRomOnly::readROM(const word& addr)
{
    int romSize = _cartridge->getROMSize();
    if (addr < romSize)
    {
        return _cartridge->getData()[addr];
    }
    else
    {
        throw std::runtime_error(
            utils::string_format("Trying to read outside of ROM range. Addr=%d, RAM Size=%d", addr, romSize));
    }
}

void MBCRomOnly::writeROM(const word& addr, const byte& value)
{
    throw std::runtime_error("Writing to ROM is not allowed.");
}

byte MBCRomOnly::readRAM(const word& addr)
{
    if (addr < _ram.size())
    {
        return _ram[addr];
    }
    else
    {
        throw std::runtime_error(
            utils::string_format("Trying to read outside of RAM range. Addr=%d, RAM Size=%d", addr, _ram.size()));
    }
}

void MBCRomOnly::writeRAM(const word& addr, const byte& value)
{
    if (addr < _ram.size())
    {
        _ram[addr] = value;
    }
    else
    {
        throw std::runtime_error(
            utils::string_format("Trying to write outside of RAM range. Addr=%d, RAM Size=%d", addr, _ram.size()));
    }
}

std::vector<byte> MBCRomOnly::serializeRAM()
{
    return std::vector<byte>();
}

bool MBCRomOnly::unserializeRAM(const std::vector<byte>& data)
{
    return true;
}