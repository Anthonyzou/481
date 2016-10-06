project(asn2)

SET(CMAKE_CXX_FLAGS_DEBUG "... -lpthread")
SET(CMAKE_CXX_FLAGS_RELEASE "... -lpthread")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(SOURCE_FILES main.cpp)
add_executable(asn1 ${SOURCE_FILES})
