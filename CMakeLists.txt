cmake_minimum_required(VERSION 3.15)
project(AOCcw2 C)

set(CMAKE_C_STANDARD 11 -pthread)

file(GLOB SOURCES *.h *c)

add_executable(AOCcw2 part1.c part1_test.c)