//
// Created by fra on 25/10/21.
//

#include "Graph.h"

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