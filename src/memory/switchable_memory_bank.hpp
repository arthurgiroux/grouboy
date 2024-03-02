#ifndef GROUBOY_SWITCHABLE_MEMORY_BANK_HPP
#define GROUBOY_SWITCHABLE_MEMORY_BANK_HPP

#include "common/types.hpp"
#include <array>

/**
 * A Switchable Memory Bank allows to map a specific memory space
 * to several memory banks.
 * Only one bank is active at once and they can be switched at will,
 * retaining their individual values.
 *
 * @tparam NBR_BANK The number of banks
 * @tparam MEM_SIZE The size in bytes of each bank
 */
template <int NBR_BANK, int MEM_SIZE>
class SwitchableMemoryBank
{
  public:
    SwitchableMemoryBank() = default;
    virtual ~SwitchableMemoryBank() = default;

    /**
     * Get the id of the active bank.
     * @return an int between 0 and NBR_BANK-1
     */
    unsigned int getBankId() const;

    /**
     * Switch the active bank to the one with corresponding index
     * @param index The index of the bank, between 0 and NBR_BANK-1
     */
    void switchBank(unsigned int index);

    /**
     * Read the value at the given address for the active bank.
     *
     * @param addr a value between 0 and MEM_SIZE-1
     * @return the value of the active bank
     */
    byte read(word addr) const;

    /**
     * Read the value at the given address for the given bank
     * @param addr a value between 0 and MEM_SIZE-1
     * @param bankId The index of the bank, between 0 and NBR_BANK-1
     * @return the value in memory of the given bank
     */
    byte readFromBank(word addr, unsigned int bankId) const;

    /**
     * Write the given value at the given address for the active bank.
     *
     * @param addr a value between 0 and MEM_SIZE-1
     * @param value the value to write
     */
    void write(word addr, byte value);

  private:
    /**
     * The underlying memory for all the banks
     */
    std::array<std::array<byte, MEM_SIZE>, NBR_BANK> memory{};

    /**
     * The id of the current bank
     */
    int currentBankId = 0;
};

#endif // GROUBOY_SWITCHABLE_MEMORY_BANK_HPP
