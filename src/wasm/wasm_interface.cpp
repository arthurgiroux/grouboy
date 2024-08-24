#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

#include "emulator.hpp"
#include "gui/emulator_sdl_gui.hpp"
#include "wasm_interface.hpp"
#include <iostream>

struct EmulatorImpl
{
    std::unique_ptr<Emulator> emulator;
    std::unique_ptr<EmulatorSDLGUI> gui;
};

void emscriptenLoop(void* param)
{
    EmulatorImpl* impl = reinterpret_cast<EmulatorImpl*>(param);

    if (impl->gui->shouldQuit())
    {
        emscripten_cancel_main_loop();
        impl->gui->destroy();
    }
    else
    {
        impl->gui->mainLoop();
    }
}

extern "C"
{

    EMSCRIPTEN_KEEPALIVE EmulatorImpl* init()
    {
        EmulatorImpl* impl = new EmulatorImpl;
        impl->emulator = std::make_unique<Emulator>();
        impl->gui = std::make_unique<EmulatorSDLGUI>(*impl->emulator);
        if (!impl->gui->create())
        {
            std::cerr << "Couldn't create gui" << std::endl;
            destroy(impl);
            return nullptr;
        }

        return impl;
    }

    EMSCRIPTEN_KEEPALIVE void destroy(EmulatorImpl* emulator)
    {
        delete emulator;
    }

    EMSCRIPTEN_KEEPALIVE bool loadROM(EmulatorImpl* emulator, const unsigned char* data, int length)
    {
        std::vector<byte> dataVector(data, data + length);

        if (!emulator->emulator->getMMU().loadCartridgeData(dataVector))
        {
            return false;
        }

        return true;
    }

    EMSCRIPTEN_KEEPALIVE void start(EmulatorImpl* emulator)
    {
        emscripten_set_main_loop_arg(emscriptenLoop, emulator, 0, 1);

        emulator->gui->destroy();
    }
}