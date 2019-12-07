cmake_minimum_required(VERSION 3.15)
project(omp1)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES main.cpp)
add_executable(omp1 main.cpp structtimeval.cpp structtimeval.h)

find_package(OpenMP)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
