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
#include <algorithm>
#include <execution>

#include <boost/dynamic_bitset.hpp>

#include "../Vertex/Vertex.h"
#include "../Colors/Colors.h"
#include "../Utils/Utils.h"

class Graph {
    std::vector<Vertex> _vertices;
    unsigned int V;
    unsigned int E;

public:
    Graph(int V, int E);

    void addEdge(int v1, int v2);

    [[nodiscard]] unsigned int getV() const;

    [[nodiscard]] unsigned int getE() const;
    
    // COLORING ALGORITHMS
    bool isColored();
    bool isColoredPar();
    void coloringSeqGreedy();
    void coloringSeqLuby();
    void coloringParLuby();

    // MIS ALGORITHMS
    bool isMIS(const boost::dynamic_bitset<> &vMap);
    std::list<int> findMIS_seq(const boost::dynamic_bitset<> &vMap);
    std::list<int> findMIS_Luby(const boost::dynamic_bitset<> &vMap);
    std::list<int> findMIS_Luby2(const boost::dynamic_bitset<> &vMap);
    std::list<int> findMIS_LubyParallel(const boost::dynamic_bitset<> &vMap);

};

std::unique_ptr<Graph> loadGraph(const std::string &fileName);

#endif //SRC_GRAPH_H
