#include "mmu.hpp"

MMU::MMU() {

}

byte MMU::read(const uint16_t& addr) {
	return memory[addr];
}

uint16_t MMU::readWord(const uint16_t& addr) {
	return (read(addr+1) << 8) | read(addr);
}

void MMU::write(const uint16_t& addr, const byte& value) {
	memory[addr] = value;
}

void MMU::writeWord(const uint16_t& addr, const uint16_t& value) {
	memory[addr] = value & 0x00FF;
	memory[addr+1] = value >> 8;
}

MMU::~MMU() {

}
