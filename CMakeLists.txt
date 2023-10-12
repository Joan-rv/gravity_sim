cmake_minimum_required(VERSION 3.16)
project(gravity_sim LANGUAGES CXX)

file(GLOB_RECURSE sources src/*.cpp src/*.h src/*.hpp)

add_executable(gravity_sim ${sources})

find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system)
target_link_libraries(gravity_sim sfml-graphics sfml-window sfml-system)