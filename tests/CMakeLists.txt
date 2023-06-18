include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(../src)
include_directories(../libs)

# TODO: Set this using environment variable, CMake seems to have issues with this right now
add_compile_definitions(DATADIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

add_executable(
        utils_tests
        test_utils.cpp
)

target_link_libraries(
        utils_tests
        gbemulator_core
        gtest_main
)

add_executable(
        cpu_tests
        cpu/test_cpu.cpp
        cpu/test_cpu_instructions_jumps_calls.cpp
        cpu/test_cpu_instructions_8bits_load_store_move.cpp
        cpu/test_cpu_instructions_8bits_arithmetic_logical.cpp
        cpu/test_cpu_instructions_16bits_load_store_move.cpp
        cpu/test_cpu_instructions_misc_control.cpp
        cpu/test_cpu_instructions_16bits_arithmetic_logical.cpp
        cpu/test_cpu_instructions_8bits_rotation_shifts_bit.cpp
        apu/test_palette.cpp)

target_link_libraries(
        cpu_tests
        gbemulator_core
        gtest_main
)

add_executable(
        blargg_tests
        test_blargg.cpp)

target_link_libraries(
        blargg_tests
        gbemulator_core
        gtest_main
)

add_executable(
        mooneye_tests
        cpu/test_mooneye.cpp)

target_link_libraries(
        mooneye_tests
        gbemulator_core
        gtest_main
)

add_executable(
        ppu_tests
        ppu/test_ppu.cpp
        ppu/test_tile.cpp
        ppu/test_rgb_image.cpp
        ppu/test_sprite.cpp
)

target_link_libraries(
        ppu_tests
        gbemulator_core
        gtest_main
)

add_executable(
        mmu_tests
        mmu/test_mmu.cpp
        cpu/test_cartridge.cpp
        test_input_controller.cpp
        mmu/test_mbc.cpp)

target_link_libraries(
        mmu_tests
        gbemulator_core
        gtest_main
)

add_executable(
        serial_tests
        cpu/test_serial.cpp
)

target_link_libraries(
        serial_tests
        gbemulator_core
        gtest_main
)

add_executable(
        timer_tests
        mmu/test_timer.cpp
)

target_link_libraries(
        timer_tests
        gbemulator_core
        gtest_main
)

add_executable(
        acid_tests
        ppu/test_acid.cpp
)

target_link_libraries(
        acid_tests
        gbemulator_core
        gtest_main
)


add_executable(
        apu_tests
        apu/test_wavelength_sweep.cpp
        apu/test_square_wave.cpp
        apu/test_volume_sweep.cpp
        apu/test_length_timer.cpp
        apu/test_frame_sequencer.cpp
        apu/test_falling_edge_detector.cpp
        apu/test_audio_mixer.cpp
        apu/test_wave.cpp)

target_link_libraries(
        apu_tests
        gbemulator_core
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(utils_tests)
gtest_discover_tests(cpu_tests PROPERTIES TIMEOUT 60)
gtest_discover_tests(blargg_tests PROPERTIES TIMEOUT 60)
gtest_discover_tests(ppu_tests)
gtest_discover_tests(mmu_tests)
gtest_discover_tests(serial_tests)
gtest_discover_tests(timer_tests)
gtest_discover_tests(acid_tests)
gtest_discover_tests(mooneye_tests)
gtest_discover_tests(apu_tests)