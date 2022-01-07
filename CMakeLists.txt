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
#	src/imgui/imgui.cpp
#		src/imgui/imgui_impl_sdl.cpp
#		src/imgui/imgui_impl_sdlrenderer.cpp
#		src/imgui/imgui_impl_win32.cpp
#		src/imgui/imgui_draw.cpp
#		src/imgui/imgui_tables.cpp
#		src/imgui/imgui_widgets.cpp
		src/tile.cpp src/tile.hpp)

link_directories(${SDL_LIB_DIR})
add_executable(gbemulator ${SOURCE_FILES})
target_include_directories(gbemulator PUBLIC ${SDL_INCLUDE_DIR} src/imgui)
target_link_libraries(gbemulator SDL2 SDL2main)

add_subdirectory(tests/)