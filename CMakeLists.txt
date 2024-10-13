cmake_minimum_required(VERSION 3.13)

include("$ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake")
project(Coursework C CXX ASM)

pico_sdk_init()

add_executable(out Coursework.c)
target_link_libraries(out pico_stdlib)

