#include <iostream>

#include "Graph/Graph.h"
#include "Utils/DurationLogger.h"

using namespace std;

int main() {
    Graph G = loadGraph("../assets/rgg_n_2_18_s0.graph");

    {
        DurationLogger dl{"Greedy"};
        G.coloringSeqGreedy();
    }

    if(G.isColored())
        cout << "Correct Coloring! " << endl;
    else
        cout << "Incorrect Coloring! " << endl;

    return 0;
}
