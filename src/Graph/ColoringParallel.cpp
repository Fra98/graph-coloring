//
// Created by fra on 25/10/21.
//

#include "Graph.h"

using namespace boost;

std::list<int> Graph::findMIS_LubyParallel(const dynamic_bitset<> &vMap) {
    dynamic_bitset MIS { vMap.size() };
    std::list<int> listMIS;
    dynamic_bitset vMapTmp = vMap;

    while(vMapTmp.any()) {
//        std::cout << vMapTmp.count() << std::endl;
        unsigned int n = 0, max_deg = 0;
        int max_v = -1;

        // Find a vertex with maximum degree (and add immediately to MIS vertex with deg=0)
        for(int i=0; i<V; i++) {
            if(vMapTmp[i]) {
                auto deg = _vertices[i].getDegree(vMapTmp);
                if(deg == 0) {
                    MIS[i] = true;
                    listMIS.emplace_back(i);
                    vMapTmp[i] = false;
                }
                else {
                    n++;
                    if(deg > max_deg) {
                        max_deg = deg;
                        max_v = i;
                    }
                }
            }
        }

        if(max_v == -1)
            continue;

        if(max_deg >= n/16) {
            MIS[max_v] = true;
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

            dynamic_bitset X { vMap.size() };

            for(int i=0; i<V; i++) {
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

            dynamic_bitset I = X;
            for(int i=0; i<V; i++)
                if(X[i]) {
                    auto adjL = _vertices[i].getAdjL();
                    for(auto j : *adjL)
                        if(X[j]) {
                            auto deg_v = _vertices[i].getDegree(vMapTmp);
                            auto deg_w = _vertices[j].getDegree(vMapTmp);
                            deg_v <= deg_w ? I[i] = false : I[j] = false;
                        }
                }

            // Update MIS and temporary vMap
            for(int i=0; i<V; i++)
                if(I[i]) {
                    MIS[i] = true;
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
    dynamic_bitset<> vMap(V);
    vMap.set(); // all 1's
    unsigned int U = V;
    int tmpCol = 0;

    while(U > 0) {
        std::cout << "Calculating MIS n: " << tmpCol+1 << std::endl;
        std::list<int> x = findMIS_LubyParallel(vMap);
        std::cout << "MIS size: " << x.size() << std::endl;

        std::for_each(
                std::execution::par_unseq,
                x.begin(),
                x.end(),
                [this, &vMap, tmpCol] (int idx) {
                    _vertices[idx].setColor(tmpCol);
                    vMap[idx] = false;
                }
            );

        tmpCol++;
        U -= x.size();
    }

}