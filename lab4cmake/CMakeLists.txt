cmake_minimum_required(VERSION 3.20)
project(lab4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SERIAL_SOURCES "src/serial/*.cpp")
file(GLOB PARALLEL_SOURCES "src/parallel/*.cpp")
file(GLOB COMMON_SOURCES "src/*.cpp")

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

# find_package(MPI REQUIRED)
# include_directories(SYSTEM ${MPI_INCLUDE_PATH})
# target_link_libraries(VectorMultiply ${MPI_C_LIBRARIES})

add_executable(VectorMultiplySerial ${SERIAL_SOURCES} ${COMMON_SOURCES})
target_link_directories(VectorMultiplySerial PUBLIC "/src")

find_package(MPI REQUIRED)
find_package(Boost REQUIRED COMPONENTS mpi serialization)

add_executable(VectorMultiply ${PARALLEL_SOURCES} ${COMMON_SOURCES})
target_link_directories(VectorMultiply PUBLIC "/src")
target_link_libraries(VectorMultiply
  MPI::MPI_CXX
  Boost::mpi
  Boost::serialization
)