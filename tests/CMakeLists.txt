cmake_minimum_required(VERSION 3.12)

add_executable(test main.cpp)
target_include_directories(test PRIVATE "../src")
target_link_libraries(test lattice)