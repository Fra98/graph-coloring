#include <iostream>

#include "Graph/Graph.h"

using namespace std;

int main() {
    unique_ptr<Graph> G = loadGraph("../assets/rgg_n_2_15_s0.graph");

    G->coloringSeqGreedy();

    if(G->isColored())
        cout << "Correct Coloring! " << endl;
    else
        cout << "Incorrect Coloring! " << endl;

    return 0;
}
