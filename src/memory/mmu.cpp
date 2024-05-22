#include "mmu.hpp"
#include <bitset>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "apu/apu.hpp"
#include "cartridge.hpp"
#include "cpu/interrupt_manager.hpp"
#include "graphics/lcd_status_register.hpp"
#include "memory/bootrom.hpp"
#include "spdlog/spdlog.h"

const std::set<word> MMU::unmappedIOAddrs = {0xFF03, 0xFF08, 0xFF09, 0xFF0A, 0xFF0B, 0xFF0C, 0xFF0D, 0xFF0E};

const std::map<word, byte> MMU::mappedIOMask = {{HardwareIOAddr::P1, static_cast<byte>(0b11000000)},
                                                {HardwareIOAddr::SC, static_cast<byte>(0b01111110)}};

const utils::AddressRange MMU::unmappedIOAddrRange = utils::AddressRange(0xFF4C, 0xFF7F);

const utils::AddressRange MMU::apuRegisterRange = utils::AddressRange(0xFF10, 0xFF3F);

MMU::MMU()
{
    reset();
}

void MMU::reset()
{
    memory = {};
    std::copy(BOOTROM.begin(), BOOTROM.end(), memory.begin());
}

byte MMU::read(const word& addr)
{
    if (cartridgeHeaderAddr.contains(addr) && cartridge != nullptr)
    {
        return cartridge->getData()[addr];
    }

    if (addr < BOOTROM.size() && isBootRomActive())
    {
        return memory[addr];
    }

    else if (_interruptManager != nullptr && addr == INTERRUPT_ENABLE_ADDR)
    {
        return _interruptManager->getInterruptEnableFlag();
    }

    else if (_interruptManager != nullptr && addr == INTERRUPT_FLAG_ADDR)
    {
        return _interruptManager->getInterruptFlag() | 0b11100000;
    }

    else if (addr == BOOT_ROM_UNMAPPED_FLAG_ADDR)
    {
        return static_cast<byte>(!isInBootrom) | 0b11111110;
    }

    else if (isColorModeSupported() && addr == VRAM_BANK_ID_ADDR)
    {
        return vram.getBankId() & 0xFE;
    }

    else if (isColorModeSupported() && addr == COLOR_PALETTE_DATA_BACKGROUND_ADDR)
    {
        return colorPaletteMemoryMapperBackground.readColor();
    }

    else if (isColorModeSupported() && addr == COLOR_PALETTE_DATA_OBJECTS_ADDR)
    {
        return colorPaletteMemoryMapperObjects.readColor();
    }

    else if (addr < ROM_BANK_1_END_ADDR && memoryBankController != nullptr)
    {
        return memoryBankController->readROM(addr);
    }

    else if (vram.addressRange.contains(addr))
    {
        return vram.read(vram.addressRange.relative(addr));
    }

    else if (externalRamAddr.contains(addr) && memoryBankController != nullptr)
    {
        return memoryBankController->readRAM(externalRamAddr.relative(addr));
    }

    else if (isColorModeSupported() && addr == WRAM_BANK_ID_ADDR)
    {
        return wramMemoryBank.getBankId() & 0xF8;
    }

    else if (wramAddressRange.contains(addr))
    {
        return wramMemoryBank.read(wramAddressRange.relative(addr));
    }

    else if (_oam.addressRange.contains(addr))
    {
        return _oam.read(_oam.addressRange.relative(addr));
    }

    else if (addr == JOYPAD_MAP_ADDR && inputController != nullptr)
    {
        return getJoypadMemoryRepresentation();
    }

    else if (mappedIOMask.count(addr))
    {
        return memory[addr] | mappedIOMask.at(addr);
    }

    else if (unmappedIOAddrs.count(addr) || unmappedIOAddrRange.contains(addr))
    {
        // For unmapped IO all bits should be set to 1
        return 0xFF;
    }
    else if (_apu != nullptr && apuRegisterRange.contains(addr))
    {
        return _apu->readRegister(addr);
    }
    else if (_timer != nullptr && addr == TIMER_DIV_ADDR)
    {
        return _timer->getDividerRegisterValue();
    }
    else if (_timer != nullptr && addr == TIMER_COUNTER_ADDR)
    {
        return _timer->getTimerCounterValue();
    }
    else if (_timer != nullptr && addr == TIMER_MODULO_ADDR)
    {
        return _timer->getTimerModuloValue();
    }
    else if (_timer != nullptr && addr == TIMER_CONTROL_ADDR)
    {
        return 0b11111000 | _timer->isTimerCounterEnabled() << 2 | _timer->getClockDivider();
    }

    else if (_lcdStatusRegister != nullptr && addr == LY_COMPARE_ADDR)
    {
        return _lcdStatusRegister->getLineYCompareRegister();
    }

    else if (_lcdStatusRegister != nullptr && addr == ADDR_LCD_STATUS)
    {
        return _lcdStatusRegister->getLcdStatusRegister() | 0b10000000;
    }

    else if (_lcdStatusRegister != nullptr && addr == ADDR_SCANLINE)
    {
        return _lcdStatusRegister->getScanlineRegister();
    }

    else if (_ppu != nullptr && addr == ADDR_LCD_PPU_CONTROL)
    {
        return _ppu->getLcdControl();
    }
    else if (_ppu != nullptr && addr == ADDR_SCROLL_Y)
    {
        return _ppu->getScrollY();
    }
    else if (_ppu != nullptr && addr == ADDR_SCROLL_X)
    {
        return _ppu->getScrollX();
    }
    else if (_ppu != nullptr && addr == WINDOW_ADDR_SCROLL_Y)
    {
        return _ppu->getWindowScrollY();
    }
    else if (_ppu != nullptr && addr == WINDOW_ADDR_SCROLL_X)
    {
        return _ppu->getWindowScrollX();
    }

    return memory[addr];
}

