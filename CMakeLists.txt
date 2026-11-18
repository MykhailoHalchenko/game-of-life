cmake_minimum_required(VERSION 3.10)
project(GameOfLife CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(game_of_life ${SOURCES})
target_include_directories(game_of_life PRIVATE ${CMAKE_SOURCE_DIR}/include)
if(NOT MSVC)
    target_compile_options(game_of_life PRIVATE -Wall -Wextra)
endif()
