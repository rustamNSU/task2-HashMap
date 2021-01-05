cmake_minimum_required(VERSION 3.17)
project(HashMap)

if(NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Release)
endif()
if(NOT CMAKE_CONFIGURATION_TYPES)
    set (CMAKE_CONFIGURATION_TYPES Release)
endif()


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include(ExternalProject)
include(${CMAKE_CURRENT_SOURCE_DIR}/externalGoogleTest.cmake)

# add_subdirectory(src)
add_subdirectory(Test)