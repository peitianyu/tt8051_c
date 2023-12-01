cmake_minimum_required(VERSION 3.3)
project(tt_stc)

SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_C_COMPILER "gcc")
SET(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -std=c++14 -fPIC -pthread")
SET(CMAKE_BUILD_TYPE Release)

add_definitions(-DTEST_ENABLE)

include_directories(src/lib)
file(GLOB_RECURSE ALL_LIBRIRY_SRCS "src/lib/*.c*")

add_executable(ttstc src/main_test.cc ${ALL_LIBRIRY_SRCS})
