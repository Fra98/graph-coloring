#ifndef SRC_SOLVER_H
#define SRC_SOLVER_H

#include "../Graph/Graph.h"

#define MAX_THREADS -1

class Solver {
protected:
    size_t _numThreads;

public:
    explicit Solver(int numThreads) {
        int availableThreads = int(std::thread::hardware_concurrency());

        if(numThreads > availableThreads)
            throw std::runtime_error("Hardware concurrency exceeded: you can use at most " +
                                     std::to_string(availableThreads) + " threads");

        if(numThreads == MAX_THREADS)
            _numThreads = availableThreads;
        else if(numThreads >= 1)
            _numThreads = numThreads;
        else
            throw std::runtime_error("Invalid number of threads");
    };

    virtual ~Solver() = default;

    [[nodiscard]] virtual std::string name() const = 0;

    virtual void solve(Graph &) = 0;

    [[nodiscard]] size_t getNumThreads() const {
        return _numThreads;
    }
};

#endif //SRC_SOLVER_H
