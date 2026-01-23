cmake_minimum_required(VERSION 3.20)
project(RawrXD-CloudHosting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build configuration  
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set output directories for Release/Debug
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin/${OUTPUTCONFIG})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib/${OUTPUTCONFIG})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib/${OUTPUTCONFIG})
endforeach()

# Add the RawrXD-ModelLoader subdirectory
add_subdirectory(RawrXD/RawrXD-ModelLoader)

message(STATUS "RawrXD-CloudHosting configured successfully")
message(STATUS "Build output directory: ${CMAKE_BINARY_DIR}/bin")
