#ifndef SRC_VERTEX_H
#define SRC_VERTEX_H

#include <list>
#include <iostream>
#include <memory>
#include <boost/dynamic_bitset.hpp>

#include "../Colors/Colors.h"

#define UNCOLORED -1

class Vertex {
    std::vector<int> _adjL;
    int _self;
    int _color;

public:
    explicit Vertex(int self, int color = UNCOLORED);

    void addEdge(int v2);

    [[nodiscard]] const std::vector<int> &getAdjL() const;

    [[nodiscard]] int getColor() const;

    [[nodiscard]] int getSelf() const;

    [[nodiscard]] unsigned int getDegree() const;

    [[nodiscard]] unsigned int getDegree(const boost::dynamic_bitset<> &vMap) const;

    [[nodiscard]] unsigned int getDegree(const std::vector<char> &vMap) const;

    void setColor(int color);
};


#endif //SRC_VERTEX_H
