cmake_minimum_required(VERSION 3.20)
project(src)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(main
        main.cpp
        Graph/Graph.cpp Graph/Graph.h
        Vertex/Vertex.cpp Vertex/Vertex.h
        Colors/Colors.cpp Colors/Colors.h
        Utils/Utils.cpp Utils/Utils.h
        Utils/DurationLogger.cpp Utils/DurationLogger.h
        Solve/Solver.h
        Solve/Greedy.cpp Solve/Greedy.h
        Solve/Luby.cpp Solve/Luby.h
        Solve/JonesPlassmann.cpp Solve/JonesPlassmann.h
        Solve/LDF.cpp Solve/LDF.h)

add_executable(seq_luby
        main_luby_seq.cpp
        Graph/Graph.cpp Graph/Graph.h
        Vertex/Vertex.cpp Vertex/Vertex.h
        Colors/Colors.cpp Colors/Colors.h
        Utils/Utils.cpp Utils/Utils.h
        Utils/DurationLogger.h Utils/DurationLogger.cpp
        Graph/ColoringSeqLuby.cpp)

# ONLY FOR GCC COMPILER
find_package(TBB)
target_link_libraries(seq_luby TBB::tbb)
target_link_libraries(main TBB::tbb)