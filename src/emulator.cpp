#include "emulator.hpp"
#include <fstream>

Emulator::Emulator() : cpu(mmu), ppu(mmu, cpu.getInterruptManager()), timer(&mmu, cpu.getInterruptManager())
{
    mmu.setInputController(&inputController);
}

Emulator::~Emulator() = default;

void Emulator::exec()
{
    // Execute current cpu instruction and retrieve the number of ticks
    int lastInstructionTicks = cpu.fetchDecodeAndExecute();

    timer.tick(lastInstructionTicks);

    ppu.step(lastInstructionTicks);

    currentTicks += lastInstructionTicks;
}

void Emulator::reset()
{
    ppu.reset();
    cpu.reset();
    mmu.reset();
    currentTicks = 0;
}

bool Emulator::saveToFile(const std::string& filepath)
{
    auto data = mmu.serializeCartridgeRAM();
    if (data.size() == 0)
    {
        return true;
    }

    std::ofstream output(filepath, std::ios::out | std::ios::binary);
    if (output)
    {
        output.write(reinterpret_cast<const char*>(data.data()), data.size());
        output.close();
        return output.good();
    }
    return false;
}

bool Emulator::loadFromFile(const std::string& filepath)
{
    std::vector<byte> data;
    if (!utils::readBinaryDataFromFile(filepath, data)) {
        return false;
    }

    return mmu.unserializeCartridgeRAM(data);
}
