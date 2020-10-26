cmake_minimum_required(VERSION 3.17)
project(HashMap)

set(CMAKE_CXX_STANDARD 20)

include(ExternalProject)
include(${CMAKE_CURRENT_SOURCE_DIR}/externalGoogleTest.cmake)

add_subdirectory(src)
add_subdirectory(Test)