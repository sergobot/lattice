cmake_minimum_required(VERSION 3.12)

add_executable(example main.cpp)
target_include_directories(example PRIVATE "../src")
target_link_libraries(example lattice)