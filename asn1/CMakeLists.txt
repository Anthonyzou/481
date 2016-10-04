project(asn1)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp)
add_executable(asn1 ${SOURCE_FILES})

set(SOURCE_FILES_TEST test.cpp)
add_executable(asn1test ${SOURCE_FILES_TEST})

add_custom_target(bench COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/benchmark.sh")