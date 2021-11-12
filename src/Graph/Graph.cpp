//
// Created by fra on 18/10/21.
//

#include "Graph.h"

Graph::Graph(int V, int E) : V(V), E(E) {
    _vertices.reserve(V);
    for(int i=0; i<V; i++)
        _vertices.emplace_back(Vertex{i});
}

unsigned int Graph::getV() const {
    return V;
}

unsigned int Graph::getE() const {
    return E;
}

void Graph::addEdge(int v1, int v2) {
    _vertices[v1].addEdge(v2);
}

unsigned int Graph::getMaxDegree() const {
    unsigned int maxDegree = 0;
    for(int i=0; i<V; i++) {
        auto deg = _vertices[i].getDegree();
        if(deg > maxDegree)
            maxDegree = deg;
    }

    return maxDegree;
}

bool Graph::isColored() {
    for(int i=0; i<V; i++) {
        Vertex &v = _vertices[i];

        int v_col = v.getColor();
        if(v_col == UNCOLORED)
            return false;

        auto adjL = v.getAdjL();
        for(int j : *adjL) {
            int v2_col = _vertices[j].getColor();
            if (v2_col == v_col || v2_col == UNCOLORED)
                return false;
        }
    }

    return true;
}

void Graph::colorVertexMinimum(Vertex &v) {
    Colors C;
    auto adjL = v.getAdjL();

    // C = { colors of all colored neighbors of v }
    for(int j : *adjL) {
        int cj = _vertices[j].getColor();
        if(cj != UNCOLORED)
            C.addColor(cj);
    }

    // Smallest color not in C
    int minCol = C.findMinCol();
    if(minCol == COLS_FULL)
        minCol = C.resizeColors();

    v.setColor(minCol);
}

int Graph::numColorsUsed() {
    int num = 0, tmpC;
    std::unordered_set<int> colors_table;

    for(int i=0; i<V; i++) {
        tmpC = _vertices[i].getColor();
        if(!colors_table.count(tmpC) && tmpC != UNCOLORED) {
            colors_table.emplace(tmpC);
            num++;
        }
    }

    return num;
}

bool Graph::isMIS(const boost::dynamic_bitset<> &vMap) {
    for(int i=0; i<V; i++) {
        auto adjL = _vertices[i].getAdjL();
        for(int j : *adjL)
            if(vMap[i] && vMap[j])
                return false;
    }
    return true;
}

std::unique_ptr<Graph> loadGraph(const std::string &fileName) {
    std::ifstream fin {fileName};
    std::string line;
    int numV, numE, v1, v2;

    if (!fin.is_open()) {
        std::cout << "Could not open the file\n";
        exit(-1);
    }

    // Get V and E on first line
    std::getline(fin, line);
    std::stringstream(line) >> numV >> numE;
    Graph G {numV, numE};

    // Read vertex adjacency list line by line
    v1 = 0;
    while(std::getline(fin, line)) {
        std::istringstream iss {line};
        while(iss >> v2)
            G.addEdge(v1, v2-1);
        v1++;
    }

    fin.close();

    return std::make_unique<Graph>(G);
}


