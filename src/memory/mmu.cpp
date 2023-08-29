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

const std::array<byte, 256> MMU::BIOS = {
    0x31, 0xFE, 0xFF, 0x21, 0x00, 0x80, 0x22, 0xCB, 0x6C, 0x28, 0xFB, 0x3E, 0x80, 0xE0, 0x26, 0xE0, 0x11, 0x3E, 0xF3,
    0xE0, 0x12, 0xE0, 0x25, 0x3E, 0x77, 0xE0, 0x24, 0x3E, 0x54, 0xE0, 0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A,
    0x47, 0xCD, 0xA2, 0x00, 0xCD, 0xA2, 0x00, 0x13, 0x7B, 0xEE, 0x34, 0x20, 0xF2, 0x11, 0xD1, 0x00, 0x0E, 0x08, 0x1A,
    0x13, 0x22, 0x23, 0x0D, 0x20, 0xF9, 0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08,
    0x32, 0x0D, 0x20, 0xF9, 0x2E, 0x0F, 0x18, 0xF5, 0x3E, 0x1E, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x16, 0x89, 0x0E,
    0x0F, 0xCD, 0xB7, 0x00, 0x7A, 0xCB, 0x2F, 0xCB, 0x2F, 0xE0, 0x42, 0x7A, 0x81, 0x57, 0x79, 0xFE, 0x08, 0x20, 0x04,
    0x3E, 0xA8, 0xE0, 0x47, 0x0D, 0x20, 0xE7, 0x3E, 0xFC, 0xE0, 0x47, 0x3E, 0x83, 0xCD, 0xCA, 0x00, 0x06, 0x05, 0xCD,
    0xC3, 0x00, 0x3E, 0xC1, 0xCD, 0xCA, 0x00, 0x06, 0x3C, 0xCD, 0xC3, 0x00, 0x21, 0xB0, 0x01, 0xE5, 0xF1, 0x21, 0x4D,
    0x01, 0x01, 0x13, 0x00, 0x11, 0xD8, 0x00, 0xC3, 0xFE, 0x00, 0x3E, 0x04, 0x0E, 0x00, 0xCB, 0x20, 0xF5, 0xCB, 0x11,
    0xF1, 0xCB, 0x11, 0x3D, 0x20, 0xF5, 0x79, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xE5, 0x21, 0x0F, 0xFF, 0xCB, 0x86, 0xCB,
    0x46, 0x28, 0xFC, 0xE1, 0xC9, 0xCD, 0xB7, 0x00, 0x05, 0x20, 0xFA, 0xC9, 0xE0, 0x13, 0x3E, 0x87, 0xE0, 0x14, 0xC9,
    0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x50};

const std::set<word> MMU::unmappedIOAddrs = {0xFF03, 0xFF08, 0xFF09, 0xFF0A, 0xFF0B, 0xFF0C, 0xFF0D, 0xFF0E};

const std::map<word, int> MMU::mappedIOMask = {{HardwareIOAddr::P1, 0b11000000},
                                               {HardwareIOAddr::SC, 0b01111110},
                                               {HardwareIOAddr::IF, 0b11100000},
                                               {HardwareIOAddr::STAT, 0b10000000},
                                               {HardwareIOAddr::BOOT_ROOM_LOCK, 0b11111110}};

const utils::AddressRange MMU::unmappedIOAddrRange = utils::AddressRange(0xFF4C, 0xFF7F);

const utils::AddressRange MMU::apuRegisterRange = utils::AddressRange(0xFF10, 0xFF3F);

MMU::MMU()
{
    reset();
}

void MMU::reset()
{
    memory = {};
    std::copy(BIOS.begin(), BIOS.end(), memory.begin());
}

/***********************************
            MEMORY LAYOUT

[ BIOS | ROM BANK 0 | ROM BANK 1 | GPU VRAM | Ext. RAM | Working RAM | Sprites info | I/O | ZRAM ]
    0     256b   8k    16k    24k   32k        40k        48k           56k                         64k


    ***********************************/

byte MMU::read(const word& addr)
{
    if (addr < BIOS.size() && isBootRomActive())
    {
        return memory[addr];
    }

    else if (addr < ROM_BANK_1_END_ADDR && memoryBankController != nullptr)
    {
        return memoryBankController->readROM(addr);
    }

    else if (addr >= EXTERNAL_RAM_START_ADDR && addr < EXTERNAL_RAM_END_ADDR && memoryBankController != nullptr)
    {
        return memoryBankController->readRAM(addr - EXTERNAL_RAM_START_ADDR);
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

    return value | 0b11000000;
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

    // Once the bootroom has been deactivated we can't reactivate it again
    else if (addr == BOOT_ROM_UNMAPPED_FLAG_ADDR && !isBootRomActive())
    {
        return;
    }

    else if (addr < ROM_BANK_1_END_ADDR && memoryBankController != nullptr)
    {
        memoryBankController->writeROM(addr, value);
    }

    else if (addr >= EXTERNAL_RAM_START_ADDR && addr < EXTERNAL_RAM_END_ADDR && memoryBankController != nullptr)
    {
        memoryBankController->writeRAM(addr - EXTERNAL_RAM_START_ADDR, value);
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

bool MMU::loadCartridge(const std::string& filepath)
{
    std::vector<byte> data;
    if (utils::readBinaryDataFromFile(filepath, data))
    {
        cartridge = std::make_unique<Cartridge>(data);
        memoryBankController = MemoryBankController::createMemoryBankControllerFromCartridge(cartridge.get());
        if (memoryBankController == nullptr)
        {
            throw std::runtime_error(
                utils::string_format("Memory Bus Controller '%s' is not implemented.",
                                     Cartridge::cartridgeTypeToString(cartridge->getType()).c_str()));
        }

        return true;
    }

    return false;
}

Cartridge* MMU::getCartridge()
{
    return cartridge.get();
}

bool MMU::isBootRomActive()
{
    return !utils::isNthBitSet(read(BOOT_ROM_UNMAPPED_FLAG_ADDR), 0);
}

void MMU::setInputController(InputController* controller)
{
    inputController = controller;
}

void MMU::performDMATransfer(word sourceAddr)
{
    for (int i = 0; i < DMA_TRANSFER_LENGTH; ++i)
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
