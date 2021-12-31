#include <iostream>

#include "Graph/Graph.h"

using namespace std;

int main() {
    Graph G = loadGraph("../assets/rgg_n_2_18_s0.graph");

    G.coloringParLuby();

    if(G.isColored())
        cout << "Correct Coloring! " << endl;
    else
        cout << "Incorrect Coloring! " << endl;

//    cout << "Number of colors used: " << G->numColorsUsed() << endl;
//    cout << "Max degree: " << G->getMaxDegree() << endl;

    return 0;
}
