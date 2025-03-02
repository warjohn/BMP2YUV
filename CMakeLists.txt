cmake_minimum_required(VERSION 3.5.0)
project(transform VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /W4")
    include_directories(${CMAKE_SOURCE_DIR}/src)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Winvalid-pch")
    include_directories(${CMAKE_SOURCE_DIR}/src)
else()
    message(WARNING "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

add_executable(main main.cpp 
                src/Convertor/BMPToYUV.cpp 
                src/ImageReader/BMPreader.cpp 
                src/ImageSave/SaveYUV.cpp 
                src/VideoProcess/VideoYUV.cpp 
                src/cstmIncludes.cpp)


include(CTest)
enable_testing()

