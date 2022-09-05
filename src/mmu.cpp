#include "mmu.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
#include <memory>

const std::array<byte, 256> MMU::BIOS = {
    0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E, 0x11, 0x3E, 0x80,
    0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0, 0x47, 0x11, 0x04, 0x01, 0x21, 0x10,
    0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B, 0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08,
    0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9, 0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28,
    0x08, 0x32, 0x0D, 0x20, 0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40,
    0x04, 0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2, 0x0E, 0x13,
    0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06, 0x7B, 0xE2, 0x0C, 0x3E, 0x87,
    0xF2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20, 0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04,
    0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17, 0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66,
    0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89,
    0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD,
    0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3c, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x4C, 0x21, 0x04, 0x01, 0x11,
    0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20, 0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05,
    0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50};

MMU::MMU()
{
	memory.fill(0);
	std::copy(BIOS.begin(), BIOS.end(), memory.begin());
}

byte MMU::read(const uint16_t& addr)
{
	if (addr >= MEMORY_SIZE_IN_BYTES)
	{
		throw InvalidMemoryAccessException();
	}

	return memory[addr];
}

uint16_t MMU::readWord(const uint16_t& addr)
{
	if (addr >= MEMORY_SIZE_IN_BYTES - 1)
	{
		throw InvalidMemoryAccessException();
	}

	return uint16_t(read(addr + 1) << 8u) | read(addr);
}

void MMU::write(const uint16_t& addr, const byte& value)
{
	if (addr >= MEMORY_SIZE_IN_BYTES)
	{
		throw InvalidMemoryAccessException();
	}

	memory[addr] = value;
}

void MMU::writeWord(const uint16_t& addr, const uint16_t& value)
{
	if (addr >= MEMORY_SIZE_IN_BYTES - 1)
	{
		throw InvalidMemoryAccessException();
	}

	memory[addr] = value & 0x00FFu;
	memory[addr + 1] = value >> 8u;
}

bool MMU::loadROM(const std::string& filepath)
{
	std::ifstream input(filepath, std::ios::binary);
	bool ret = false;
	if (input.good())
	{
		// The BIOS is always present, we only load the ROM after the BIOS
		input.seekg(BIOS.size());
		std::copy(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>(),
		          memory.begin() + BIOS.size());
		ret = true;
	}

	input.close();
	return ret;
}

MMU::~MMU() = default;
