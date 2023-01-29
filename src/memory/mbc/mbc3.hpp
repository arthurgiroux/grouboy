#ifndef GBEMULATOR_MBC3_HPP
#define GBEMULATOR_MBC3_HPP

#include "memory/mbc/memory_bank_controller.hpp"
#include <array>

/**
 * A controllable MBC that supports up to 2 MiB ROM with up to 32 KiB of banked RAM.
 * This MBC also contains an RTC.
 */
class MBC3 : public MemoryBankController
{
  public:
    /**
     * Create a new MBC that supports up to 2 MiB ROM with up to 32 KiB of banked RAM
     *
     * @param cartridge The cartridge to use to retrieve the data.
     */
    MBC3(Cartridge* cartridge);

    /**
     * Default destructor
     */
    virtual ~MBC3() = default;

    byte readROM(const word& addr) override;
    void writeROM(const word& addr, const byte& value) override;
    byte readRAM(const word& addr) override;
    void writeRAM(const word& addr, const byte& value) override;

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
     * The address range that allows to enable the RAM
     */
    const utils::AddressRange ramEnableAddrRange = utils::AddressRange(0x0000, 0x1FFF);

    /**
     * The address range that allows to select which ROM bank is active
     */
    const utils::AddressRange selectRomBankAddrRange = utils::AddressRange(0x2000, 0x3FFF);

    /**
     * The address range that allows to select which RAM bank is active
     */
    const utils::AddressRange selectRamBankAddrRange = utils::AddressRange(0x4000, 0x5FFF);

    /**
     * The size of a ROM bank in bytes
     */
    static const int ROM_BANK_SIZE_IN_BYTES = 16_KiB;

    /**
     * The size of a RAM bank in bytes
     */
    static const int RAM_BANK_SIZE_IN_BYTES = 8_KiB;

    /**
     * Maximum number of ram banks
     */
    static const int MAX_NBR_RAM_BANKS = 4;

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
     * Whether or not the RTC registers should be mapped in the RAM.
     */
    bool _isRTCModeEnabled = false;

    /**
     * The id of the RAM bank currently selected.
     */
    int _selectedRAMBankId = 0;

    /**
     * Optional RAM bank.
     */
    std::vector<byte> _ram = {};
};

#endif // GBEMULATOR_MBC3_HPP
