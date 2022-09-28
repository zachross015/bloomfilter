project(bloomfilter)

add_executable(${PROJECT_NAME})

add_subdirectory(extlib)

target_sources(${PROJECT_NAME} main.cpp)
