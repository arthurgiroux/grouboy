#ifndef MMU_H
#define MMU_H

#include "types.hpp"

#define TOTAL_MEMORY 65536

class MMU {
public:
	MMU();
	~MMU();

	byte read(const uint16_t& addr);
	void write(const uint16_t& addr, const byte& value);

private:
	byte memory[TOTAL_MEMORY];
	
};

#endif
