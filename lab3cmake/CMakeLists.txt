cmake_minimum_required(VERSION 3.10)
project(DiffEquation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(DiffEquation ${SOURCES})

# Include directories
target_include_directories(DiffEquation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(OpenMP COMPONENTS CXX)
if(OpenMP_CXX_FOUND)
    target_link_libraries(DiffEquation PRIVATE OpenMP::OpenMP_CXX)
endif()