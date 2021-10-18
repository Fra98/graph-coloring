//
// Created by fra on 18/10/21.
//

#include "Vertex.h"

Vertex::Vertex(int self, int color, int weight) : _self(self), _color(color), _weight(weight) {}

void Vertex::addEdge(int v2) {
    _adjL.push_back(v2);
}

