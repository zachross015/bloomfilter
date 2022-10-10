cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_FLAGS "${CMAKE_CSS_FLAGS} -std=c++11")

project(bloomfilter)

add_executable(bloomfilter bloomfilter.cpp)

add_subdirectory(extlib)
#target_sources(${PROJECT_NAME} main.cpp)
