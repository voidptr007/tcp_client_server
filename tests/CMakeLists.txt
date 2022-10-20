# Set the minimum required version of CMake for this project.
cmake_minimum_required(VERSION 3.8)

# Set the C++ standard to use to build this target.
set(CMAKE_CXX_STANDARD 20) 

#generate debug symbols
set(CMAKE_BUILD_TYPE Debug)

#include headers from include folder
include_directories(${PROJECT_SOURCE_DIR}/include)

#define global variable for all cpp files
file(GLOB TCPSERVER_SOURCES
            "${PROJECT_SOURCE_DIR}/src/*.cpp"
            "${PROJECT_SOURCE_DIR}/tests/test_tcpserver.cpp")

file(GLOB TCPCLIENT_SOURCES
            "${PROJECT_SOURCE_DIR}/src/*.cpp"
            "${PROJECT_SOURCE_DIR}/tests/test_tcpclient.cpp")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

#compile and build exe for source files
add_executable(tcpserver ${TCPSERVER_SOURCES})
add_executable(tcpclient ${TCPCLIENT_SOURCES})

#link libraries
target_link_libraries(tcpserver pthread)
target_link_libraries(tcpclient pthread)

