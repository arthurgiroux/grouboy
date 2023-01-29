#include "mbc2.hpp"
#include "spdlog/spdlog.h"

MBC2::MBC2(Cartridge* cartridge) : MemoryBankController(cartridge)
{
}

byte MBC2::readROM(const word& addr)
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

void MBC2::writeROM(const word& addr, const byte& value)
{
    if (ramEnableBankControlAddrRange.contains(addr))
    {
        // The LSB of the upper address byte tells us which command to execute:
        // RAM enable/disable or rom bank selection
        if (!utils::isNthBitSet(addr, 8))
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
        else
        {
            // ROM selection is only made using the 4 lowest bits
            int bitMask = 0x0F;
            int selectedBankValue = value & bitMask;

            if (selectedBankValue == 0)
            {
                selectedBankValue = 1;
            }

            _selectedROMBankId = selectedBankValue;
            spdlog::debug("MBC: Switching to ROM bank {}", _selectedROMBankId);
        }
    }
}

byte MBC2::readRAM(const word& addr)
{
    if (!_isRAMEnabled)
    {
        // If the RAM is disabled it will read open bus values
        // In reality value is often 0xFF
        return 0xFF;
    }

    /*
     * Only the 9 lowest bits are used for addressing the RAM
     * Which means that the RAM has "echo" section that will repeat the first section.
     */
    int addrBitMask = 0x1FF;
    return _ram[addr & addrBitMask];
}

void MBC2::writeRAM(const word& addr, const byte& value)
{
    if (!_isRAMEnabled)
    {
        return;
    }

    /*
     * Only the 9 lowest bits are used for addressing the RAM
     * Which means that the RAM has "echo" section that will repeat the first section.
     */
    int addrBitMask = 0x1FF;

    // The RAM of MBC2 is only 4 bits value
    int valueBitMask = 0xF;
    _ram[addr & addrBitMask] = value & valueBitMask;
}
