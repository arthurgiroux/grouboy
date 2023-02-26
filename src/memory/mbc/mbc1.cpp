#include "mbc1.hpp"
#include "spdlog/spdlog.h"
#include <stdexcept>

MBC1::MBC1(Cartridge* cartridge) : MemoryBankController(cartridge)
{
    _ram.resize(cartridge->getRAMSize());
}

byte MBC1::readROM(const word& addr)
{
    if (fixedRomAddrRange.contains(addr))
    {
        return _cartridge->getData()[fixedRomAddrRange.relative(addr)];
    }

    else if (swappableRomAddrRange.contains(addr))
    {
        return _cartridge
            ->getData()[_selectedROMBankId * ROM_BANK_SIZE_IN_BYTES + swappableRomAddrRange.relative(addr)];
    }

    throw std::runtime_error("Error, trying to read from bad memory location.");
}

void MBC1::writeROM(const word& addr, const byte& value)
{
    // TODO: Handle addressing for ROM >= 1MiB
    if (ramEnableAddrRange.contains(addr))
    {
        // RAM is enabled only if value 0xA is written in lower 4 bits
        int bitMask = 0x0A;
        _isRAMEnabled = ((value & bitMask) == bitMask);
        if (_isRAMEnabled)
        {
            spdlog::debug("RAM was enabled for MBC.");
        }
        else
        {
            spdlog::debug("RAM was disabled for MBC.");
        }
    }
    else if (selectRomBankAddrRange.contains(addr))
    {
        // ROM selection is only made using the 5 lowest bits
        int bitMask = 0x1F;
        int selectedBankValue = value & bitMask;

        if (selectedBankValue == 0)
        {
            selectedBankValue = 1;
        }

        // We also mask the selected bank with the number of available banks
        int nbrBanks = _cartridge->getROMSize() / ROM_BANK_SIZE_IN_BYTES;
        // Retrieve the number of bits needed to encode the bank number
        int nbrBitsNeeded = log2(nbrBanks);
        selectedBankValue &= (1 << nbrBitsNeeded) - 1;

        _selectedROMBankId = selectedBankValue;
        spdlog::debug("MBC: Switching to ROM bank {}", _selectedROMBankId);
    }

    else if (selectRamBankAddrRange.contains(addr))
    {
        // RAM selection is only made using the 2 lowest bits
        int bitMask = 0b00000011;
        _selectedRAMBankId = (value & bitMask);
        spdlog::debug("MBC: Switching to RAM bank {}", _selectedRAMBankId);
    }
}

byte MBC1::readRAM(const word& addr)
{
    if (!_isRAMEnabled)
    {
        // If the RAM is disabled it will read open bus values
        // In reality value is often 0xFF
        return 0xFF;
    }

    return _ram[_selectedRAMBankId * RAM_BANK_SIZE_IN_BYTES + addr];
}

void MBC1::writeRAM(const word& addr, const byte& value)
{
    if (!_isRAMEnabled)
    {
        return;
    }

    _ram[_selectedRAMBankId * RAM_BANK_SIZE_IN_BYTES + addr] = value;
}

std::vector<byte> MBC1::serializeRAM()
{
    return _ram;
}

bool MBC1::unserializeRAM(const std::vector<byte>& data)
{
    if (data.size() != _ram.size()) {
        return false;
    }

    _ram = data;
    return true;
}