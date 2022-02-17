//
// Created by fra on 18/10/21.
//

#ifndef SRC_GRAPH_H
#define SRC_GRAPH_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <thread>
#include <filesystem>

// Data Structures
#include <vector>
#include <unordered_set>
#include <boost/dynamic_bitset.hpp>

#include "../Vertex/Vertex.h"
#include "../Colors/Colors.h"
#include "../Utils/Utils.h"

class Graph {
private:
    std::vector<Vertex> _vertices;
    unsigned int V;
    unsigned int E;

public:
    explicit Graph(int V, int E = 0);

    [[nodiscard]] unsigned int getV() const;

    [[nodiscard]] unsigned int getE() const;

    [[nodiscard]] unsigned int getMaxDegree() const;

    std::vector<Vertex> &getVertices();

    Vertex &getVertex(unsigned int pos);

    void addEdge(int v1, int v2);
    void setEdgeNumber(unsigned int E);

    // COLORING FUNCTIONS
    bool isColored();
    void resetColors();
    void colorVertexMinimum(Vertex &v);
    int numColorsUsed();

    // MIS ALGORITHMS
    bool isMIS(const boost::dynamic_bitset<> &vMap);
    std::list<int> findMIS_seq(const boost::dynamic_bitset<> &vMap);
    std::list<int> findMIS_Luby(const boost::dynamic_bitset<> &vMap);
    std::list<int> findMIS_Luby2(const boost::dynamic_bitset<> &vMap);

    // TO BE DELETED
    void coloringSeqLuby();
};

Graph loadGraph(const std::string &fileName);

#endif //SRC_GRAPH_H
