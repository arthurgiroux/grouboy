#include "mbc3.hpp"
#include "spdlog/spdlog.h"

MBC3::MBC3(Cartridge* cartridge) : MemoryBankController(cartridge)
{
    _ram.resize(cartridge->getRAMSize());
}

byte MBC3::readROM(const word& addr)
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

void MBC3::writeROM(const word& addr, const byte& value)
{
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
        // ROM selection is only made using 7 bits
        int bitMask = 0x7F;
        int selectedBankValue = value & bitMask;

        if (selectedBankValue == 0)
        {
            selectedBankValue = 1;
        }

        _selectedROMBankId = selectedBankValue;
        spdlog::debug("MBC: Switching to ROM bank {}", _selectedROMBankId);
    }

    else if (selectRamBankAddrRange.contains(addr))
    {
        if (value < MAX_NBR_RAM_BANKS)
        {
            _isRTCModeEnabled = false;
            _selectedRAMBankId = value;
            spdlog::debug("MBC: Switching to RAM bank {}", _selectedRAMBankId);
        }
        else
        {
            // TODO: Implement RTC
            _isRTCModeEnabled = true;
        }
    }
}

byte MBC3::readRAM(const word& addr)
{
    if (!_isRAMEnabled)
    {
        // If the RAM is disabled it will read open bus values
        // In reality value is often 0xFF
        return 0xFF;
    }

    if (!_isRTCModeEnabled)
    {
        return _ram[_selectedRAMBankId * RAM_BANK_SIZE_IN_BYTES + addr];
    }
    else
    {
        // TODO: Implement RTC
        return 0x00;
    }
}

void MBC3::writeRAM(const word& addr, const byte& value)
{
    if (!_isRAMEnabled)
    {
        return;
    }

    if (!_isRTCModeEnabled)
    {
        _ram[_selectedRAMBankId * RAM_BANK_SIZE_IN_BYTES + addr] = value;
    }
    else
    {
        // TODO: Implement RTC
    }
}