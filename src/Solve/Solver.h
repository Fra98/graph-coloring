//
// Created by fra on 30/12/21.
//

#ifndef SRC_SOLVER_H
#define SRC_SOLVER_H

#include "../Graph/Graph.h"

class Solver {
public:
    virtual ~Solver() = default;

    [[nodiscard]] virtual std::string name() const = 0;

    virtual void solve(Graph &) = 0;
};

#endif //SRC_SOLVER_H
