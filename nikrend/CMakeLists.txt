cmake_minimum_required(VERSION 4.0...4.4.3)
project(nikrend)

find_package(OpenMP REQUIRED)
add_executable(nikrend main.cpp)

target_include_directories(nikrend PRIVATE src/)
target_compile_options(nikrend PRIVATE -Wall -Wextra -g)
target_link_options(nikrend PRIVATE)
target_link_libraries(nikrend PRIVATE ncurses OpenMP::OpenMP_CXX)
