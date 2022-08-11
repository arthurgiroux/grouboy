#include "cpu.hpp"

void CPU::NoOperation()
{
	lastInstructionTicks = 1;
}

void CPU::enableInterrupts()
{
	// TODO: Review interrupts logic
	ticksBeforeEnablingInterrupts = 2;
	lastInstructionTicks = 1;
}

void CPU::disableInterrupts()
{
	// TODO: Review interrupts logic
	ticksBeforeDisablingInterrupts = 2;
	lastInstructionTicks = 1;
}