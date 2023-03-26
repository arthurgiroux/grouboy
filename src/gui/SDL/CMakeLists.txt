add_executable(main_sdl
        main.cpp)

target_include_directories(main_sdl PRIVATE ${CMAKE_SOURCE_DIR}/src/)

add_dependencies(main_sdl gbemulator_core SDL3::SDL3)
target_link_libraries(main_sdl PRIVATE gbemulator_core SDL3::SDL3)
