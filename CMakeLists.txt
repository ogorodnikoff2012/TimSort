cmake_minimum_required(VERSION 3.0.2)

project("TimSort")

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g")
include_directories("include")

add_executable(timsort src/main.cpp src/utils.cpp)
