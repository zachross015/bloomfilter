cmake_minimum_required(VERSION 3.16)
project(bloomfilter)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(extlib)

#target_sources(${PROJECT_NAME} main.cpp)
