#ifndef MMU_H
#define MMU_H

#include <array>
#include <stdexcept>

#include "types.hpp"

/***********************************
            MEMORY LAYOUT

 [ BIOS | ROM BANK 0 | ROM BANK 1 | GPU VRAM | Ext. RAM | Working RAM | Sprites info | I/O | ZRAM ]
 0     256b   8k    16k    24k   32k        40k        48k           56k                         64k


 ***********************************/


class MMU
{
  public:
	MMU();
	~MMU();

	byte read(const uint16_t& addr);
	uint16_t readWord(const uint16_t& addr);
	void write(const uint16_t& addr, const byte& value);
	void writeWord(const uint16_t& addr, const uint16_t& value);
	bool loadROM(const std::string& filepath);

    static const size_t MEMORY_SIZE_IN_BYTES = 65536;
    static const std::array<byte, 256> BIOS;

	class InvalidMemoryAccessException : public std::exception
	{
	  public:
		const char* what() const noexcept override
		{
			return "Invalid memory access.";
		}
	};

  private:
	std::array<byte, MEMORY_SIZE_IN_BYTES> memory{};
};

#endif
