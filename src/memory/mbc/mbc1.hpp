#ifndef GBEMULATOR_MBC1_HPP
#define GBEMULATOR_MBC1_HPP

#include "memory/mbc/memory_bank_controller.hpp"
#include <array>

class MBC1 : public MemoryBankController
{
  public:
    virtual ~MBC1() = default;
    MBC1(Cartridge* cartridge);

    byte readROM(const word& addr) override;
    void writeROM(const word& addr, const byte& value) override;
    byte readRAM(const word& addr) override;
    void writeRAM(const word& addr, const byte& value) override;

  private:
    int _selectedROMBankId = 1;
    int _selectedRAMBankId = 0;
    bool _isRAMEnabled = false;
    static const int ROM_BANK0_START_ADDR = 0x0000;
    static const int ROM_BANK0_END_ADDR = 0x3FFF;
    static const int ROM_BANK_SWAP_START_ADDR = 0x4000;
    static const int ROM_BANK_SWAP_END_ADDR = 0x7FFF;
    static const int ROM_BANK_SIZE_IN_BYTES = 16 * 1024;

    static const int RAM_BANK_START_ADDR = 0xA000;
    static const int RAM_BANK_END_ADDR = 0xBFFF;
    static const int RAM_BANK_SIZE_IN_BYTES = 8 * 1024;
    std::array<byte, 32 * 1024> _ram = {};
};

#endif // GBEMULATOR_MBC1_HPP
