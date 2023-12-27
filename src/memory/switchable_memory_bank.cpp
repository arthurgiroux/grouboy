#include "switchable_memory_bank.hpp"
#include "common/utils.hpp"

template <int NBR_BANK, int MEM_SIZE>
void SwitchableMemoryBank<NBR_BANK, MEM_SIZE>::switchBank(unsigned int index)
{
    if (index < NBR_BANK)
    {
        currentBankId = index;
    }
}

template <int NBR_BANK, int MEM_SIZE>
byte SwitchableMemoryBank<NBR_BANK, MEM_SIZE>::read(word addr) const
{
    return readFromBank(addr, currentBankId);
}

template <int NBR_BANK, int MEM_SIZE>
void SwitchableMemoryBank<NBR_BANK, MEM_SIZE>::write(word addr, byte value)
{
    if (addr < MEM_SIZE)
    {
        memory[currentBankId][addr] = value;
    }
}

template <int NBR_BANK, int MEM_SIZE>
unsigned int SwitchableMemoryBank<NBR_BANK, MEM_SIZE>::getBankId() const
{
    return currentBankId;
}

template <int NBR_BANK, int MEM_SIZE>
byte SwitchableMemoryBank<NBR_BANK, MEM_SIZE>::readFromBank(word addr, unsigned int bankId) const
{
    if (addr < MEM_SIZE && bankId < NBR_BANK)
    {
        return memory[bankId][addr];
    }

    return 0;
}

template class SwitchableMemoryBank<2, 8_KiB>;
template class SwitchableMemoryBank<7, 4_KiB>;