cmake_minimum_required(VERSION 3.15)
project(AOCcw2 C)

set(CMAKE_C_STANDARD 11)
set(-pthread)
file(GLOB SOURCES *.h *c)

add_executable(AOCcw2 part2.c part2_test.c)