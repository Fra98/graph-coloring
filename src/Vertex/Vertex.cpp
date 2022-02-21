//
// Created by fra on 18/10/21.
//

#include <memory>
#include "Vertex.h"

Vertex::Vertex(int self, int color, int weight) : _self(self), _color(color), _weight(weight) {}

void Vertex::addEdge(int v2) {
    _adjL.push_back(v2);
}

const std::vector<int> &Vertex::getAdjL() const {
    return _adjL;
}

int Vertex::getColor() const {
    return _color;
}

int Vertex::getWeight() const {
    return _weight;
}

int Vertex::getSelf() const {
    return _self;
}

void Vertex::setColor(int color) {
    _color = color;
}

void Vertex::setWeight(int weight) {
    _weight = weight;
}

unsigned int Vertex::getDegree() const {
    return _adjL.size();
}

unsigned int Vertex::getDegree(const boost::dynamic_bitset<> &vMap) const {
    unsigned int deg = 0;
    for(auto w : _adjL)
        if(vMap[w])
            deg++;

    return deg;
}

unsigned int Vertex::getDegree(const std::vector<char> &vMap) const {
    unsigned int deg = 0;
    for(auto w : _adjL)
        if(vMap[w])
            deg++;

    return deg;
}

