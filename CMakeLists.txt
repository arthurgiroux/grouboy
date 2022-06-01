cmake_minimum_required (VERSION 3.0)
project (GBEmulator)

cmake_policy(SET CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(SDL2_DIR src/SDL)
add_subdirectory(${SDL2_DIR})

set(IMGUI_DIR src/imgui)
add_library(IMGUI STATIC)

target_sources(IMGUI
		PRIVATE
		${IMGUI_DIR}/imgui_demo.cpp
		${IMGUI_DIR}/imgui_draw.cpp
		${IMGUI_DIR}/imgui_tables.cpp
		${IMGUI_DIR}/imgui_widgets.cpp
		${IMGUI_DIR}/imgui.cpp

		PRIVATE
		${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
		${IMGUI_DIR}/backends/imgui_impl_sdl.cpp
)

target_include_directories(IMGUI
		PUBLIC ${IMGUI_DIR}
		PUBLIC ${IMGUI_DIR}/backends
		PUBLIC ${SDL2_DIR}/include
)

if(USE_GLES)
	target_compile_definitions(IMGUI PUBLIC -DIMGUI_IMPL_OPENGL_ES2)
	target_link_libraries(IMGUI PUBLIC GLESv2)
else()
	find_package(OpenGL REQUIRED)
	target_link_libraries(IMGUI PUBLIC ${OPENGL_LIBRARIES})

endif()

target_link_libraries(IMGUI PUBLIC SDL2main SDL2 ${CMAKE_DL_LIBS})

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
	src/cpu.cpp
	src/cpu.hpp
	src/emulator.hpp
	src/emulator.cpp
	src/instructions.hpp
	src/mmu.cpp
	src/mmu.hpp
	src/types.hpp
	src/main.cpp
	src/gpu.cpp
	src/gpu.hpp
	src/tile.cpp
	src/tile.hpp
	src/utils.hpp
	src/GUI.cpp
	src/GUI.hpp)

add_executable(gbemulator ${SOURCE_FILES})
target_link_libraries(gbemulator IMGUI)

enable_testing()
add_subdirectory(tests/)