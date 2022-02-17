//
// Created by fra on 29/12/21.
//

#include "JonesPlassmann.h"

JonesPlassmann::JonesPlassmann(size_t numThreads) {
    auto availableThreads = std::thread::hardware_concurrency();
    if(numThreads > availableThreads)
        throw std::runtime_error("Hardware concurrency exceeded: you can use at most " +
                                 std::to_string(availableThreads) + " threads");
    if(numThreads == 0)
        _numThreads = std::thread::hardware_concurrency();
    else
        _numThreads = numThreads;
}

std::string JonesPlassmann::name() const{
    return "Jones Plassmann";
}

void JonesPlassmann::solve(Graph & G) {
    std::vector<int> weights = randomPermutation(G.getV());
    std::vector<std::thread> threads;
    size_t activeThreads = _numThreads, startCount = 0, endCount = 0;

    threads.reserve(_numThreads);
    for(auto t=0; t<_numThreads; t++)
        threads.emplace_back(std::thread(&JonesPlassmann::asyncHeuristic, this, std::ref(G),
                                         std::cref(weights), t, std::ref(activeThreads),
                                         std::ref(startCount), std::ref(endCount)));

    for(auto& t : threads)
        t.join();
}

// Asynchronous parallel coloring heuristic
void JonesPlassmann::asyncHeuristic(Graph &G, const std::vector<int> &weights, unsigned int idThread,
                                    size_t &activeThreads, size_t &startCount, size_t &endCount) {
    auto V = G.getV();
    auto& vertices = G.getVertices();
    Range rs {V, _numThreads};
    auto start = rs.getStart(idThread);
    auto end = rs.getEnd(idThread);
    auto toBeColored = end - start;

//    std::stringstream msg;
//    msg << "Thread Id: " << idThread << "\t toBeColored: " << toBeColored << "\n";
//    std::cout << msg.str();

//    int loop = 0;
    while(toBeColored > 0) {
//        loop++;
        std::unique_lock ul_start(_m_start);
        startCount++;
        if(startCount == activeThreads)
            _cv_start.notify_all();
        else
            _cv_start.wait(ul_start);

        startCount--;
//        std::cout << "Thread Id: " << idThread << "\t ENTERED -> LOOP: " << loop << "\t startCount: " << startCount << '\n';
        ul_start.unlock();

        // NON-CRITICAL SECTION
        for(auto v=start; v<end; v++) {
            Vertex& vert_v = vertices[v];
            if(vert_v.getColor() == UNCOLORED) {
                auto adjL = vert_v.getAdjL();
                bool isLocalMax = true;
                for(auto w : *adjL)
                    if(vertices[w].getColor() == UNCOLORED && weights[w] > weights[v]) {
                        isLocalMax = false;
                        break;
                    }

                if(isLocalMax) {
                    G.colorVertexMinimum(vert_v);
                    toBeColored--;
                }
            }
        }
        // END NON-CRITICAL SECTION

        std::unique_lock ul_end(_m_end);
        endCount++;
        if(endCount == activeThreads)
            _cv_end.notify_all();
        else if(toBeColored > 0)          // enter in waiting only if there are uncolored vertices left
            _cv_end.wait(ul_end);

        if(toBeColored == 0)
            activeThreads--;
        endCount--;

//        std::cout << "Thread Id: " << idThread << "\t EXITED  -> LOOP: " << loop
//                << "\t endCount: " << endCount << "\t toBeColored: " << toBeColored << '\n';
    }
}


//Asynchronous parallel coloring heuristic
void JonesPlassmann::asyncHeuristicUselessOpt(Graph &G, const std::vector<int> &weights, unsigned int idThread,
                                       size_t &activeThreads, size_t &startCount, size_t &endCount) {
    auto V = G.getV();
    auto& vertices = G.getVertices();
    std::vector<char> vSep, vLoc;
    int numLoc = 0, numSep = 0;
    unsigned int size = V / _numThreads;
    unsigned int start = idThread * size, end = idThread * size + size;
    if(idThread+1 == _numThreads)   // last thread take remaining vertices
        end = V;

    // Determine Separator and Local Vertices
    vSep.reserve(V);
    vLoc.reserve(V);
    for (auto v=start; v<end; v++) {
        auto adjL = vertices[v].getAdjL();
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
//    int loop = 0;
    while(numSep > 0) {
//        loop++;
        std::unique_lock ul_start(_m_start);
        startCount++;
        if (startCount == activeThreads)
            _cv_start.notify_all();
        else
            _cv_start.wait(ul_start);
        startCount--;

//        std::cout << "Thread Id: " << idThread << "\t ENTERED -> LOOP: " << loop << "\t startCount: " << startCount << '\n';
        ul_start.unlock();

        // NON-CRITICAL SECTION
        for (auto v = start; v < end && numSep > 0; v++) {
            Vertex& vert_v = vertices[v];
            if (vSep[v] && vert_v.getColor() == UNCOLORED) {
                auto adjL = vert_v.getAdjL();
                bool isLocalMax = true;
                for (auto w: *adjL)
                    if(vertices[w].getColor() == UNCOLORED && weights[w] > weights[v]) {
                        isLocalMax = false;
                        break;
                    }
                if (isLocalMax) {
                    G.colorVertexMinimum(vert_v);
                    numSep--;
                }
            }
        }
        // END NON-CRITICAL SECTION

        std::unique_lock ul_end(_m_end);
        endCount++;
        if (endCount == activeThreads)
            _cv_end.notify_all();
        else if (numSep > 0)          // enter in waiting only if there are uncolored vertices left
            _cv_end.wait(ul_end);

        if (numSep == 0)
            activeThreads--;
        endCount--;
    }

    // Color local vertices
    for (auto v = start; v < end && numLoc > 0; v++)
        if (vLoc[v]) {
            G.colorVertexMinimum(vertices[v]);
            numLoc--;
        }
}

