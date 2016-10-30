cmake_minimum_required(VERSION 3.0.2)

project("TimSort")

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")# -O2") # -g -D_GLIBCXX_DEBUG -DDEBUG")
include_directories("include")

set(TIMSORT_SOURCES src/utils.cpp src/stdTimSortParams.cpp)

add_executable(timsort src/main.cpp ${TIMSORT_SOURCES})
add_executable(tests src/tests.cpp src/mainTest.cpp ${TIMSORT_SOURCES})
add_executable(testList src/testList.cpp)
