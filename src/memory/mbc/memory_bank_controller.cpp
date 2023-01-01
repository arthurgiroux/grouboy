#include "memory_bank_controller.hpp"
#include "memory/mbc/mbc1.hpp"
#include "memory/mbc/mbc_rom_only.hpp"

MemoryBankController::MemoryBankController(Cartridge* cartridge) : _cartridge(cartridge){};

std::unique_ptr<MemoryBankController>
MemoryBankController::createMemoryBankControllerFromCartridge(Cartridge* cartridge)
{
    auto cartridgeType = cartridge->getType();
    switch (cartridgeType)
    {
    case Cartridge::CartridgeType::ROM_ONLY:
        return std::make_unique<MBCRomOnly>(cartridge);
    case Cartridge::CartridgeType::MBC1:
    case Cartridge::CartridgeType::MBC1_RAM:
    case Cartridge::CartridgeType::MBC1_RAM_BATTERY:
        return std::make_unique<MBC1>(cartridge);
    default:
        return nullptr;
    }
}
