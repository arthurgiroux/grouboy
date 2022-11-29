#ifndef GBEMULATOR_CARTRIDGE_HPP
#define GBEMULATOR_CARTRIDGE_HPP

#include "common/utils.hpp"
#include <string>
#include <vector>

/**
 * Represents a Gameboy Cartridge, holds the ROM/RAM data
 * and information about the cartridge itself.
 */
class Cartridge
{
  public:
    /**
     * Create a cartridge from some given binary data
     * @param binaryData       the cartridge data
     */
    explicit Cartridge(std::vector<byte> binaryData);

    ~Cartridge() = default;

    /**
     * Get the title
     * @return an ascii string (max 11 characters)
     */
    const std::string& getTitle() const;

    /**
     * Get the manufacturer code
     * @return a 4 ascii character code
     */
    const std::string& getManufacturerCode() const;

    /**
     * Get the binary data
     * @return
     */
    std::vector<byte>& getData();

    /**
     * Represents the type of hardware presents on the cartridge
     */
    enum CartridgeType
    {
        UNKNOWN = -1,
        ROM_ONLY = 0x00,
        MBC1 = 0x01,
        MBC1_RAM = 0x02,
        MBC1_RAM_BATTERY = 0x03,
        MBC2 = 0x05,
        MBC2_BATTERY = 0x06,
        ROM_RAM_1 = 0x08,
        ROM_RAM_BATTERY_1 = 0x09,
        MMM01 = 0x0B,
        MMM01_RAM = 0x0C,
        MMM01_RAM_BATTERY = 0x0D,
        MBC3_TIMER_BATTERY = 0x0F,
        MBC3_TIMER_RAM_BATTERY_2 = 0x10,
        MBC3 = 0x11,
        MBC3_RAM_2 = 0x12,
        MBC3_RAM_BATTERY_2 = 0x13,
        MBC5 = 0x19,
        MBC5_RAM = 0x1A,
        MBC5_RAM_BATTERY = 0x1B,
        MBC5_RUMBLE = 0x1C,
        MBC5_RUMBLE_RAM = 0x1D,
        MBC5_RUMBLE_RAM_BATTERY = 0x1E,
        MBC6 = 0x20,
        MBC7_SENSOR_RUMBLE_RAM_BATTERY = 0x22,
        POCKET_CAMERA = 0xFC,
        BANDAI_TAMA5 = 0xFD,
        HUC3 = 0xFE,
        HUC1_RAM_BATTERY = 0xFF
    };

    static std::string cartridgeTypeToString(CartridgeType value);

    /**
     * Get the hardware type of the cartridge
     * @return the type retrieved from the cartridge
     */
    CartridgeType getType() const;

  private:
    /**
     * Read the cartridge header from the binary data
     * To retrieve information about the cartridge.
     */
    void readHeader();

    /**
     * Read the title from the header.
     */
    void readTitle();

    /**
     * Read the manufacturer code from the header.
     */
    void readManufacturerCode();

    /**
     * Read the hardware type from the header.
     */
    void readType();

    /**
     * The binary data of the cartridge (ROM/RAM etc.)
     */
    std::vector<byte> data;

    /**
     * The cartridge's title read from the header
     */
    std::string title;

    /**
     * The length of the ascii title (number of characters)
     */
    static const int TITLE_LENGTH = 11;

    /**
     * The start address of the title in the binary data
     */
    static const int TITLE_START_ADDR = 0x0134;

    /**
     * The manufacturer code read from the header
     */
    std::string manufacturerCode;

    /**
     * The length of the ascii manufacturer code (number of characters)
     */
    static const int MANUFACTURER_CODE_LENGTH = 4;

    /**
     * The start address of the manufacturer code in the binary data
     */
    static const int MANUFACTURER_CODE_START_ADDR = 0x013F;

    /**
     * The address of the cartridge type in the binary data
     */
    static const int CARTRIDGE_TYPE_ADDR = 0x0147;

    /**
     * The cartridge type read from the header
     */
    CartridgeType type = CartridgeType::UNKNOWN;
};
#endif // GBEMULATOR_CARTRIDGE_HPP
