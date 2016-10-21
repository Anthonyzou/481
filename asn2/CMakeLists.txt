cmake_minimum_required(VERSION 3.6.2)
project(asn2)

find_package(Boost COMPONENTS mpi REQUIRED)
find_package(MPI REQUIRED)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(asn2 ${SOURCE_FILES})
target_link_libraries(asn2 ${MPI_LIBRARIES} ${Boost_LIBRARIES})
