cmake_minimum_required(VERSION 3.2)
project(ex2)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wextra -Wvla")

set(SOURCE_FILES main.c)
add_executable(ex2 ${SOURCE_FILES})