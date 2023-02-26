#ifndef GBEMULATOR_MEMORY_BANK_CONTROLLER_HPP
#define GBEMULATOR_MEMORY_BANK_CONTROLLER_HPP

#include "memory/cartridge.hpp"
#include <memory>

/**
 * The memory bank controller is in charge of switching memory units
 * for the cartridge.
 */
class MemoryBankController
{
  public:
    /**
     * Default destructor
     */
    virtual ~MemoryBankController() = default;

    /**
     * Read a byte from an address in the ROM.
     *
     * @param addr The address to read from
     * @return  The byte stored at this address
     */
    virtual byte readROM(const word& addr) = 0;

    /**
     * Write a byte to the given address in the ROM.
     *
     * @param addr  the address to write to
     * @param value the byte value to write
     */
    virtual void writeROM(const word& addr, const byte& value) = 0;

    /**
     * Read a byte from an address in the RAM.
     *
     * @param addr The address to read from
     * @return  The byte stored at this address
     */
    virtual byte readRAM(const word& addr) = 0;

    /**
     * Write a byte to the given address in the RAM.
     *
     * @param addr  the address to write to
     * @param value the byte value to write
     */
    virtual void writeRAM(const word& addr, const byte& value) = 0;

    /**
     * Serialize the current state of the RAM.
     *
     * @return  a binary representation of the RAM that can be restored later
     */
    virtual std::vector<byte> serializeRAM() = 0;

    /**
     * Load a state of the RAM that was previously serialized using serializeRAM().
     *
     * @param data  the serialized data to load.
     */
    virtual bool unserializeRAM(const std::vector<byte>& data) = 0;

    /**
     * Factory function to create a memory controller based on cartridge information.
     *
     * @param cartridge The cartridge object from which we will read the MBC information
     * @return An MBC implementation or null if no implementation available
     */
    static std::unique_ptr<MemoryBankController> createMemoryBankControllerFromCartridge(Cartridge* cartridge);

  protected:
    /**
     * Create a new memory bank controller on a given cartridge
     *
     * @param cartridge The cartridge to use to retrieve the data
     */
    MemoryBankController(Cartridge* cartridge);

    /**
     * The cartridge object to be used to access the memory units
     */
    Cartridge* _cartridge = nullptr;
};

#endif // GBEMULATOR_MEMORY_BANK_CONTROLLER_HPP
