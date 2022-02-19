//
// Created by fra on 31/12/21.
//

#include "LDF.h"

LDF::LDF(size_t numThreads) {
    auto availableThreads = std::thread::hardware_concurrency();
    if(numThreads > availableThreads)
        throw std::runtime_error("Hardware concurrency exceeded: you can use at most " +
                                 std::to_string(availableThreads) + " threads");
    if(numThreads == 0)
        _numThreads = std::thread::hardware_concurrency();
    else
        _numThreads = numThreads;
}

std::string LDF::name() const{
    return "LDF";
}

void LDF::solve(Graph & G) {
    std::vector<int> weights = randomPermutation(G.getV());
    std::vector<std::thread> threads;
    size_t activeThreads = _numThreads, startCount = 0, endCount = 0;

    threads.reserve(_numThreads);
    for(auto t=0; t<_numThreads; t++)
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
                auto& adjL = vert_v.getAdjL();
                auto v_degree = vert_v.getDegree();
                bool isLocalMax = true;
                for(auto w : adjL) {
                    auto w_degree = vertices[w].getDegree();
                    if(vertices[w].getColor() == UNCOLORED)
                        if((w_degree > v_degree || (w_degree == v_degree && weights[w] > weights[v]))) {
                            isLocalMax = false;
                            break;
                        }
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