//
// Created by fra on 10/11/21.
//

#include "Graph.h"

static std::mutex m;
static std::condition_variable cv;

// Asynchronous parallel coloring heuristic
void Graph::asyncHeuristic(const std::vector<int> &weights, unsigned int idThread, size_t num_threads,
                           size_t &activeThreads, size_t &running) {
    int toBeColored = 0;
    for(auto v=idThread; v<V; v+=num_threads)
        toBeColored++;

    std::stringstream msg;
    msg << "Thread Id: " << idThread << "\t NumSep: " << toBeColored << "\n";
    std::cout << msg.str();

    int loop = 0;
    while(toBeColored > 0) {
        for(auto v=idThread; v<V; v+=num_threads) {
            if(_vertices[v].getColor() == UNCOLORED) {
                auto adjL = _vertices[v].getAdjL();
                bool isLocalMax = true;
                for(auto w : *adjL)
                    if(weights[w] > weights[v] && _vertices[w].getColor() == UNCOLORED) {
                        isLocalMax = false;
                        break;
                    }

                if(isLocalMax) {
                    colorVertexMinimum(_vertices[v]);
                    toBeColored--;
                }
            }
        }

        std::unique_lock ul(m);
        running--;
//        std::cout << "Thread Id: " << idThread << "\t Loop: " << ++loop << "\t toBeColored: " << toBeColored
//                    << "\t Running: " << running << '\n';
        if(toBeColored == 0)                // no vertex remaining
            activeThreads--;                // decrement number of waiting threads for next loop
        if(running == 0) {
            running = activeThreads;
            cv.notify_all();
        }
        else if(toBeColored > 0)        // enter in waiting only if there are uncolored vertices left
            cv.wait(ul, [&running, &activeThreads](){ return running == activeThreads; });
    }
}

void Graph::coloringJonesPlassmann(size_t num_threads) {
    std::vector<int> weights = randomPermutation(V);
    std::vector<std::thread> threads;
    size_t activeThreads, running;

    if(num_threads == 0)
        num_threads = std::thread::hardware_concurrency();
    activeThreads = num_threads;
    running = num_threads;

    threads.reserve(num_threads);
    for(auto t=0; t<num_threads; t++)
        threads.emplace_back(std::thread(&Graph::asyncHeuristic, this, std::cref(weights), t, num_threads,
                                         std::ref(activeThreads), std::ref(running)));

    for (auto& t : threads)
        t.join();
}
