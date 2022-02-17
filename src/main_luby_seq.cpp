#include <iostream>

#include "Graph/Graph.h"
#include "Utils/DurationLogger.h"

using namespace std;

int main() {
    Graph G = loadGraph("../assets/DIMACS10/rgg_n_2_18_s0.graph");

    DurationLogger dl {"Luby Seq"};
    dl.start();
    G.coloringSeqLuby();
    dl.stop();

    if(G.isColored())
        cout << "Correct Coloring! " << endl;
    else
        cout << "Incorrect Coloring! " << endl;

    return 0;
}
