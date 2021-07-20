cmake_minimum_required(VERSION 3.19)
project(Text_Editor)

set(CMAKE_CXX_STANDARD 14)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(Text_Editor main.cpp Buffer.cpp Buffer.h Editor.cpp Editor.h)
target_link_libraries(Text_Editor ${CURSES_LIBRARY})
