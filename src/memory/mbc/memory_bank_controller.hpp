#ifndef GBEMULATOR_MEMORY_BANK_CONTROLLER_HPP
#define GBEMULATOR_MEMORY_BANK_CONTROLLER_HPP

#include "memory/cartridge.hpp"

class MemoryBankController
{
  public:
    virtual ~MemoryBankController() = default;

    virtual byte readROM(const word& addr) = 0;
    virtual void writeROM(const word& addr, const byte& value) = 0;
    virtual byte readRAM(const word& addr) = 0;
    virtual void writeRAM(const word& addr, const byte& value) = 0;

    static std::unique_ptr<MemoryBankController> createMemoryBankControllerFromCartridge(Cartridge* cartridge);

  protected:
    MemoryBankController(Cartridge* cartridge);
    Cartridge* _cartridge = nullptr;
};

#endif // GBEMULATOR_MEMORY_BANK_CONTROLLER_HPP
