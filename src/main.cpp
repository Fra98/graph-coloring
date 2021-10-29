//
// Created by fra on 29/10/21.
//

#include <iostream>

#include "Graph/Graph.h"
#include "Utils/DurationLogger.h"

using namespace std;

int main() {
    unique_ptr<Graph> G = loadGraph("../assets/rgg_n_2_21_s0.graph");

    G->coloringSeqGreedy();

    {
        DurationLogger d {"Test Check Coloring SEQUENTIAL"};
        if(G->isColored())
            cout << "Correct Coloring! " << endl;
        else
            cout << "Incorrect Coloring! " << endl;
    }

    {
        DurationLogger d {"Test Check Coloring PARALLEL"};
        if(G->isColoredPar())
            cout << "Correct Coloring! " << endl;
        else
            cout << "Incorrect Coloring! " << endl;
    }


    return 0;
}
