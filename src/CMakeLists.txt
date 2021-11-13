cmake_minimum_required(VERSION 3.20)
project(src)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(seq_greedy main_greedy_seq.cpp Graph/Graph.cpp Graph/Graph.h Vertex/Vertex.cpp Vertex/Vertex.h Colors/Colors.cpp Colors/Colors.h Utils/Utils.cpp Utils/Utils.h Graph/ColoringSequential.cpp)
add_executable(seq_luby main_luby_seq.cpp Graph/Graph.cpp Graph/Graph.h Vertex/Vertex.cpp Vertex/Vertex.h Colors/Colors.cpp Colors/Colors.h Utils/Utils.cpp Utils/Utils.h Graph/ColoringSequential.cpp)
add_executable(par_luby main_luby_par.cpp Graph/Graph.cpp Graph/Graph.h Vertex/Vertex.cpp Vertex/Vertex.h Colors/Colors.cpp Colors/Colors.h Utils/Utils.cpp Utils/Utils.h Graph/ColoringLuby.cpp)
add_executable(jones_plassmann main_jones_plassmann.cpp Graph/Graph.cpp Graph/Graph.h Vertex/Vertex.cpp Vertex/Vertex.h Colors/Colors.cpp Colors/Colors.h Utils/Utils.cpp Utils/Utils.h Graph/ColoringJonesPlassmann.cpp)

# ONLY FOR GCC COMPILER
find_package(TBB)
target_link_libraries(seq_greedy TBB::tbb)
target_link_libraries(seq_luby TBB::tbb)
target_link_libraries(par_luby TBB::tbb)
target_link_libraries(jones_plassmann TBB::tbb)