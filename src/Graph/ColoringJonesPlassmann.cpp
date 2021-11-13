//
// Created by fra on 10/11/21.
//

#include "Graph.h"

static std::mutex m_start;
static std::mutex m_end;
static std::condition_variable cv_start;
static std::condition_variable cv_end;

// Asynchronous parallel coloring heuristic
void Graph::asyncHeuristic(const std::vector<int> &weights, unsigned int idThread,
                           size_t num_threads, size_t &activeThreads,
                           size_t &startCount, size_t &endCount) {
    unsigned int toBeColored = V / num_threads + (V % num_threads > idThread);

//    std::stringstream msg;
//    msg << "Thread Id: " << idThread << "\t toBeColored: " << toBeColored << "\n";
//    std::cout << msg.str();

//    int loop = 0;
    while(toBeColored > 0) {
//        loop++;
        std::unique_lock ul_start(m_start);
        startCount++;
        if(startCount == activeThreads)
            cv_start.notify_all();
        else
            cv_start.wait(ul_start);

        startCount--;
//        std::cout << "Thread Id: " << idThread << "\t ENTERED -> LOOP: " << loop << "\t startCount: " << startCount << '\n';
        ul_start.unlock();

        // NON-CRITICAL SECTION
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
        // END NON-CRITICAL SECTION

        std::unique_lock ul_end(m_end);
        endCount++;
        if(endCount == activeThreads) {
            cv_end.notify_all();
        }
        else if(toBeColored > 0)          // enter in waiting only if there are uncolored vertices left
            cv_end.wait(ul_end);

        if(toBeColored == 0)
            activeThreads--;
        endCount--;

//        std::cout << "Thread Id: " << idThread << "\t EXITED  -> LOOP: " << loop
//                << "\t endCount: " << endCount << "\t toBeColored: " << toBeColored << '\n';
    }
}

void Graph::coloringJonesPlassmann(size_t num_threads) {
    std::vector<int> weights = randomPermutation(V);
    std::vector<std::thread> threads;
    size_t activeThreads, startCount, endCount;

    if(num_threads == 0)
        num_threads = std::thread::hardware_concurrency();
//    num_threads = 8;
    activeThreads = num_threads;
    startCount = 0;
    endCount = 0;

    threads.reserve(num_threads);
    for(auto t=0; t<num_threads; t++)
        threads.emplace_back(std::thread(&Graph::asyncHeuristic, this, std::cref(weights), t,
                                         num_threads, std::ref(activeThreads),
                                         std::ref(startCount), std::ref(endCount)));

    for (auto& t : threads)
        t.join();
}

/*
static unsigned int getVertexThread(unsigned int vertex, size_t num_threads) {
    return vertex % num_threads;
}

//Asynchronous parallel coloring heuristic
void Graph::asyncHeuristicOpt(const std::vector<int> &weights, unsigned int idThread, size_t num_threads,
                              size_t &activeThreads, size_t &running) {
    int numLoc = 0, numSep = 0;
    std::vector<char> vSep, vLoc;

    // Upper bounds
//    vSep.reserve(V / num_threads + (V % num_threads != 0));
//    vLoc.reserve(V / num_threads + (V % num_threads != 0));
    vSep.reserve(V);
    vLoc.reserve(V);

    // Determine Separator and Local Vertices
    for(auto v=idThread; v<V; v+=num_threads) {
        auto adjL = _vertices[v].getAdjL();
        bool local = true;

        for(auto w : *adjL)
            if(getVertexThread(w, num_threads) != idThread) {
                local = false;
                break;
            }

        if(local) {
            vLoc[v] = true;
            numLoc++;
        }
        else {
            vSep[v] = true;
            numSep++;
        }
    }

    std::stringstream msg;
    msg << "Thread Id: " << idThread << "\t NumSep: " << numSep << "\n";
    std::cout << msg.str();

    int loop = 0;
    while(numSep > 0) {
        for(auto v=idThread; v<V; v+=num_threads) {
            if(vSep[v] && _vertices[v].getColor() == UNCOLORED) {
                auto adjL = _vertices[v].getAdjL();
                bool isLocalMax = true;
                for(auto w : *adjL)
                    if(weights[w] > weights[v] && _vertices[w].getColor() == UNCOLORED) {
                        isLocalMax = false;
                        break;
                    }

                if(isLocalMax) {
                    colorVertexMinimum(_vertices[v]);
                    numSep--;
                }
            }
        }

        if(numSep < 0)
            exit(5);

        std::unique_lock ul(m);
        running--;
        std::cout << "Thread Id: " << idThread << "\t Loop: " << ++loop << "\t numSep: " << numSep << "\t" << running << '\n';

        if(numSep == 0) {               // no separator vertex remaining
            activeThreads--;            // decrement number of waiting threads for next loop
            std::cout << "Thread Id: " << idThread << "\t Finished" << '\n';
        }

        if(running == 0) {
            running = activeThreads;
            cv.notify_all();
        }
        else if(numSep > 0)
            cv.wait(ul, [&running, &activeThreads](){ return running == activeThreads; });

    }

    // Color local vertices
    for(auto v=idThread; v<V && numLoc>0; v+=num_threads) {
        if(vLoc[v]) {
            colorVertexMinimum(_vertices[v]);
            numLoc--;
        }
    }
}
 */