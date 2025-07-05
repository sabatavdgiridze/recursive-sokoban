cmake_minimum_required(VERSION 3.31)
project(recursive_sokoban)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/raylib/include)
link_directories(${CMAKE_SOURCE_DIR}/raylib/lib)

add_executable(recursive_sokoban main.cpp
        Chaikin.cpp
        Chaikin.h
        Board.cpp
        Board.h
        Animation.cpp
        Animation.h
        InputManager.cpp
        InputManager.h)

target_link_libraries(recursive_sokoban raylib opengl32 gdi32 winmm)