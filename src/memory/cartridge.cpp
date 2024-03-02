#include "cartridge.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <set>
#include <stdexcept>
#include <utility>

Cartridge::Cartridge(std::vector<byte> binaryData) : data(std::move(binaryData))
{
    readHeader();
}

const std::string& Cartridge::getTitle() const
{
    return title;
}

const std::string& Cartridge::getManufacturerCode() const
{
    return manufacturerCode;
}

std::vector<byte>& Cartridge::getData()
{
    return data;
}

void Cartridge::readHeader()
{
    readTitle();
    readManufacturerCode();
    readType();
}

void Cartridge::readTitle()
{
    const byte* startAddr = data.data() + TITLE_START_ADDR;
    // The title can be null terminated or not if it has the max size
    int tileLength = TITLE_LENGTH;
    size_t len = std::min(static_cast<int>(strlen(reinterpret_cast<const char*>(startAddr))), tileLength);
    title = std::string(startAddr, startAddr + len);
}

void Cartridge::readManufacturerCode()
{
    const byte* startAddr = data.data() + MANUFACTURER_CODE_START_ADDR;
    manufacturerCode = std::string(startAddr, startAddr + MANUFACTURER_CODE_LENGTH);
}

std::string Cartridge::cartridgeTypeToString(CartridgeType value)
{
    switch (value)
    {
    case 0x00:
        return "ROM_ONLY";
    case 0x01:
        return "MBC1";
    case 0x02:
        return "MBC1_RAM";
    case 0x03:
        return "MBC1_RAM_BATTERY";
    case 0x05:
        return "MBC2";
    case 0x06:
        return "MBC2_BATTERY";
    case 0x08:
        return "ROM_RAM_1";
    case 0x09:
        return "ROM_RAM_BATTERY_1";
    case 0x0B:
        return "MMM01";
    case 0x0C:
        return "MMM01_RAM";
    case 0x0D:
        return "MMM01_RAM_BATTERY";
    case 0x0F:
        return "MBC3_TIMER_BATTERY";
    case 0x10:
        return "MBC3_TIMER_RAM_BATTERY_2";
    case 0x11:
        return "MBC3";
    case 0x12:
        return "MBC3_RAM_2";
    case 0x13:
        return "MBC3_RAM_BATTERY_2";
    case 0x19:
        return "MBC5";
    case 0x1A:
        return "MBC5_RAM";
    case 0x1B:
        return "MBC5_RAM_BATTERY";
    case 0x1C:
        return "MBC5_RUMBLE";
    case 0x1D:
        return "MBC5_RUMBLE_RAM";
    case 0x1E:
        return "MBC5_RUMBLE_RAM_BATTERY";
    case 0x20:
        return "MBC6";
    case 0x22:
        return "MBC7_SENSOR_RUMBLE_RAM_BATTERY";
    case 0xFC:
        return "POCKET_CAMERA";
    case 0xFD:
        return "BANDAI_TAMA5";
    case 0xFE:
        return "HUC3";
    case 0xFF:
        return "HUC1_RAM_BATTERY";
    default:
        return "UNKNOWN";
    }
}

Cartridge::CartridgeType Cartridge::getType() const
{
    return type;
}

void Cartridge::readType()
{
    type = static_cast<CartridgeType>(data[CARTRIDGE_TYPE_ADDR]);
}

size_t Cartridge::getROMSize() const
{
    return 32_KiB * (1ULL << data[CARTRIDGE_ROM_SIZE_ADDR]);
}

size_t Cartridge::getRAMSize() const
{
    unsigned int ramSizeValue = data[CARTRIDGE_RAM_SIZE_ADDR];

    if (!hasRAM())
    {
        return 0_KiB;
    }

    // Values taken from: https://gbdev.io/pandocs/The_Cartridge_Header.html#0149--ram-size
    std::array<size_t, 6> ramSizeValueToBytes = {0_KiB, 0_KiB, 8_KiB, 32_KiB, 128_KiB, 64_KiB};

    if (ramSizeValue >= ramSizeValueToBytes.size())
    {
        throw std::runtime_error(utils::string_format("Unexpected RAM size value %d", ramSizeValue));
    }

    return ramSizeValueToBytes[ramSizeValue];
}

bool Cartridge::hasRAM() const
{
    std::set<CartridgeType> typesWithRAM = {MBC1_RAM,
                                            MBC1_RAM_BATTERY,
                                            ROM_RAM_1,
                                            ROM_RAM_BATTERY_1,
                                            MMM01_RAM,
                                            MMM01_RAM_BATTERY,
                                            MBC3_TIMER_RAM_BATTERY_2,
                                            MBC3_RAM_2,
                                            MBC3_RAM_BATTERY_2,
                                            MBC5_RAM,
                                            MBC5_RAM_BATTERY,
                                            MBC5_RUMBLE_RAM,
                                            MBC5_RUMBLE_RAM_BATTERY,
                                            MBC7_SENSOR_RUMBLE_RAM_BATTERY,
                                            HUC1_RAM_BATTERY};

    return typesWithRAM.count(type) > 0;
}

bool Cartridge::isColorModeSupported() const
{
    byte colorFlag = data[COLOR_MODE_FLAG_ADDR];
    byte flagColorOnly = 0xC0;
    byte flagColorAndMono = 0x80;
    return colorFlag == flagColorOnly || colorFlag == flagColorAndMono;
}
