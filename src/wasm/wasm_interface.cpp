#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

#include "emulator.hpp"
#include "gui/emulator_sdl_gui.hpp"
#include "wasm_interface.hpp"
#include <iostream>

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
        gui->mainLoop();
    }
}

extern "C"
{
    EMSCRIPTEN_KEEPALIVE void startEmulator()
    {
        Emulator emulator;
        const char* file = "tictactoe.gb";
        if (!emulator.getMMU().loadCartridge(file))
        {
            std::cout << "Couldn't load rom file." << std::endl;
            return;
        }

        EmulatorSDLGUI gui(emulator);

        gui.enableAudio(false);

        if (!gui.create())
        {
            std::cerr << "Error while creating GUI." << std::endl;
            return;
        }

        emscripten_set_main_loop_arg(emscriptenLoop, &gui, 0, 1);

        gui.destroy();
    }
}