byte MMU::getJoypadMemoryRepresentation()
{
    /**
     * This is the internal memory representation
     * of the joypad status in memory at location JOYPAD_MAP_ADDR.
     *
     * Bit 7 - Not used
     * Bit 6 - Not used
     * Bit 5 - P15 Select Action buttons    (0=Select)
     * Bit 4 - P14 Select Direction buttons (0=Select)
     * Bit 3 - P13 Input: Down  or Start    (0=Pressed) (Read Only)
     * Bit 2 - P12 Input: Up    or Select   (0=Pressed) (Read Only)
     * Bit 1 - P11 Input: Left  or B        (0=Pressed) (Read Only)
     * Bit 0 - P10 Input: Right or A        (0=Pressed) (Read Only)
     */
    int value = memory[JOYPAD_MAP_ADDR];
    bool isAction = !utils::isNthBitSet(value, 5);

    setNthBitIfButtonIsReleased(isAction ? InputController::Button::A : InputController::Button::RIGHT, 0, value);
    setNthBitIfButtonIsReleased(isAction ? InputController::Button::B : InputController::Button::LEFT, 1, value);
    setNthBitIfButtonIsReleased(isAction ? InputController::Button::SELECT : InputController::Button::UP, 2, value);
    setNthBitIfButtonIsReleased(isAction ? InputController::Button::START : InputController::Button::DOWN, 3, value);

    return static_cast<byte>(value | 0b11000000);
}

void MMU::setNthBitIfButtonIsReleased(InputController::Button button, int bitPosition, int& value)
{
    utils::setNthBit(value, bitPosition, inputController->isButtonReleased(button));
}

word MMU::readWord(const word& addr)
{
    if (addr >= MEMORY_SIZE_IN_BYTES - 1)
    {
        throw InvalidMemoryAccessException();
    }

    return utils::createWordFromBytes(read(addr + 1), read(addr));
}

