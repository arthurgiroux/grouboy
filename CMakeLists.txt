cmake_minimum_required (VERSION 3.0)
project (GBEmulator)

set(SDL_INCLUDE_DIR "" CACHE PATH "SDL")
set(SDL_LIB_DIR "" CACHE PATH "SDL")

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
	src/cpu.cpp
	src/cpu.hpp
	src/emulator.hpp
	src/emulator.cpp
	#src/graphics.cpp
	#src/graphics.hpp
	src/instructions.hpp
	src/mmu.cpp
	src/mmu.hpp
	src/types.hpp
	src/main.cpp
	src/gpu.cpp
	src/gpu.hpp
	)

link_directories(${SDL_LIB_DIR})
add_executable(gbemulator ${SOURCE_FILES})
target_include_directories(gbemulator PUBLIC ${SDL_INCLUDE_DIR})
target_link_libraries(gbemulator SDL2)

add_subdirectory(tests/)