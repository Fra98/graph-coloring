//
// Created by fra on 31/12/21.
//

#include "Greedy.h"

std::string Greedy::name() const {
    return "Greedy";
}

void Greedy::solve(Graph &G) {
    std::vector<int> idx = randomPermutation(G.getV());

    auto& vertices = G.getVertices();
    for(int i=0; i<G.getV(); i++)
        G.colorVertexMinimum(vertices[idx[i]]);
}
