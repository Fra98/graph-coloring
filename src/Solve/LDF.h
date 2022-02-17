//
// Created by fra on 31/12/21.
//

#ifndef SRC_LDF_H
#define SRC_LDF_H

#include <thread>
#include <condition_variable>
#include <mutex>

#include "Solver.h"
#include "../Graph/Graph.h"
#include "../Utils/Range.h"

class LDF : public Solver {
    size_t _numThreads;
    std::mutex _m_start;
    std::mutex _m_end;
    std::condition_variable _cv_start;
    std::condition_variable _cv_end;

    void asyncHeuristic(Graph &G, const std::vector<int> &weights, unsigned int idThread,
                        size_t &activeThreads, size_t &startCount, size_t &endCount);

public:
    explicit LDF(size_t numThreads = 0);

    [[nodiscard]] std::string name() const override;

    void solve(Graph & G) override;
};


#endif //SRC_LDF_H
