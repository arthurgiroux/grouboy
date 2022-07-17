#ifndef EMULATOR_H
#define EMULATOR_H

#include "cpu.hpp"
#include "gpu.hpp"
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

	int getCurrentTicks() const
	{
		return currentTicks;
	}

  private:
	MMU mmu;
	CPU cpu;
	GPU gpu;
	int currentTicks = 0;
};

#endif
