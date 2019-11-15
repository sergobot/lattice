cmake_minimum_required(VERSION 3.12)
project(lattice)

set(CMAKE_CXX_STANDARD 14)

add_executable(lattice main.cpp square_lattice.cpp square_lattice.h)
target_compile_options(lattice PUBLIC -Wall -Wextra)