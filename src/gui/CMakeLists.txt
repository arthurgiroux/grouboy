set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 COMPONENTS Quick Gui Qml REQUIRED)

qt_add_executable(gbemulator
        main.cpp
        )

qt_add_qml_module(gbemulator
        URI GBQml
        VERSION 1.0
        QML_FILES
        qml/main.qml
        qml/DropFileArea.qml
        SOURCES
        QGBEmulator.hpp
        QGBEmulator.cpp
        TileDebugger.cpp
        TileDebugger.hpp
        ImageItem.cpp
        ImageItem.hpp
        ApplicationData.hpp
        )

target_include_directories(gbemulator PRIVATE ${CMAKE_SOURCE_DIR}/src/)

target_link_libraries(gbemulator PRIVATE gbemulator_core Qt6::Gui Qt6::Quick Qt6::Qml)

set_target_properties(gbemulator PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE TRUE
        )

install(TARGETS gbemulator
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )

qt_generate_deploy_qml_app_script(
        TARGET gbemulator
        FILENAME_VARIABLE deploy_script
)

install(SCRIPT ${deploy_script})