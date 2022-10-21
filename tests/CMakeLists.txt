include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
        emulator_tests
        ../src/mmu.cpp
        ../src/mmu.hpp
        ../src/cpu.cpp
        ../src/cpu_instructions_misc_control.cpp
        ../src/cpu_instructions_jumps_calls.cpp
        ../src/cpu_instructions_8bits_load_store_move.cpp
        ../src/cpu_instructions_16bits_load_store_move.cpp
        ../src/cpu_instructions_8bits_arithmetic_logical.cpp
        ../src/cpu_instructions_16bits_arithmetic_logical.cpp
        ../src/cpu_instructions_8bits_rotation_shifts_bit.cpp
        ../src/cpu.hpp
        ../src/gpu.cpp
        ../src/gpu.hpp
        ../src/tile.cpp
        ../src/tile.hpp
        ../src/utils.hpp
        ../src/utils.cpp
        ../src/cartridge.cpp
        ../src/cartridge.hpp
        ../src/interrupt_handler.cpp
        ../src/interrupt_handler.hpp
        ../src/input_controller.cpp
        ../src/input_controller.hpp
        ../src/rgb_image.cpp
        ../src/rgb_image.hpp
        test_mmu.cpp
        test_gpu.cpp
        test_cpu.cpp
        test_tile.cpp
        test_utils.cpp
        test_cpu_instructions_jumps_calls.cpp
        test_cpu_instructions_8bits_load_store_move.cpp
        test_cpu_instructions_8bits_arithmetic_logical.cpp
        test_cpu_instructions_16bits_load_store_move.cpp
        test_cpu_instructions_misc_control.cpp
        test_cpu_instructions_16bits_arithmetic_logical.cpp
        test_cpu_instructions_8bits_rotation_shifts_bit.cpp
        test_cartridge.cpp
        test_input_controller.cpp)

target_compile_definitions(emulator_tests PRIVATE -DUNIT_TESTING)

target_include_directories(emulator_tests PRIVATE ../src/)

target_link_libraries(
        emulator_tests
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(emulator_tests)