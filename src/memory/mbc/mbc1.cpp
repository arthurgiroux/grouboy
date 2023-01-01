#include "mbc1.hpp"
#include <iostream>

MBC1::MBC1(Cartridge* cartridge) : MemoryBankController(cartridge)
{
}

byte MBC1::readROM(const word& addr)
{
    if (addr >= ROM_BANK0_START_ADDR && addr <= ROM_BANK0_END_ADDR)
    {
        return _cartridge->getData()[addr - ROM_BANK0_START_ADDR];
    }

    else if (addr >= ROM_BANK_SWAP_START_ADDR && addr <= ROM_BANK_SWAP_END_ADDR)
    {
        int addrOffset = addr - ROM_BANK_SWAP_START_ADDR;
        return _cartridge->getData()[_selectedROMBankId * ROM_BANK_SIZE_IN_BYTES + addrOffset];
    }

    throw std::runtime_error("Error, trying to read from bad memory.");
}

void MBC1::writeROM(const word& addr, const byte& value)
{
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        _isRAMEnabled = ((value & 0x0A) == 0x0A);
        std::cout << "Enabling RAM " << _isRAMEnabled << std::endl;
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        int selectedBankValue = value & 0x1F;
        if (selectedBankValue == 0)
        {
            selectedBankValue = 1;
        }
        _selectedROMBankId = selectedBankValue;

        std::cout << "SWITCHING TO ROM BANK " << _selectedROMBankId
                  << ", max=" << (_cartridge->getData().size() / ROM_BANK_SIZE_IN_BYTES) << std::endl;
        // TODO: Mask depending on number of available banks
    }

    else if (addr >= 0x4000 && addr <= 0x5FFF)
    {
        std::cout << "SWITCHING TO RAM BANK " << _selectedROMBankId << std::endl;
    }
}

byte MBC1::readRAM(const word& addr)
{
    if (!_isRAMEnabled)
    {
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
