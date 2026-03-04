cmake_minimum_required(VERSION 3.10)
project(lab2)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(home_work src/home_work.c)

find_package(OpenMP)
if(OpenMP_C_FOUND)
    target_link_libraries(home_work PRIVATE OpenMP::OpenMP_C)
endif()