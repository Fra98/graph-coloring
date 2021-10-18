//
// Created by fra on 18/10/21.
//

#ifndef SRC_VERTEX_H
#define SRC_VERTEX_H

#include <list>
#include "../Colors/Colors.h"

#define UNCOLORED -1
#define UNWEIGHTED -1

class Vertex {
    std::list<int> _adjL;
    int _self;
    int _color;
    int _weight;

public:
    explicit Vertex(int self, int color = UNCOLORED, int weight = UNWEIGHTED);

    void addEdge(int v2);

    [[nodiscard]] std::shared_ptr<std::list<int>> getAdjL();

    [[nodiscard]] int getColor() const;

    [[nodiscard]] int getWeight() const;

    [[nodiscard]] int getSelf() const;

    void setColor(int color);

    void setWeight(int weight);
};


#endif //SRC_VERTEX_H
