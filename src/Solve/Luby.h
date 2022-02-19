//
// Created by fra on 31/12/21.
//

#ifndef SRC_LUBY_H
#define SRC_LUBY_H

#include <mutex>
#include <atomic>

#include "Solver.h"
#include "../Graph/Graph.h"
#include "../Utils/Range.h"
#include "../Utils/ParallelForEach.hpp"

class Luby : public Solver {

public:
    explicit Luby(int numThreads = MAX_THREADS);

    [[nodiscard]] std::string name() const override;

    void solve(Graph &G) override;
};

#endif //SRC_LUBY_H
