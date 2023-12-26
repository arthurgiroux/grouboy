cmake_minimum_required(VERSION 3.16)
project(Grouboy)
add_subdirectory(libs/spdlog-1.11.0)

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
        src/common/types.hpp
        src/graphics/ppu.cpp
        src/graphics/ppu.hpp
        src/graphics/tile.cpp
        src/graphics/tile.hpp
        src/graphics/lcd_status_register.hpp
        src/graphics/lcd_status_register.cpp
        src/common/utils.hpp
        src/common/utils.cpp
        src/memory/mmu.cpp
        src/memory/mmu.hpp
        src/memory/cartridge.cpp
        src/memory/cartridge.hpp
        src/memory/mbc/memory_bank_controller.cpp
        src/memory/mbc/memory_bank_controller.hpp
        src/memory/mbc/mbc_rom_only.hpp
        src/memory/mbc/mbc_rom_only.cpp
        src/memory/mbc/mbc1.hpp
        src/memory/mbc/mbc1.cpp
        src/memory/mbc/mbc2.cpp
        src/memory/mbc/mbc2.hpp
        src/memory/mbc/mbc3.cpp
        src/memory/mbc/mbc3.hpp
        src/cpu/interrupt_handler.cpp
        src/cpu/interrupt_handler.hpp
        src/cpu/input_controller.cpp
        src/cpu/input_controller.hpp
        src/graphics/rgb_image.cpp
        src/graphics/rgb_image.hpp
        src/graphics/palette.cpp
        src/graphics/palette.hpp
        src/graphics/sprite.cpp
        src/graphics/sprite.hpp
        src/serial/serial_transfer_manager.cpp
        src/serial/serial_transfer_manager.hpp
        src/timer/timer.cpp
        src/timer/timer.hpp
        src/cpu/interrupt_manager.cpp
        src/cpu/interrupt_manager.hpp
        src/graphics/tilemap.cpp
        src/graphics/tilemap.hpp
        src/apu/apu.cpp
        src/apu/apu.hpp
        src/apu/signal/square_wave.cpp
        src/apu/signal/square_wave.hpp
        src/apu/channels/channel.cpp
        src/apu/channels/channel.hpp
        src/apu/channels/pulse_channel.hpp
        src/apu/channels/pulse_channel.cpp
        src/apu/channels/channel1.hpp
        src/apu/channels/channel1.cpp
        src/apu/channels/channel2.cpp
        src/apu/channels/channel2.hpp
        src/apu/channels/frequency_sweep.cpp
        src/apu/channels/frequency_sweep.hpp
        src/apu/channels/volume_sweep.cpp
        src/apu/channels/volume_sweep.hpp
        src/apu/channels/length_timer.cpp
        src/apu/channels/length_timer.hpp
        src/apu/channels/frame_sequencer.cpp
        src/apu/channels/frame_sequencer.hpp
        src/apu/signal/falling_edge_detector.cpp
        src/apu/signal/falling_edge_detector.hpp
        src/apu/channels/audio_mixer.cpp
        src/apu/channels/audio_mixer.hpp
        src/apu/channels/channel3.cpp
        src/apu/channels/channel3.hpp
        src/apu/signal/wave.cpp
        src/apu/signal/wave.hpp
        src/apu/channels/channel4.cpp
        src/apu/channels/channel4.hpp
        src/apu/signal/noise_signal.cpp
        src/apu/signal/noise_signal.hpp
        src/apu/filters/highpass_filter.cpp
        src/apu/filters/highpass_filter.hpp
        src/graphics/pixel_fifo.cpp
        src/graphics/pixel_fifo.hpp
        src/graphics/pixel.cpp
        src/graphics/pixel.hpp
        src/memory/bootrom.hpp)

add_library(gbemulator_core ${SOURCE_FILES})
target_include_directories(gbemulator_core PRIVATE src/ libs/spdlog-1.11.0/include)
target_link_libraries(gbemulator_core spdlog::spdlog)

if (MSVC)
    target_compile_options(gbemulator_core PRIVATE /W4 /WX)
else ()
    target_compile_options(gbemulator_core PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif ()

set(SDL3_DIR ${CMAKE_SOURCE_DIR}/libs/SDL)
add_subdirectory(${SDL3_DIR})

add_executable(grouboy src/main.cpp)

target_include_directories(grouboy PRIVATE ${CMAKE_SOURCE_DIR}/src/)

add_dependencies(grouboy gbemulator_core SDL3::SDL3)
target_link_libraries(grouboy PRIVATE gbemulator_core SDL3::SDL3)

enable_testing()
add_subdirectory(tests/)