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

void CPU::stopInstruction()
{
	// TODO: Stop CPU properly

	// Stop instruction is always followed by a 0 that we need to swallow
	pc++;
	lastInstructionTicks = 1;
}

void CPU::haltInstruction()
{
	// TODO: Halt CPU properly
	halted = true;
	lastInstructionTicks = 1;
}