cmake_minimum_required(VERSION 4.0)
project(Termia)

set(CMAKE_CXX_STANDARD 23)

add_executable(Termia main.cpp
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
        Game/Chart.cpp
        Game/Chart.h
        Game/Note.cpp
        Game/Note.h
        Game/Hold.cpp
        Game/Hold.h
        Game/GameObject.h
        Game/GameObject.cpp
        Game/ChartManager.cpp
        Game/ChartManager.h
        miniaudio.h
        Game/AudioManager.cpp
        Game/AudioManager.h
        Game/TermiaConfig.cpp
        Game/TermiaConfig.h
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEVDEV REQUIRED IMPORTED_TARGET libevdev)

target_include_directories(Termia PRIVATE ${LIBEVDEV_INCLUDE_DIRS})
target_link_libraries(Termia PRIVATE ${LIBEVDEV_LIBRARIES})