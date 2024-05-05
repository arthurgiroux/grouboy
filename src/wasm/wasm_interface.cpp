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
    EMSCRIPTEN_KEEPALIVE void startEmulatorFromData(const unsigned char* data, int length)
    {
        std::vector<byte> dataVector(data, data + length);

        Emulator emulator;
        if (!emulator.getMMU().loadCartridgeData(dataVector))
        {
            std::cout << "Couldn't load rom file." << std::endl;
            return;
        }

        std::cout << "Rom loaded, starting GUI" << std::endl;
        EmulatorSDLGUI gui(emulator);

        if (!gui.create())
        {
            std::cerr << "Error while creating GUI." << std::endl;
            return;
        }

        emscripten_set_main_loop_arg(emscriptenLoop, &gui, 0, 1);

        gui.destroy();
    }

    EMSCRIPTEN_KEEPALIVE void startEmulatorFromDefaultFile()
    {
        Emulator emulator;
        const char* file = "tictactoe.gb";
        if (!emulator.getMMU().loadCartridgeFromFile(file))
        {
            std::cout << "Couldn't load rom file." << std::endl;
            return;
        }

        EmulatorSDLGUI gui(emulator);

        if (!gui.create())
        {
            std::cerr << "Error while creating GUI." << std::endl;
            return;
        }

        emscripten_set_main_loop_arg(emscriptenLoop, &gui, 0, 1);

        gui.destroy();
    }
}