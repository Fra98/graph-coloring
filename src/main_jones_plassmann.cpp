#include <iostream>

#include "Graph/Graph.h"
#include "Solve/JonesPlassmann.h"
#include "Utils/DurationLogger.h"

using namespace std;

int main() {
    Graph G = loadGraph("../assets/rgg_n_2_18_s0.graph");

    auto solver = std::make_unique<JonesPlassmann>();
    {
        DurationLogger dl {solver->name()};
        solver->solve(G);
    }

    if(G.isColored())
        cout << "Correct Coloring! " << endl;
    else
        cout << "Incorrect Coloring! " << endl;

//    cout << "Number of colors used: " << G->numColorsUsed() << endl;
//    cout << "Max degree: " << G->getMaxDegree() << endl;

    return 0;
}
