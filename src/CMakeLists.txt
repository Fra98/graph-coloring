cmake_minimum_required(VERSION 3.20)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(seq_greedy main_seq_greedy.cpp Graph/Graph.cpp Graph/Graph.h Vertex/Vertex.cpp Vertex/Vertex.h Colors/Colors.cpp Colors/Colors.h Utils/Utils.cpp Utils/Utils.h Graph/Coloring.cpp)
add_executable(seq_luby main_seq_luby.cpp Graph/Graph.cpp Graph/Graph.h Vertex/Vertex.cpp Vertex/Vertex.h Colors/Colors.cpp Colors/Colors.h Utils/Utils.cpp Utils/Utils.h Graph/Coloring.cpp)
