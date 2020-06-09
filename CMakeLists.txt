cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 14)

project(lattice VERSION 0.1 DESCRIPTION "Tiny extensible library to compute percolation thresholds of different lattices"
        LANGUAGES CXX)

add_subdirectory(src lattice)
add_subdirectory(examples examples)
