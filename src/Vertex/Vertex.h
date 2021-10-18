//
// Created by fra on 18/10/21.
//

#ifndef SRC_VERTEX_H
#define SRC_VERTEX_H

#include <list>

#define UNCOLORED -1
#define UNWEIGHTED -1

class Vertex {
private:
    std::list<int> _adjL;
    int _self;
    int _color;
    int _weight;

public:
    explicit Vertex(int self, int color = UNCOLORED, int weight = UNWEIGHTED);

    void addEdge(int v2);
};


#endif //SRC_VERTEX_H