void MMU::write(const word& addr, const byte& value)
{
    if (addr == DMA_TRANSFER_ADDR)
    {
        word sourceAddr = value << 8;
        performDMATransfer(sourceAddr);
        return;
    }

    else if (_interruptManager != nullptr && addr == INTERRUPT_ENABLE_ADDR)
    {
        _interruptManager->setInterruptEnableFlag(value);
    }

    else if (_interruptManager != nullptr && addr == INTERRUPT_FLAG_ADDR)
    {
        _interruptManager->setInterruptFlag(value);
    }

    // Writing to the bootrom address deactivates it
    else if (addr == BOOT_ROM_UNMAPPED_FLAG_ADDR)
    {
        isInBootrom = false;
        return;
    }

    else if (isColorModeSupported() && addr == VRAM_BANK_ID_ADDR)
    {
        vram.switchBank(value & 0x01);
    }

    else if (isColorModeSupported() && addr == COLOR_PALETTE_SPECS_BACKGROUND_ADDR)
    {
        colorPaletteMemoryMapperBackground.enableAddressAutoIncrement(utils::isNthBitSet(value, 7));
        colorPaletteMemoryMapperBackground.setAddress(value & 0x7F);
    }

    else if (isColorModeSupported() && addr == COLOR_PALETTE_DATA_BACKGROUND_ADDR)
    {
        colorPaletteMemoryMapperBackground.writeColor(value);
    }

    else if (isColorModeSupported() && addr == COLOR_PALETTE_SPECS_OBJECTS_ADDR)
    {
        colorPaletteMemoryMapperObjects.enableAddressAutoIncrement(utils::isNthBitSet(value, 7));
        colorPaletteMemoryMapperObjects.setAddress(value & 0x7F);
    }

    else if (isColorModeSupported() && addr == COLOR_PALETTE_DATA_OBJECTS_ADDR)
    {
        colorPaletteMemoryMapperObjects.writeColor(value);
    }

    else if (addr < ROM_BANK_1_END_ADDR && memoryBankController != nullptr)
    {
        memoryBankController->writeROM(addr, value);
    }

    else if (vram.addressRange.contains(addr))
    {
        vram.write(vram.addressRange.relative(addr), value);
    }

    else if (externalRamAddr.contains(addr) && memoryBankController != nullptr)
    {
        memoryBankController->writeRAM(externalRamAddr.relative(addr), value);
    }

    else if (isColorModeSupported() && addr == WRAM_BANK_ID_ADDR)
    {
        // Value 0 maps to the first bank, other value map to 1-based bank
        wramMemoryBank.switchBank(value == 0 ? 0 : (value & 0x07) - 1);
    }

    else if (wramAddressRange.contains(addr))
    {
        wramMemoryBank.write(wramAddressRange.relative(addr), value);
    }

    else if (_oam.addressRange.contains(addr))
    {
        _oam.write(_oam.addressRange.relative(addr), value);
    }

    else if (_apu != nullptr && apuRegisterRange.contains(addr))
    {
        _apu->writeRegister(addr, value);
    }
    else if (_timer != nullptr && addr == TIMER_DIV_ADDR)
    {
        _timer->resetDividerRegisterValue();
    }
    else if (_timer != nullptr && addr == TIMER_COUNTER_ADDR)
    {
        _timer->setTimerCounterValue(value);
    }
    else if (_timer != nullptr && addr == TIMER_MODULO_ADDR)
    {
        _timer->setTimerModuloValue(value);
    }
    else if (_timer != nullptr && addr == TIMER_CONTROL_ADDR)
    {
        _timer->enableTimerCounter(utils::isNthBitSet(value, 2));
        _timer->setClockDivider(value & 0b00000011);
    }

    else if (_lcdStatusRegister != nullptr && addr == LY_COMPARE_ADDR)
    {
        _lcdStatusRegister->setLineYCompareRegister(value);
    }

    else if (_lcdStatusRegister != nullptr && addr == ADDR_LCD_STATUS)
    {
        _lcdStatusRegister->setLcdStatusRegister(value);
    }

    else if (_lcdStatusRegister != nullptr && addr == ADDR_SCANLINE)
    {
        _lcdStatusRegister->setScanlineRegister(value);
    }

    else if (_ppu != nullptr && addr == ADDR_LCD_PPU_CONTROL)
    {
        _ppu->setLcdControl(value);
    }
    else if (_ppu != nullptr && addr == ADDR_SCROLL_Y)
    {
        _ppu->setScrollY(value);
    }
    else if (_ppu != nullptr && addr == ADDR_SCROLL_X)
    {
        _ppu->setScrollX(value);
    }
    else if (_ppu != nullptr && addr == WINDOW_ADDR_SCROLL_Y)
    {
        _ppu->setWindowScrollY(value);
    }
    else if (_ppu != nullptr && addr == WINDOW_ADDR_SCROLL_X)
    {
        _ppu->setWindowScrollX(value);
    }

    else
    {
        memory[addr] = value;
    }
}

