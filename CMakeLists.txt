cmake_minimum_required(VERSION 3.10)
project(distributed-n-coloring-graph)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(common)
add_subdirectory(distributed)
add_subdirectory(regular)