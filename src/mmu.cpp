#include <cassert>
#include <memory>
#include <cstring>
#include "mmu.hpp"

MMU::MMU() {
	memset(memory, 0, sizeof(memory));
	memcpy(memory, bios, sizeof(bios));
}

byte MMU::read(const uint16_t& addr) {
	assert(addr < (TOTAL_MEMORY - 1));
	return memory[addr];
}

uint16_t MMU::readWord(const uint16_t& addr) {
	return (read(addr+1) << 8) | read(addr);
}

void MMU::write(const uint16_t& addr, const byte& value) {
	assert(addr < (TOTAL_MEMORY - 1));
	memory[addr] = value;
}

void MMU::writeWord(const uint16_t& addr, const uint16_t& value) {
	assert(addr < (TOTAL_MEMORY - 2));
	memory[addr] = value & 0x00FF;
	memory[addr+1] = value >> 8;
}

MMU::~MMU() {

}
