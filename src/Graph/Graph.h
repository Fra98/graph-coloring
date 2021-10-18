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

#include "../Vertex/Vertex.h"

class Graph {
private:
    std::vector<Vertex> _vertices;
    int V;
    int E;

public:
    Graph(int V, int E);

    void addEdge(int v1, int v2);
};

std::unique_ptr<Graph> loadGraph(const std::string &fileName);

#endif //SRC_GRAPH_H
