#ifndef CPU_H
#define CPU_H

#include "types.hpp"
#include "mmu.hpp"


class CPU {
public:
	CPU(MMU* mmu);
	~CPU();
	void exec();

private:

	void process(const byte& opCode);

	// frequency of the CPU, 4.194304MHz
	int frequency;

	// registers
	byte a;
	byte b;
	byte c;
	byte d;
	byte e;
	byte h;
	byte l;
	// flag register
	byte f;

	int tick;

	int lastInstructionTicks;

	// program counter
	uint16_t pc;
	// stack pointer
	uint16_t sp;

	MMU* mmu;
};

#endif
