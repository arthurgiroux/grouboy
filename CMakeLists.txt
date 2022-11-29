cmake_minimum_required(VERSION 3.0)
project(GBEmulator)

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

if (USE_GLES)
    target_compile_definitions(IMGUI PUBLIC -DIMGUI_IMPL_OPENGL_ES2)
    target_link_libraries(IMGUI PUBLIC GLESv2)
else ()
    find_package(OpenGL REQUIRED)
    target_link_libraries(IMGUI PUBLIC ${OPENGL_LIBRARIES})

endif ()

target_link_libraries(IMGUI PUBLIC SDL2main SDL2 ${CMAKE_DL_LIBS})

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        src/cpu/cpu.cpp
        src/cpu/cpu_instructions_misc_control.cpp
        src/cpu/cpu_instructions_jumps_calls.cpp
        src/cpu/cpu_instructions_8bits_load_store_move.cpp
        src/cpu/cpu_instructions_16bits_load_store_move.cpp
        src/cpu/cpu_instructions_8bits_arithmetic_logical.cpp
        src/cpu/cpu_instructions_16bits_arithmetic_logical.cpp
        src/cpu/cpu_instructions_8bits_rotation_shifts_bit.cpp
        src/cpu/cpu.hpp
        src/emulator.hpp
        src/emulator.cpp
        src/cpu/instructions.hpp
        src/memory/mmu.cpp
        src/memory/mmu.hpp
        src/common/types.hpp
        src/graphics/ppu.cpp
        src/graphics/ppu.hpp
        src/graphics/tile.cpp
        src/graphics/tile.hpp
        src/common/utils.hpp
        src/common/utils.cpp
        src/gui/GUI.cpp
        src/gui/GUI.hpp
        src/memory/cartridge.cpp
        src/memory/cartridge.hpp
        src/cpu/interrupt_handler.cpp
        src/cpu/interrupt_handler.hpp
        src/cpu/input_controller.cpp
        src/cpu/input_controller.hpp
        src/graphics/rgb_image.cpp
        src/graphics/rgb_image.hpp
        src/graphics/sprite.cpp
        src/graphics/sprite.hpp
        src/serial/serial_transfer_manager.cpp
        src/serial/serial_transfer_manager.hpp)

add_library(gbemulator_core ${SOURCE_FILES})
target_include_directories(gbemulator_core PRIVATE src/)
target_link_libraries(gbemulator_core IMGUI)

add_executable(gbemulator src/main.cpp)
target_include_directories(gbemulator PRIVATE src/)
target_link_libraries(gbemulator gbemulator_core)

enable_testing()
add_subdirectory(tests/)