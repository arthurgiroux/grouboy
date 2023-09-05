#ifndef GBEMULATOR_MBC2_HPP
#define GBEMULATOR_MBC2_HPP

#include "memory/mbc/memory_bank_controller.hpp"
#include <array>

/**
 * A controllable MBC that supports 256 KiB of ROM and 512x4 bits RAM.
 */
class MBC2 : public MemoryBankController
{
  public:
    /**
     * Create a new MBC that supports 256 KiB of ROM and 512x4 bits RAM.
     *
     * @param cartridge The cartridge to use to retrieve the data.
     */
    MBC2(Cartridge* cartridge);

    /**
     * Default destructor
     */
    virtual ~MBC2() = default;

    byte readROM(const word& addr) override;
    void writeROM(const word& addr, const byte& value) override;
    byte readRAM(const word& addr) override;
    void writeRAM(const word& addr, const byte& value) override;
    std::vector<byte> serializeRAM() override;
    bool unserializeRAM(const std::vector<byte>& data) override;

  private:
    /**
     * The address range that corresponds to the fixed ROM bank
     */
    const utils::AddressRange fixedRomAddrRange = utils::AddressRange(0x0000, 0x3FFF);

    /**
     * The address range that corresponds to the ROM bank that can be swapped
     */
    const utils::AddressRange swappableRomAddrRange = utils::AddressRange(0x4000, 0x7FFF);

    /**
     * The address range that allows to enable the RAM or control which ROM Bank is active
     */
    const utils::AddressRange ramEnableBankControlAddrRange = utils::AddressRange(0x0000, 0x3FFF);

    /**
     * The size of a ROM bank in bytes
     */
    static const size_t ROM_BANK_SIZE_IN_BYTES = 16_KiB;

    /**
     * The size of the RAM in bytes
     */
    static const size_t RAM_SIZE_IN_BYTES = 512_KiB;

    /**
     * The id of the ROM bank currently selected.
     * This value cannot be 0.
     */
    int _selectedROMBankId = 1;

    /**
     * Whether or not the RAM bank is enabled.
     */
    bool _isRAMEnabled = false;

    /**
     * 4 bits 512KiB RAM.
     */
    std::vector<byte> _ram = {};
};

#endif // GBEMULATOR_MBC2_HPP
