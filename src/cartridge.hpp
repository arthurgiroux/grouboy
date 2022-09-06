#ifndef GBEMULATOR_CARTRIDGE_HPP
#define GBEMULATOR_CARTRIDGE_HPP

#include "utils.hpp"
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
};
#endif // GBEMULATOR_CARTRIDGE_HPP
