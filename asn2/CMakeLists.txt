cmake_minimum_required(VERSION 2.8)
project(asn2)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/release")

find_package(MPI)
find_package(Boost COMPONENTS serialization mpi)
include_directories( ${MPI_CXX_INCLUDE_PATH} )

message("BOOST " ${Boost_LIBRARIES})
message("MPI " ${MPI_CXX_LIBRARIES})

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g") # -g is in default config
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3") # -O3 is in default config
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES asn2.cpp)
add_executable(asn2 ${SOURCE_FILES})
target_link_libraries(asn2 ${Boost_LIBRARIES} ${MPI_CXX_LIBRARIES})

set(SOURCE_FILES_TEST test.cpp)
add_executable(sequential ${SOURCE_FILES_TEST})