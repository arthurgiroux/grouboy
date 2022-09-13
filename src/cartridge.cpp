#include "cartridge.hpp"

#include <algorithm>
#include <cstring>
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
	size_t len = std::min(static_cast<int>(strlen(reinterpret_cast<const char*>(startAddr))), TITLE_LENGTH);
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
