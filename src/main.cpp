#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include "emulator.hpp"
#include "gui/emulator_sdl_gui.hpp"
#include <iostream>
#include <mutex>

#ifdef __EMSCRIPTEN__
void emscriptenLoop(void* fct)
{
    EmulatorSDLGUI* gui = reinterpret_cast<EmulatorSDLGUI*>(fct);
    if (gui->shouldQuit())
    {
        emscripten_cancel_main_loop();
        gui->destroy();
    }
    else
    {
        std::cout << "loop" << std::endl;
        gui->mainLoop();
    }
}
#endif

int main(int argc, char* args[])
{
    const std::string file = args[1];
    Emulator emulator;
    std::cout << "Loading " << file << std::endl;
    if (!emulator.getMMU().loadCartridge(file))
    {
        std::cout << "Couldn't load rom file." << std::endl;
        return EXIT_FAILURE;
    }

    EmulatorSDLGUI gui(emulator);

#ifdef __EMSCRIPTEN__
    gui.enableAudio(false);
#endif

    if (!gui.create())
    {
        std::cerr << "Error while creating GUI." << std::endl;
        return EXIT_FAILURE;
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(emscriptenLoop, &gui, 0, 1);
#else
    while (!gui.shouldQuit())
    {
        gui.mainLoop();
    }

    gui.destroy();
#endif

    return 0;
}
