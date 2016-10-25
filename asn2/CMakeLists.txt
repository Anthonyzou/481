cmake_minimum_required(VERSION 2.8)
project(asn2)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/release")
set(Boost_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/boost/build/lib")
set(Boost_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/boost/build/include")
set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/boost/build")
set(Boost_NO_SYSTEM_PATHS "ON")

find_package(MPI REQUIRED)
find_package(Boost COMPONENTS mpi serialization REQUIRED)
message("BOOST " ${Boost_LIBRARIES})

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g") # -g is in default config
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3") # -O3 is in default config
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp)
add_executable(asn2 ${SOURCE_FILES})
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(asn2 ${MPI_CXX_LIBRARIES} ${Boost_LIBRARIES})
