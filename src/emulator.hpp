#ifndef EMULATOR_H
#define EMULATOR_H

#include "cpu.hpp"
#include "gpu.hpp"
#include "input_controller.hpp"
#include "mmu.hpp"

class Emulator
{
  public:
	Emulator();
	~Emulator();
	void exec();

	GPU& getGPU()
	{
		return gpu;
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
	GPU gpu;
	InputController inputController;
	int currentTicks = 0;
};

#endif
