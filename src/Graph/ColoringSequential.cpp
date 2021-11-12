//
// Created by fra on 25/10/21.
//

#include "Graph.h"

using namespace boost;

void Graph::coloringSeqGreedy() {
    std::vector<int> idx = randomPermutation(V);

    for(int i=0; i<V; i++)
        colorVertexMinimum(_vertices[idx[i]]);
}

std::list<int> Graph::findMIS_seq(const dynamic_bitset<> &vMap) {
    dynamic_bitset MIS { vMap.size() };
    std::list<int> listMIS;

    for(int i=0; i<vMap.size(); i++) {
        if(vMap[i]) {
            auto adjL = _vertices[i].getAdjL();
            bool add = true;

            for(int j : *adjL)
                if(vMap[j] && MIS[j]) {
                    add = false;
                    break;
                }

            if(add) {
                MIS[i] = true;
                listMIS.push_back(i);
            }
        }
    }

    return listMIS;
}

std::list<int> Graph::findMIS_Luby(const dynamic_bitset<> &vMap) {
    dynamic_bitset MIS { vMap.size() };
    std::list<int> listMIS;
    dynamic_bitset vMapTmp = vMap;

    while(vMapTmp.any()) {
        // Randomly choose to add v to X with probability 1/(2*deg)
        dynamic_bitset X { vMap.size() };
        for(int i=0; i<V; i++) {
            if(vMapTmp[i]) {
                auto deg = (float) _vertices[i].getDegree(vMapTmp);
                if(deg == 0)
                    X[i] = true;
                else X[i] = randomWithProb(1/(2*deg));
            }
        }

        // For every arch in X, remove the vertex with the lowest degree
        dynamic_bitset I = X;
        for(int i=0; i<V; i++) {
            if(X[i]) {
                auto adjL = *_vertices[i].getAdjL();
                for(auto j : adjL) {
                    if(X[j]) {
                        auto deg_v = _vertices[i].getDegree(vMapTmp);
                        auto deg_w = _vertices[j].getDegree(vMapTmp);
                        deg_v <= deg_w ? I[i] = false : I[j] = false;
                    }
                }
            }
        }

        // Update MIS and temporary vMap
        MIS |= I;
        dynamic_bitset N_I { vMap.size() };
        for(int i=0; i<V; i++)
            if(I[i]) {
                listMIS.emplace_back(i);
                auto adjL = *_vertices[i].getAdjL();
                for(auto j : adjL)
                    if(vMap[j])
                        N_I[j] = true;
            }
        dynamic_bitset Y = I | N_I;
        vMapTmp -= Y;
    }

    return listMIS;
}

std::list<int> Graph::findMIS_Luby2(const dynamic_bitset<> &vMap) {
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

void Graph::coloringSeqLuby() {
    dynamic_bitset<> vMap(V);
    vMap.set(); // all 1's
    unsigned int U = V;
    int tmpCol = 0;

    while(U > 0) {
        std::cout << "Calculating MIS n: " << tmpCol+1 << std::endl;
        std::list<int> x = findMIS_Luby2(vMap);
        std::cout << "MIS size: " << x.size() << std::endl;
        for(auto idx : x) {
            Vertex &v = _vertices[idx];
            colorVertexMinimum(v);
            vMap[idx] = false;
        }
        tmpCol++;
        U -= x.size();
    }
}