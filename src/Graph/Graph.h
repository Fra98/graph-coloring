//
// Created by fra on 18/10/21.
//

#ifndef SRC_GRAPH_H
#define SRC_GRAPH_H

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <iterator>

#include "../Vertex/Vertex.h"
#include "../Colors/Colors.h"
#include "../Utils.h"

class Graph {
    std::vector<Vertex> _vertices;
    int V;
    int E;

public:
    Graph(int V, int E);

    void addEdge(int v1, int v2);

    void coloringSeqGreedy();
};

std::unique_ptr<Graph> loadGraph(const std::string &fileName);

#endif //SRC_GRAPH_H
