##---------------------------------------------------------------------------
## Author:      Andreas Ziegler
##---------------------------------------------------------------------------
 
# define minimum cmake version
cmake_minimum_required(VERSION 2.8)

# Activate C++11
add_definitions(-std=c++11)
 
# Our project is called 'Stretcher' this is how it will be called in
# visual studio, and in our makefiles. 
project(Stretcher)

set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_INSTALL_RPATH "${CMAKE_SOURCE_DIR}/lib")
 
# Directory where the install version should be placed
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
# Location where cmake first looks for cmake modules.
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/cmake/Modules")

# Add the subdirectories to the CMake configuration
include_directories("./src")
add_subdirectory(src)
