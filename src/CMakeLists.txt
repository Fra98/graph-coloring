cmake_minimum_required(VERSION 3.20)
project(src)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(main
        main.cpp
        Graph/Graph.cpp Graph/Graph.h Graph/Loaders.cpp
        Vertex/Vertex.cpp Vertex/Vertex.h
        Colors/Colors.cpp Colors/Colors.h
        Utils/Utils.cpp Utils/Utils.h
        Utils/DurationLogger.h
        Utils/Range.h
        Utils/ParallelForEach.hpp
        Utils/Jobs.hpp
        Utils/Benchmark.h
        Solve/Solver.h
        Solve/Greedy.cpp Solve/Greedy.h
        Solve/Luby.cpp Solve/Luby.h
        Solve/JonesPlassmann.cpp Solve/JonesPlassmann.h
        Solve/LDF.cpp Solve/LDF.h)

add_executable(benchmark
        benchmark.cpp
        Graph/Graph.cpp Graph/Graph.h Graph/Loaders.cpp
        Vertex/Vertex.cpp Vertex/Vertex.h
        Colors/Colors.cpp Colors/Colors.h
        Utils/Utils.cpp Utils/Utils.h
        Utils/Benchmark.h
        Utils/DurationLogger.h
        Utils/Range.h
        Utils/ParallelForEach.hpp
        Utils/Jobs.hpp
        Solve/Solver.h
        Solve/Greedy.cpp Solve/Greedy.h
        Solve/Luby.cpp Solve/Luby.h
        Solve/JonesPlassmann.cpp Solve/JonesPlassmann.h
        Solve/LDF.cpp Solve/LDF.h)