#include "LDF.h"

LDF::LDF(int numThreads) : Solver(numThreads) {}

std::string LDF::name() const{
    return "LDF";
}

void LDF::solve(Graph & G) {
    std::vector<int> weights = randomPermutation(G.getV());
    std::vector<std::thread> threads;
    size_t activeThreads = _numThreads, startCount = 0, endCount = 0;

    std::vector<char> threadToStart(_numThreads,true);

    Range rs {G.getV(), _numThreads};
    for (int i = 0; i<_numThreads; i++) {
        if (rs.getStart(i) == rs.getEnd(i)) {
            threadToStart[i] = false;
            activeThreads--;
        }
    }

    threads.reserve(activeThreads);
    for(auto t=0; t<_numThreads; t++)
        if (threadToStart[t])
            threads.emplace_back(std::thread(&LDF::asyncHeuristic, this, std::ref(G),
                                             std::cref(weights), t, std::ref(activeThreads),
                                             std::ref(startCount), std::ref(endCount)));

    for(auto& t : threads)
        t.join();
}

// Asynchronous parallel coloring heuristic
void LDF::asyncHeuristic(Graph &G, const std::vector<int> &weights, unsigned int idThread,
                                    size_t &activeThreads, size_t &startCount, size_t &endCount) {
    auto V = G.getV();
    auto& vertices = G.getVertices();
    Range rs {V, _numThreads};
    auto start = rs.getStart(idThread);
    auto end = rs.getEnd(idThread);

    // Determine Separator and Local Vertices
    int numLoc = 0, numSep = 0;
    std::vector<char> vSep(V), vLoc(V);

    for (auto v=start; v<end; v++) {
        auto& adjV = vertices[v].getAdjV();
        bool local = true;
        for(auto w : adjV)
            if(w < start || w >= end) {
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

//    std::stringstream msg;
//    msg << "Thread Id: " << idThread << "\t NumSep: " << numSep <<  "\t NumLoc: " << numLoc << "\n";
//    std::cout << msg.str();

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
                auto& adjV = vert_v.getAdjV();
                auto v_degree = vert_v.getDegree();
                bool isLocalMax = true;
                for(auto w : adjV) {
                    if ((vertices[w].getColor() == UNCOLORED) && (w < start || w >= end)) {
                        auto w_degree = vertices[w].getDegree();
                        if ((w_degree > v_degree || (w_degree == v_degree && weights[w] > weights[v]))) {
                            isLocalMax = false;
                            break;
                        }
                    }
                }
                if(isLocalMax) {
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