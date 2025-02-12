cmake_minimum_required(VERSION 3.5.0)
project(transform VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(main main.cpp src/BMPToYUV.cpp)


include(CTest)
enable_testing()

