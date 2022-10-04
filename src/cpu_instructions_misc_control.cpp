#include "cpu.hpp"

void CPU::NoOperation()
{
	lastInstructionTicks = 1;
}

void CPU::enableInterrupts()
{
    interruptsEnabledRequested = true;
	lastInstructionTicks = 1;
}

void CPU::disableInterrupts()
{
    interruptsEnabledRequested = false;
	interruptsEnabled = false;
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