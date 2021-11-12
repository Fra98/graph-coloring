#include <iostream>

#include "Graph/Graph.h"

using namespace std;

int main() {
    unique_ptr<Graph> G = loadGraph("../assets/rgg_n_2_20_s0.graph");

    G->coloringJonesPlassmann();

    if(G->isColored())
        cout << "Correct Coloring! " << endl;
    else
        cout << "Incorrect Coloring! " << endl;

//    cout << "Number of colors used: " << G->numColorsUsed() << endl;
//    cout << "Max degree: " << G->getMaxDegree() << endl;

    return 0;
}
