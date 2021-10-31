//
// Created by fra on 25/10/21.
//

#include "Graph.h"

using namespace boost;

std::list<int> Graph::findMIS_LubyParallel(const std::vector<char> &vMap) {
    std::list<int> listMIS;
    std::vector<char> vMapTmp = vMap;
    std::mutex _lock;

    while(true) {
//        std::cout << vMapTmp.count() << std::endl;
        std::atomic_int n = 0, max_deg = 0, max_v = -1;

        std::for_each(
                std::execution::par,
                _vertices.begin(),
                _vertices.end(),
                [this, &listMIS, &vMapTmp, &n, &max_deg, &max_v, &_lock] (Vertex &v) {
                    auto i = v.getSelf();
                    if(vMapTmp[i]) {
                        auto deg = _vertices[i].getDegree(vMapTmp);
                        if(deg == 0) {
                            std::unique_lock lg(_lock);
                            listMIS.emplace_back(i);
                            lg.unlock();
                            vMapTmp[i] = false;
                        }
                        else {
                            n++;
                            if(deg > max_deg) {
                                max_deg = static_cast<int>(deg);
                                max_v = i;
                            }
                        }
                    }
                }
        );

        // Check if there are remaining nodes
        if(n == 0)
            break;

        if(max_deg >= n/16) {
            listMIS.emplace_back(max_v);
            vMapTmp[max_v] = false;
            auto adjL = _vertices[max_v].getAdjL();
            for(auto j : *adjL)
                vMapTmp[j] = false;
        }
        else {
            unsigned int q = findPrimeWithinRange(n, 2*n);
            unsigned int x = randomWithinRange(0, (int) q-1);
            unsigned int y = randomWithinRange(0, (int) q-1);

            std::vector<char> X(V, false);

            std::for_each(
                    std::execution::par_unseq,
                    _vertices.begin(),
                    _vertices.end(),
                    [this, &vMapTmp, &X, &n, q, x, y] (Vertex &v) {
                        auto i = v.getSelf();
                        if (vMapTmp[i]) {
                            auto deg_v = _vertices[i].getDegree(vMapTmp);
                            if(deg_v < n/16) {
                                auto n_v =  q / (2 * deg_v);
                                auto l_v = (x + i*y) % q;
                                if (l_v <= n_v)
                                    X[i] = true;
                            }
                        }
                    }
            );

            std::vector<char> I = X;
            std::for_each(
                    std::execution::par,
                    _vertices.begin(),
                    _vertices.end(),
                    [this, &vMapTmp, &X, &I, &_lock] (Vertex &v) {
                        auto i = v.getSelf();
                        if(X[i]) {
                            auto adjL = _vertices[i].getAdjL();
                            for(auto j : *adjL)
                                if(X[j]) {
                                    auto deg_v = _vertices[i].getDegree(vMapTmp);
                                    auto deg_w = _vertices[j].getDegree(vMapTmp);
                                    if(deg_v <= deg_w)
                                        I[i] = false;
                                    else {
                                        std::lock_guard lg{_lock};
                                        I[j] = false;
                                    }
                                }
                        }
                    }
            );


            // Update MIS and temporary vMap
            for(int i=0; i<V; i++)
                if(I[i]) {
                    listMIS.emplace_back(i);
                    vMapTmp[i] = false;
                    auto adjL = _vertices[i].getAdjL();
                    for(auto j : *adjL)
                        if(vMapTmp[j])
                            vMapTmp[j] = false;
                }
        }
    }

    return listMIS;
}

void Graph::coloringParLuby() {
    std::vector<char> vMap(V, true);
    unsigned int U = V;
    int numMis = 0;

    while(U > 0) {
        std::cout << "Calculating MIS n: " << ++numMis << std::endl;
        std::list<int> x = findMIS_LubyParallel(vMap);
        std::cout << "MIS size: " << x.size() << std::endl;

        std::for_each(
                std::execution::par_unseq,
                x.begin(),
                x.end(),
                [this, &vMap] (int idx) {
                    Vertex &v = _vertices[idx];
                    colorVertexMinimum(v);
                    vMap[idx] = false;
                }
        );

        U -= x.size();
    }

}