cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(gravity_sim LANGUAGES CXX)

file(GLOB_RECURSE sources src/*.cpp src/*.h src/*.hpp)

add_executable(gravity_sim ${sources})

target_include_directories(gravity_sim PRIVATE src)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    FIND_PACKAGE_ARGS COMPONENTS graphics window system)
FetchContent_MakeAvailable(SFML)
target_link_libraries(gravity_sim sfml-graphics sfml-window sfml-system)

if ( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options(main PRIVATE -Wall -Wextra)
endif()
if ( MSVC )
    target_compile_options(main PRIVATE /W4)
endif()
