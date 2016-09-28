project(asn1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -g")

set(SOURCE_FILES main.cpp)
add_executable(asn1 ${SOURCE_FILES})

set(SOURCE_FILES_TEST test.cpp)
add_executable(asn1test ${SOURCE_FILES_TEST})
