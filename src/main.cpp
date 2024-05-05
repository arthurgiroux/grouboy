#include "emulator.hpp"
#include "gui/emulator_sdl_gui.hpp"
#include <iostream>

int main(int argc, char* args[])
{
    const std::string file = args[1];
    Emulator emulator;
    if (!emulator.getMMU().loadCartridgeFromFile(file))
    {
        std::cout << "Couldn't load rom file." << std::endl;
        return EXIT_FAILURE;
    }

    EmulatorSDLGUI gui(emulator);

    if (!gui.create())
    {
        std::cerr << "Error while creating GUI." << std::endl;
        return EXIT_FAILURE;
    }

    while (!gui.shouldQuit())
    {
        gui.mainLoop();
    }

    gui.destroy();

    return 0;
}
