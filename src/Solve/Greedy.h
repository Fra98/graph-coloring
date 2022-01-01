#ifndef SRC_GREEDY_H
#define SRC_GREEDY_H

#include "Solver.h"
#include "../Graph/Graph.h"

class Greedy : public Solver {

public:
    explicit Greedy() = default;

    [[nodiscard]] std::string name() const override;

    void solve(Graph & G) override;
};


#endif //SRC_GREEDY_H
