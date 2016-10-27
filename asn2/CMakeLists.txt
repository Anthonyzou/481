cmake_minimum_required(VERSION 2.8)
project(asn2)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/release")

set(Boost_USE_STATIC_LIBS ON)

find_package(MPI)
find_package(Boost COMPONENTS serialization mpi)

if(NOT Boost_FOUND)
  set(Boost_LIBRARY_DIR "/local/scrach/481/asn2/boost/build/lib")
  set(Boost_INCLUDE_DIR "/local/scrach/481/asn2/boost/build/include")
  set(BOOST_ROOT "/local/scrach/481/asn2/boost/build")
  set(Boost_NO_SYSTEM_PATHS ON)
  find_package(Boost COMPONENTS serialization mpi)
endif()

message("BOOST " ${Boost_LIBRARIES})
message("MPI " ${MPI_CXX_LIBRARIES})

set(STATIC_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g") # -g is in default config
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3") # -O3 is in default config
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread ${STATIC_FLAGS}")

file(GLOB_RECURSE MPI "${CMAKE_CURRENT_SOURCE_DIR}/mpi/*.cpp")
set(SOURCE_FILES main.cpp)

add_executable(asn2 ${SOURCE_FILES})
target_link_libraries(asn2 ${MPI_CXX_LIBRARIES} ${Boost_LIBRARIES})
