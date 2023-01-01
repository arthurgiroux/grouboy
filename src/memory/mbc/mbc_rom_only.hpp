#ifndef GBEMULATOR_MBC_ROM_ONLY_HPP
#define GBEMULATOR_MBC_ROM_ONLY_HPP

#include "memory/mbc/memory_bank_controller.hpp"
#include <array>

class MBCRomOnly : public MemoryBankController
{
  public:
    virtual ~MBCRomOnly() = default;
    MBCRomOnly(Cartridge* cartridge);

    byte readROM(const word& addr) override;
    void writeROM(const word& addr, const byte& value) override;
    byte readRAM(const word& addr) override;
    void writeRAM(const word& addr, const byte& value) override;

  private:
    std::array<byte, 8 * 1024> _ram = {};
};

#endif // GBEMULATOR_MBC_ROM_ONLY_HPP
