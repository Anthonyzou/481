cmake_minimum_required(VERSION 2.8)
project(asn2)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/release")

set(Boost_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/boost/build/lib")
set(Boost_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/boost/build/include")
set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/boost/build")
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_STATIC_LIBS ON)

set(MPI_CXX_COMPILER "${CMAKE_CURRENT_SOURCE_DIR}/mpich-3.2/build/bin/mpicc")
set(MPI_CXX_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/mpich-3.2/build/include")
file(GLOB MPI_CXX_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/mpich-3.2/build/lib/*.so")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/mpich-3.2/build/include")

find_package(MPI REQUIRED)
find_package(Boost COMPONENTS mpi serialization REQUIRED)

message("BOOST " ${Boost_LIBRARIES})
message("MPI " ${MPI_CXX_LIBRARIES})

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g") # -g is in default config
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3") # -O3 is in default config
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(asn2 ${SOURCE_FILES})
target_link_libraries(asn2 ${MPI_CXX_LIBRARIES})
target_link_libraries(asn2 ${Boost_LIBRARIES})
