cmake_minimum_required(VERSION 3.20)
project(src)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(seq_greedy main_greedy_seq.cpp Graph/ColoringSequential.cpp Graph/Graph.cpp Graph/Graph.h
        Vertex/Vertex.cpp Vertex/Vertex.h Colors/Colors.cpp Colors/Colors.h
        Utils/Utils.cpp Utils/Utils.h Utils/DurationLogger.h Utils/DurationLogger.cpp)
add_executable(seq_luby main_luby_seq.cpp Graph/ColoringSequential.cpp Graph/Graph.cpp Graph/Graph.h
        Vertex/Vertex.cpp Vertex/Vertex.h Colors/Colors.cpp Colors/Colors.h
        Utils/Utils.cpp Utils/Utils.h Utils/DurationLogger.h Utils/DurationLogger.cpp)
add_executable(par_luby main_luby_par.cpp Graph/ColoringLuby.cpp Graph/Graph.cpp Graph/Graph.h
        Vertex/Vertex.cpp Vertex/Vertex.h Colors/Colors.cpp Colors/Colors.h
        Utils/Utils.cpp Utils/Utils.h Utils/DurationLogger.h Utils/DurationLogger.cpp)
add_executable(jones_plassmann main_jones_plassmann.cpp Solve/Solver.h Solve/JonesPlassmann.cpp Solve/JonesPlassmann.h Graph/Graph.cpp Graph/Graph.h
        Vertex/Vertex.cpp Vertex/Vertex.h Colors/Colors.cpp Colors/Colors.h Utils/Utils.cpp
        Utils/Utils.h Utils/DurationLogger.h Utils/DurationLogger.cpp)
add_executable(LDF main_LDF.cpp Graph/ColoringLDF.cpp Graph/Graph.cpp Graph/Graph.h
        Vertex/Vertex.cpp Vertex/Vertex.h Colors/Colors.cpp Colors/Colors.h
        Utils/Utils.cpp Utils/Utils.h Utils/DurationLogger.h Utils/DurationLogger.cpp)

# ONLY FOR GCC COMPILER
find_package(TBB)
target_link_libraries(seq_greedy TBB::tbb)
target_link_libraries(seq_luby TBB::tbb)
target_link_libraries(par_luby TBB::tbb)
target_link_libraries(jones_plassmann TBB::tbb)
target_link_libraries(LDF TBB::tbb)