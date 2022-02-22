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

std::vector<Vertex> &Graph::getVertices(){
    return _vertices;
}

Vertex &Graph::getVertex(unsigned int pos) {
    return _vertices[pos];
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

        auto& adjV = v.getAdjV();
        for(auto j : adjV) {
            int v2_col = _vertices[j].getColor();
            if (v2_col == v_col || v2_col == UNCOLORED)
                return false;
        }
    }

    return true;
}

void Graph::resetColors() {
    for(int i=0; i<V; i++)
        _vertices[i].setColor(UNCOLORED);
}

void Graph::colorVertexMinimum(Vertex &v) {
    Colors C;
    auto& adjV = v.getAdjV();

    // C = { colors of all colored neighbors of v }
    for(auto j : adjV) {
        int cj = _vertices[j].getColor();
        if(cj != UNCOLORED)
            C.addColor(cj);
    }

    // Smallest color not in C
    int minCol = C.findMinCol();

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
        auto& adjV = _vertices[i].getAdjV();
        for(auto j : adjV)
            if(vMap[i] && vMap[j])
                return false;
    }
    return true;
}

void Graph::setEdgeNumber(unsigned int E) {
    this->E = E;
}
