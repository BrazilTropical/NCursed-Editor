cmake_minimum_required(VERSION 3.19)
project(smk-cursed)

set(CMAKE_CXX_STANDARD 14)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(smk-cursed main.cpp Buffer/Buffer.cpp Buffer/Buffer.h Editor/Editor.cpp Editor/Editor.h)
target_link_libraries(smk-cursed ${CURSES_LIBRARY})
