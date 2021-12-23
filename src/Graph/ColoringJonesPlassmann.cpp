//
// Created by fra on 10/11/21.
//

#include "Graph.h"

static std::mutex m_start;
static std::mutex m_end;
static std::condition_variable cv_start;
static std::condition_variable cv_end;

void Graph::coloringJonesPlassmann(size_t num_threads) {
    std::vector<int> weights = randomPermutation(V);
    std::vector<std::thread> threads;
    size_t activeThreads, startCount, endCount;

    if(num_threads == 0)
        num_threads = std::thread::hardware_concurrency();
    activeThreads = num_threads;
    startCount = 0;
    endCount = 0;

    threads.reserve(num_threads);
    for(auto t=0; t<num_threads; t++)
        threads.emplace_back(std::thread(&Graph::asyncHeuristic, this, std::cref(weights), t,
                                         num_threads, std::ref(activeThreads),
                                         std::ref(startCount), std::ref(endCount)));

    for(auto& t : threads)
        t.join();
}

// Asynchronous parallel coloring heuristic
void Graph::asyncHeuristic(const std::vector<int> &weights, unsigned int idThread,
                           size_t num_threads, size_t &activeThreads,
                           size_t &startCount, size_t &endCount) {
    unsigned int size = V / num_threads;
    unsigned int start = idThread * size, end = idThread * size + size;
    if(idThread+1 == num_threads)   // last thread take remaining vertices
        end = V;
    unsigned int toBeColored = end - start;

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
        for(auto v=start; v<end; v++) {
            if(_vertices[v].getColor() == UNCOLORED) {
                auto adjL = _vertices[v].getAdjL();
                bool isLocalMax = true;
                for(auto w : *adjL)
                    if(_vertices[w].getColor() == UNCOLORED && weights[w] > weights[v]) {
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
        if(endCount == activeThreads)
            cv_end.notify_all();
        else if(toBeColored > 0)          // enter in waiting only if there are uncolored vertices left
            cv_end.wait(ul_end);

        if(toBeColored == 0)
            activeThreads--;
        endCount--;

//        std::cout << "Thread Id: " << idThread << "\t EXITED  -> LOOP: " << loop
//                << "\t endCount: " << endCount << "\t toBeColored: " << toBeColored << '\n';
    }
}

/*
//Asynchronous parallel coloring heuristic
void Graph::asyncHeuristicOpt(const std::vector<int> &weights, unsigned int idThread,
                              size_t num_threads, size_t &activeThreads,
                              size_t &startCount, size_t &endCount,
                              bool &START, bool &END) {
    std::vector<char> vSep, vLoc;
    int numLoc = 0, numSep = 0;
    unsigned int size = V / num_threads;
    unsigned int start = idThread * size, end = idThread * size + size;
    if(idThread == num_threads-1)
        end = V;

    // Determine Separator and Local Vertices
    vSep.reserve(V);
    vLoc.reserve(V);
    for (auto v=start; v<end; v++) {
        auto adjL = _vertices[v].getAdjL();
        bool local = true;

        for (auto w: *adjL)
            if (w >= start &&  w < end) {
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
    msg << "Thread Id: " << idThread << "\t NumSep: " << numSep <<  "\t NumLoc: " << numLoc << "\n";
    std::cout << msg.str();

    // COLORING Separator vertices
    int loop = 0;
    while(numSep > 0) {
        loop++;
        std::unique_lock ul_start(m_start);
        startCount++;
        if (startCount == activeThreads) {
            START = true;
            cv_start.notify_all();
        } else
            cv_start.wait(ul_start, [&START]() { return START; });

        startCount--;
        if (startCount == 0)
            START = false;
        std::cout << "Thread Id: " << idThread << "\t ENTERED -> LOOP: " << loop << "\t startCount: " << startCount << '\n';
        ul_start.unlock();

        // NON-CRITICAL SECTION
        for (auto v = start; v < end && numSep > 0; v++) {
            if (vSep[v] && _vertices[v].getColor() == UNCOLORED) {
                auto adjL = _vertices[v].getAdjL();
                bool isLocalMax = true;
                for (auto w: *adjL)
                    if (weights[w] > weights[v] && _vertices[w].getColor() == UNCOLORED) {
                        isLocalMax = false;
                        break;
                    }

                if (isLocalMax) {
                    colorVertexMinimum(_vertices[v]);
                    numSep--;
                }
            }
        }
        // END NON-CRITICAL SECTION

        std::unique_lock ul_end(m_end);
        endCount++;
        if (endCount == activeThreads) {
            END = true;
            cv_end.notify_all();
        } else if (numSep > 0)          // enter in waiting only if there are uncolored vertices left
            cv_end.wait(ul_end, [&END]() { return END; });

        if (numSep == 0)
            activeThreads--;
        endCount--;
        if (endCount == 0) {
            END = false;
        }
    }

    // Color local vertices
    for (auto v = start; v < end && numLoc > 0; v++)
        if (vLoc[v]) {
            colorVertexMinimum(_vertices[v]);
            numLoc--;
        }
}
*/