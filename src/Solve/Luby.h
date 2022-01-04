//
// Created by fra on 31/12/21.
//

#ifndef SRC_LUBY_H
#define SRC_LUBY_H

#include <execution>

#include "Solver.h"
#include "../Graph/Graph.h"
#include <mutex>

class Luby : public Solver {

public:
    explicit Luby() = default;

    [[nodiscard]] std::string name() const override;

    void solve(Graph &G) override;
};

#endif //SRC_LUBY_H
