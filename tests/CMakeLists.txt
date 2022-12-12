include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(../src)

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
        test_cpu.cpp
        test_cpu_instructions_jumps_calls.cpp
        test_cpu_instructions_8bits_load_store_move.cpp
        test_cpu_instructions_8bits_arithmetic_logical.cpp
        test_cpu_instructions_16bits_load_store_move.cpp
        test_cpu_instructions_misc_control.cpp
        test_cpu_instructions_16bits_arithmetic_logical.cpp
        test_cpu_instructions_8bits_rotation_shifts_bit.cpp
        test_blargg.cpp)

# TODO: Set this using environment variable, CMake seems to have issues with this right now
target_compile_definitions(cpu_tests PRIVATE -DDATADIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

target_link_libraries(
        cpu_tests
        gbemulator_core
        gtest_main
)

add_executable(
        ppu_tests
        test_ppu.cpp
        test_tile.cpp
        test_rgb_image.cpp
        test_sprite.cpp
)

target_link_libraries(
        ppu_tests
        gbemulator_core
        gtest_main
)

add_executable(
        mmu_tests
        test_mmu.cpp
        test_cartridge.cpp
        test_input_controller.cpp
)

target_link_libraries(
        mmu_tests
        gbemulator_core
        gtest_main
)

add_executable(
        serial_tests
        test_serial.cpp
)

target_link_libraries(
        serial_tests
        gbemulator_core
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(utils_tests)
gtest_discover_tests(cpu_tests PROPERTIES TIMEOUT 30)
gtest_discover_tests(ppu_tests)
gtest_discover_tests(mmu_tests)
gtest_discover_tests(serial_tests)