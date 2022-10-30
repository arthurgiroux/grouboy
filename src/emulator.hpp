#ifndef EMULATOR_H
#define EMULATOR_H

#include "cpu.hpp"
#include "input_controller.hpp"
#include "mmu.hpp"
#include "ppu.hpp"

class Emulator
{
  public:
	Emulator();
	~Emulator();
	void exec();

	PPU& getPPU()
	{
		return ppu;
	}

	MMU& getMMU()
	{
		return mmu;
	}

	CPU& getCPU()
	{
		return cpu;
	}

	InputController& getInputController()
	{
		return inputController;
	}

	int getCurrentTicks() const
	{
		return currentTicks;
	}

  private:
	MMU mmu;
	CPU cpu;
	PPU ppu;
	InputController inputController;
	int currentTicks = 0;
};

#endif
