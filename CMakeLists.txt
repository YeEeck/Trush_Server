cmake_minimum_required(VERSION 3.13)
project(Trush_Server)

set(CMAKE_CXX_STANDARD 11)

add_executable(Trush_Server main.cpp)
target_link_libraries(Trush_Server ws2_32)

set(CMAKE_EXE_LINKER_FLAGS "-static")