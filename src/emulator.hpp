#ifndef EMULATOR_H
#define EMULATOR_H

#include "apu/apu.hpp"
#include "cpu/cpu.hpp"
#include "cpu/input_controller.hpp"
#include "graphics/ppu.hpp"
#include "memory/mmu.hpp"
#include "serial/serial_transfer_manager.hpp"
#include "timer/timer.hpp"

/**
 * Emulator class that wraps up all the different components of the system.
 * This class is in charge of decoding instruction from the MMU and executing them,
 * then updating the PPU in sync with the other components.
 */
class Emulator
{
  public:
    /**
     * Create a new emulator.
     */
    Emulator();

    /**
     * Destroy the emulator.
     */
    ~Emulator();

    /**
     * Execute the next instruction of the emulator and update the PPU.
     */
    void exec();

    /**
     * Resets all the components of the emulator.
     */
    void reset();

    /**
     * @return the Picture Processing Unit of the emulator.
     */
    PPU& getPPU()
    {
        return ppu;
    }

    /**
     * @return the Memory Management Unit of the emulator.
     */
    MMU& getMMU()
    {
        return mmu;
    }

    /**
     * @return the Central Computing Unit of the emulator.
     */
    CPU& getCPU()
    {
        return cpu;
    }

    APU& getAPU()
    {
        return apu;
    }

    /**
     * @return the input controller used by the emulator.
     */
    InputController& getInputController()
    {
        return inputController;
    }

    /**
     * Get the number of CPU ticks elapsed since the start of the emulator.
     *
     * @return 	the number of ticks
     */
    int getCurrentTicks() const
    {
        return currentTicks;
    }

    /**
     * Save the current game state to a file
     *
     * @return true if saving was successful, false otherwise
     */
    bool saveToFile(const std::string& filepath);

    /**
     * Load a save state from a file.
     *
     * @param filepath  the path to the save to load
     * @return True if loading was successful, false otherwise
     */
    bool loadFromFile(const std::string& filepath);

  private:
    static const int AUDIO_SAMPLING_FREQ = 44100;
    MMU mmu;
    CPU cpu;
    PPU ppu;
    InputController inputController;
    Timer timer;
    APU apu;
    int currentTicks = 0;
};

#endif
