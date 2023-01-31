set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick Gui REQUIRED)

qt_add_executable(gbemulator
        main.cpp
        )

qt_add_qml_module(gbemulator
        URI GBQml
        VERSION 1.0
        QML_FILES
        qml/main.qml
        qml/EmulatorView.qml
        SOURCES
        QGBEmulator.hpp
        QGBEmulator.cpp
        )

target_include_directories(gbemulator PRIVATE ${CMAKE_SOURCE_DIR}/src/)

target_link_libraries(gbemulator PRIVATE gbemulator_core Qt6::Gui Qt6::Quick)
