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
#include <iterator>
#include <algorithm>

// Concurrency
#include <execution>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>

// Data Structures
#include <vector>
#include <unordered_set>
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

    [[nodiscard]] unsigned int getV() const;

    [[nodiscard]] unsigned int getE() const;

    [[nodiscard]] unsigned int getMaxDegree() const;

    void addEdge(int v1, int v2);

    bool isColored();
    void colorVertexMinimum(Vertex &v);
    int numColorsUsed();

    // COLORING ALGORITHMS
    void coloringSeqGreedy();
    void coloringSeqLuby();
    void coloringParLuby();
    void coloringJonesPlassmann(size_t num_threads = 0);
    void asyncHeuristic(const std::vector<int> &weights, unsigned int idThread,
                        size_t num_threads, size_t &activeThreads, size_t &running);

    // MIS ALGORITHMS
    bool isMIS(const boost::dynamic_bitset<> &vMap);
    std::list<int> findMIS_seq(const boost::dynamic_bitset<> &vMap);
    std::list<int> findMIS_Luby(const boost::dynamic_bitset<> &vMap);
    std::list<int> findMIS_Luby2(const boost::dynamic_bitset<> &vMap);
    std::list<int> findMIS_LubyParallel(const std::vector<char> &vMap);
};

std::unique_ptr<Graph> loadGraph(const std::string &fileName);

#endif //SRC_GRAPH_H
