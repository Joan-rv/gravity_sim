cmake_minimum_required(VERSION 3.28)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(gravity_sim LANGUAGES CXX)

file(GLOB_RECURSE sources src/*.cpp src/*.h src/*.hpp)

add_executable(gravity_sim ${sources})

target_include_directories(gravity_sim PRIVATE src)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost CONFIG COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
    FIND_PACKAGE_ARGS COMPONENTS graphics window)
FetchContent_MakeAvailable(SFML)

target_link_libraries(gravity_sim sfml-graphics sfml-window ${Boost_LIBRARIES})

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(gravity_sim PRIVATE -Wall -Wpedantic -Wextra)
elseif ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
    target_compile_options(gravity_sim PRIVATE /W4)
endif()
