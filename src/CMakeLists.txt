cmake_minimum_required(VERSION 3.20)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(src main.cpp Graph/Graph.cpp Graph/Graph.h Vertex/Vertex.cpp Vertex/Vertex.h Colors/Colors.cpp Colors/Colors.h Utils.cpp Utils.h)
