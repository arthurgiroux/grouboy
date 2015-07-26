#include "mmu.hpp"

MMU::MMU() {

}

byte MMU::read(const uint16_t& addr) {
	return memory[addr];
}

void MMU::write(const uint16_t& addr, const byte& value) {
	memory[addr] = value;
}

MMU::~MMU() {

}
