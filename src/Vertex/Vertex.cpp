#include "Vertex.h"

Vertex::Vertex(int self, int color) : _self(self), _color(color) {}

void Vertex::addEdge(int v2) {
    _adjV.push_back(v2);
}

const std::vector<int> &Vertex::getAdjV() const {
    return _adjV;
}

int Vertex::getColor() const {
    return _color;
}

int Vertex::getSelf() const {
    return _self;
}

void Vertex::setColor(int color) {
    _color = color;
}

unsigned int Vertex::getDegree() const {
    return _adjV.size();
}

unsigned int Vertex::getDegree(const boost::dynamic_bitset<> &vMap) const {
    unsigned int deg = 0;
    for(auto w : _adjV)
        if(vMap[w])
            deg++;

    return deg;
}

unsigned int Vertex::getDegree(const std::vector<char> &vMap) const {
    unsigned int deg = 0;
    for(auto w : _adjV)
        if(vMap[w])
            deg++;

    return deg;
}

