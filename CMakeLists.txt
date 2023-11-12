cmake_minimum_required(VERSION 3.20)

project(regex)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE C_FILES src/*.c)

add_executable(main ${CPP_FILES} ${C_FILES})

target_include_directories(main PRIVATE src)

target_link_libraries(main)
target_compile_options(main PRIVATE -g -Wall -Wextra -pedantic -Wno-unused-parameter)