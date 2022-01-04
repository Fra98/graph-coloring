#include <iostream>

#include "Graph/Graph.h"
#include "Utils/DurationLogger.h"
#include "Solve/Greedy.h"
#include "Solve/Luby.h"
#include "Solve/JonesPlassmann.h"
#include "Solve/LDF.h"

using namespace std;

int main(int argc, char **argv) {
    if(argc != 2) {
        cerr << "Missing algorithm number: 0 (all), 1 (greedy), 2 (Luby), 3 (Jones Plassmann), 4 (LDF)" << std::endl;
        return 1;
    }

    int algorithm = std::stoi(argv[1]);
    Graph G = loadGraph("../assets/DIMACS/large/cit-Patents.scc.gra");
    unique_ptr<Solver> solver;

    switch(algorithm) {
        case 0:
            cout << "Not yet implemented" << endl;
            return 1;
        case 1:
            solver = std::make_unique<Greedy>();
            break;
        case 2:
            solver = std::make_unique<Luby>();
            break;
        case 3:
            solver = std::make_unique<JonesPlassmann>();
            break;
        case 4:
            solver = std::make_unique<LDF>();
            break;
        default:
            cerr << "Invalid algorithm number: 0 (all), 1 (greedy), 2 (Luby), 3 (Jones Plassmann), 4 (LDF)" << endl;
            return 0;
    }

    DurationLogger dl {solver->name()};
    dl.start();
    solver->solve(G);
    dl.stop();

    G.isColored() ? cout << "Correct Coloring! " << endl : cout << "Incorrect Coloring! " << endl;

    cout << "Number of colors used: " << G.numColorsUsed() << endl;
    cout << "Max degree: " << G.getMaxDegree() << endl;

    return 0;
}
