include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        emulator_tests
        ../src/mmu.cpp
        ../src/mmu.hpp
        ../src/cpu.cpp
        ../src/cpu.hpp
        ../src/gpu.cpp
        ../src/gpu.hpp
        test_mmu.cpp
        test_gpu.cpp
        test_cpu.cpp
)

target_compile_definitions(emulator_tests PRIVATE -DUNIT_TESTING)

target_include_directories(emulator_tests PRIVATE ../src/)

target_link_libraries(
        emulator_tests
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(emulator_tests)