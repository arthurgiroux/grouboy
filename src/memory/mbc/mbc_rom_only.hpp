#ifndef GBEMULATOR_MBC_ROM_ONLY_HPP
#define GBEMULATOR_MBC_ROM_ONLY_HPP

#include "memory/mbc/memory_bank_controller.hpp"
#include <array>

/**
 * A simple MBC implementation where there's only 1 non-swappable ROM available.
 * Optional RAM can be available.
 */
class MBCRomOnly : public MemoryBankController
{
  public:
    /**
     * Default destructor.
     */
    virtual ~MBCRomOnly() = default;
    /**
     * Create a new MBC with 1 fixed ROM and optional RAM.
     *
     * @param cartridge The cartridge to use to retrieve the data.
     */
    MBCRomOnly(Cartridge* cartridge);

    byte readROM(const word& addr) override;
    void writeROM(const word& addr, const byte& value) override;
    byte readRAM(const word& addr) override;
    void writeRAM(const word& addr, const byte& value) override;
    std::vector<byte> serializeRAM() override;
    bool unserializeRAM(const std::vector<byte>& data) override;

  private:
    /**
     * Optional RAM can be up to 8KiB
     */
    std::vector<byte> _ram = {};
};

#endif // GBEMULATOR_MBC_ROM_ONLY_HPP
