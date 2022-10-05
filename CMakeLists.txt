cmake_minimum_required(VERSION 3.16)
project(bloomfilter)

set(CMAKE_CXX_FLAGS "${CMAKE_CSS_FLAGS} -std=c++11")

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(extlib)

#target_sources(${PROJECT_NAME} main.cpp)
