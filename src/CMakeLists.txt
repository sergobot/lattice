cmake_minimum_required(VERSION 3.12)

add_library(lattice SHARED
        lattice.h
        node.h
        threshold_finder.cpp threshold_finder.h
        square_lattice.cpp square_lattice.h
        triangular_lattice.cpp triangular_lattice.h
        hexagonal_lattice.cpp hexagonal_lattice.h edge.h)

set_target_properties(lattice PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(lattice PUBLIC -Wall -Wextra)
target_link_libraries(lattice -pthread)
