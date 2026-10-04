cmake_minimum_required(VERSION 4.0)
project(Termia)

set(CMAKE_CXX_STANDARD 23)

add_library(TermiaCore STATIC
        CORE/Chart.cpp
        CORE/Chart.h
        CORE/Note.cpp
        CORE/Note.h
        CORE/Hold.cpp
        CORE/Hold.h
        CORE/GameObject.cpp
        CORE/GameObject.h
        CORE/TermiaConfig.cpp
        CORE/TermiaConfig.h
)

target_include_directories(TermiaCore
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(Termia
        main.cpp

        UI/ConsoleConstants.h
        UI/TUIManager.cpp
        UI/TUIManager.h
        UI/TUIMenu.cpp
        UI/TUIMenu.h
        UI/Menus/Menu_Main.cpp
        UI/Menus/Menu_Main.h
        UI/TerminalHelper.cpp
        UI/TerminalHelper.h
        UI/Menus/Menu_SongSelect.cpp
        UI/Menus/Menu_SongSelect.h
        UI/Menus/Menu_Options.cpp
        UI/Menus/Menu_Options.h
        UI/Menus/Menu_Play.cpp
        UI/Menus/Menu_Play.h

        Game/ChartManager.cpp
        Game/ChartManager.h
        miniaudio.h
        Game/AudioManager.cpp
        Game/AudioManager.h
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEVDEV REQUIRED IMPORTED_TARGET libevdev)

target_include_directories(Termia PRIVATE ${LIBEVDEV_INCLUDE_DIRS})
target_link_libraries(Termia PRIVATE TermiaCore ${LIBEVDEV_LIBRARIES})