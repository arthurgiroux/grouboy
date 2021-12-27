#include "emulator.hpp"
#include <SDL2/SDL.h>

#undef main

int main(int argc, char** argv) {
	Emulator emulator;
	emulator.exec();
	return 0;
}
