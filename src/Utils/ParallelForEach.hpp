#ifndef SRC_PARALLELFOREACH_HPP
#define SRC_PARALLELFOREACH_HPP

#include <functional>
#include <thread>

#include "Range.h"

#define MAX_SEQ 500

template<typename V, typename F>
void parallelForEach(size_t numThreads, std::vector<V> &vector, F&& fn) {
    // Sequential loop if size of the array is reasonably small
    if(vector.size() < MAX_SEQ) {
        for(int i=0; i<vector.size(); i++)
            fn(vector[i]);
        return;
    }

    Range rs (vector.size(), numThreads);
    std::vector<std::thread> threads;
    threads.reserve(numThreads);
    
    for(int i=0; i<numThreads; i++) {
        auto start = rs.getStart(i);
        auto end = rs.getEnd(i);
        if(start==end) // empty range
            continue;
        threads.emplace_back(std::thread([start, end, &vector, &fn]() {
            for(auto j=start; j<end; j++)
                fn(vector[j]);
        }));
    }

    for(auto &t : threads)
        t.join();
}

#endif //SRC_PARALLELFOREACH_HPP