void MMU::writeWord(const word& addr, const word& value)
{
    if (addr >= MEMORY_SIZE_IN_BYTES - 1)
    {
        throw InvalidMemoryAccessException();
    }

    write(addr, utils::getLsbFromWord(value));
    write(addr + 1, utils::getMsbFromWord(value));
}

bool MMU::loadCartridgeFromFile(const std::string& filepath)
{
    std::vector<byte> data;
    if (utils::readBinaryDataFromFile(filepath, data))
    {
        return loadCartridgeData(data);
    }

    return false;
}

bool MMU::loadCartridgeData(std::vector<byte> data)
{
    cartridge = std::make_unique<Cartridge>(data);

    memoryBankController = MemoryBankController::createMemoryBankControllerFromCartridge(cartridge.get());
    if (memoryBankController == nullptr)
    {
        // TODO: Error code instead of exception
        spdlog::error("Memory Bus Controller '{}' is not implemented.",
                      Cartridge::cartridgeTypeToString(cartridge->getType()).c_str());
        return false;
    }

    return true;
}

Cartridge* MMU::getCartridge()
{
    return cartridge.get();
}

bool MMU::isBootRomActive()
{
    return isInBootrom;
}

void MMU::setInputController(InputController* controller)
{
    inputController = controller;
}

void MMU::performDMATransfer(word sourceAddr)
{
    for (word i = 0; i < DMA_TRANSFER_LENGTH; ++i)
    {
        write(DMA_TRANSFER_TARGET_ADDR + i, read(sourceAddr + i));
    }
}

std::vector<byte> MMU::serializeCartridgeRAM()
{
    if (memoryBankController != nullptr)
    {
        return memoryBankController->serializeRAM();
    }

    return std::vector<byte>();
}

bool MMU::unserializeCartridgeRAM(const std::vector<byte>& data)
{
    if (memoryBankController != nullptr)
    {
        return memoryBankController->unserializeRAM(data);
    }

    return false;
}

void MMU::setAPU(APU* apu)
{
    _apu = apu;
}

void MMU::setTimer(Timer* timer)
{
    _timer = timer;
}

VRAM& MMU::getVRAM()
{
    return vram;
}

ColorPaletteMemoryMapper& MMU::getColorPaletteMemoryMapperBackground()
{
    return colorPaletteMemoryMapperBackground;
}

ColorPaletteMemoryMapper& MMU::getColorPaletteMemoryMapperObj()
{
    return colorPaletteMemoryMapperObjects;
}

bool MMU::isColorModeSupported()
{
    return isBootRomActive() || (cartridge != nullptr && cartridge->isColorModeSupported());
}

void MMU::setInterruptManager(InterruptManager* interruptManager)
{
    _interruptManager = interruptManager;
}

void MMU::setLcdStatusRegister(LCDStatusRegister* lcdStatusRegister)
{
    _lcdStatusRegister = lcdStatusRegister;
}

void MMU::setPPU(PPU* ppu)
{
    _ppu = ppu;
}

OAM& MMU::getOAM()
{
    return _oam;
}
