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

add_executable(parallelogram parallelogram/main.cpp parallelogram/crunching/process_parallelogram.cpp parallelogram/distribution/Worker.cpp parallelogram/distribution/Worker.h parallelogram/distribution/Master.cpp parallelogram/distribution/Master.h parallelogram/utils.cpp parallelogram/utils.h parallelogram/distribution/SharedArray.hpp parallelogram/distribution/SharedValue.hpp parallelogram/distribution/Grid.hpp parallelogram/distribution/IWorkChunk.cpp parallelogram/distribution/IWorkChunk.h)
target_link_libraries(parallelogram ${MPI_CXX_LIBRARIES})

add_executable(parallelogram_parallelogram_benchmark parallelogram/crunching/process_parallelogram.cpp parallelogram/parallelogram_benchmark.cpp)

add_executable(bridge bridge/grid/IGrid.cpp bridge/grid/impl/galqiwi_v1_impl.cpp bridge/grid/impl/galqiwi_v1_impl.h bridge/grid/impl/crunching/process_parallelogram.cpp bridge/main.cpp bridge/grid/grid_facade.cpp bridge/grid/grid_facade.h bridge/solvers/ISolver.h bridge/solvers/multithreaded_solver/multithreaded_solver.cpp bridge/solvers/multithreaded_solver/multithreaded_solver.h bridge/solvers/simple_solver/simple_solver.cpp)
