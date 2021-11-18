cmake_minimum_required(VERSION 3.20)
project(rule30_21)

#set(CMAKE_CXX_COMPILER /usr/bin/mpic++)
set(CMAKE_CXX_STANDARD 17)
set(CXXFLAGS "-O3")

find_package(MPI)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(diamond diamond/main.cpp diamond/IChunkProcessor.cpp diamond/IChunkProcessor.h diamond/SimpleChunkProcessor.cpp diamond/SimpleChunkProcessor.h diamond/Edge.cpp diamond/Edge.h)

add_executable(flat flat/main.cpp)

add_executable(palych palych/main.cpp)

add_executable(parallelogram parallelogram/main.cpp parallelogram/crunching/process_parallelogram.cpp parallelogram/distribution/Worker.cpp parallelogram/distribution/Worker.h parallelogram/distribution/Master.cpp parallelogram/distribution/Master.h parallelogram/utils.cpp parallelogram/utils.h parallelogram/distribution/WorkChunk.cpp parallelogram/distribution/WorkChunk.h parallelogram/distribution/SharedArray.hpp parallelogram/distribution/SharedValue.hpp)
target_link_libraries(parallelogram ${MPI_CXX_LIBRARIES})

add_executable(parallelogram_parallelogram_benchmark parallelogram/crunching/process_parallelogram.cpp parallelogram/parallelogram_benchmark.cpp)