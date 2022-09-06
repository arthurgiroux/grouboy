#include "GUI.hpp"
#include "emulator.hpp"
#include <iostream>

int main(int argc, char** argv)
{
	Emulator emulator;

	if (argc > 1)
	{
		bool cartridgeLoaded = emulator.getMMU().loadCartridge(argv[1]);
		if (!cartridgeLoaded)
		{
			std::cerr << "Couldn't load cartridge" << std::endl;
			return EXIT_FAILURE;
		}
	}
	else
	{
		std::cerr << "Missing argument, you should provide a binary ROM as the first argument." << std::endl;
		return EXIT_FAILURE;
	}

	GUI gui(emulator);

	gui.init();
	gui.startMainLoop();
	gui.uninit();

	return EXIT_SUCCESS;
}