//
// Created by fra on 25/10/21.
//

#include "Graph.h"

using namespace boost;

void Graph::coloringSeqGreedy() {
    std::vector<int> idx = randomPermutation(V);

    for(int i=0; i<V; i++) {
        Colors C;
        Vertex &v = _vertices[idx[i]-1];
        auto adjL = v.getAdjL();

        // C = { colors of all colored neighbors of v }
        for(int j : *adjL) {
            int cj = _vertices[j-1].getColor();
            if(cj != UNCOLORED)
                C.addColor(cj);
        }

        // Smallest color not in C
        int minCol = C.findMinCol();
        if(minCol == COLS_FULL)
            minCol = C.resizeColors();

        v.setColor(minCol);
    }
}

std::list<int> Graph::findMIS_seq(const std::vector<bool>& vMap) {
    std::vector<bool> MIS (vMap.size(), false);
    std::list<int> listMIS;

    for(int i=0; i<vMap.size(); i++) {
        if(vMap[i]) {
            auto adjL = _vertices[i].getAdjL();
            bool add = true;

            for(int j : *adjL)
                if(vMap[j-1] && MIS[j-1]) {
                    add = false;
                    break;
                }

            if(add) {
                MIS[i] = true;
                listMIS.push_back(i+1);
            }
        }
    }

    return listMIS;
}

std::list<int> Graph::findMIS_Luby(const dynamic_bitset<> & vMap) {
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
                unsigned int deg_v = _vertices[i].getDegree(vMapTmp);
                auto adjL = *_vertices[i].getAdjL();
                for(auto j : adjL) {
                    if(X[j-1]) {
                        unsigned int deg_w = _vertices[j-1].getDegree(vMapTmp);
                        deg_v <= deg_w ? I[i] = false : I[j-1] = false;
                    }
                }
            }
        }

        // Update MIS and temporary vMap
        MIS |= I;
        dynamic_bitset N_I { vMap.size() };
        for(int i=0; i<V; i++)
            if(I[i]) {
                listMIS.emplace_back(i+1);
                auto adjL = *_vertices[i].getAdjL();
                for(auto j : adjL)
                    if(vMap[j-1])
                        N_I[j-1] = true;
            }
        dynamic_bitset Y = I | N_I;
        vMapTmp -= Y;
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
        std::list<int> x = findMIS_Luby(vMap);
        for(auto idx : x) {
            _vertices[idx-1].setColor(tmpCol);
            vMap[idx-1] = false;
        }
        tmpCol++;
        U -= x.size();
    }

}

