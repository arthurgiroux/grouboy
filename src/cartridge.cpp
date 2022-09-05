#include "cartridge.hpp"

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
