#include "emulator.hpp"
#include "GUI.hpp"

int main(int, char**)
{
    Emulator emulator;
    GUI gui(emulator);

    gui.init();
    gui.startMainLoop();
    gui.uninit();

    return 0;
}