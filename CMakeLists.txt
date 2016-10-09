cmake_minimum_required (VERSION 3.0)
project (GBEmulator)

set(SOURCE_FILES
	src/cpu.cpp
	src/cpu.hpp
	src/emulator.hpp
	src/graphics.cpp
	src/graphics.hpp
	src/instructions.hpp
	src/mmu.cpp
	src/mmu.hpp
	src/types.hpp
	src/main.cpp
)

add_executable(gbemulator ${SOURCE_FILES})
