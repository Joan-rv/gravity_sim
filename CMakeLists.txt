cmake_minimum_required(VERSION 3.16)
project(gravity_sim LANGUAGES CXX)

file(GLOB_RECURSE sources src/*.cpp src/*.h src/*.hpp)

add_executable(gravity_sim ${sources})

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    FIND_PACKAGE_ARGS COMPONENTS graphics window system)
FetchContent_MakeAvailable(SFML)
target_link_libraries(gravity_sim sfml-graphics sfml-window sfml-system)