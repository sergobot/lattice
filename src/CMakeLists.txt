cmake_minimum_required(VERSION 3.12)

add_library(lattice SHARED lattice.h)

set_target_properties(lattice PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(lattice PUBLIC -Wall -Wextra)
