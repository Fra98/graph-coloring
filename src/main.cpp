#include <iostream>

#include "Graph/Graph.h"
#include "Utils/DurationLogger.h"
#include "Solve/Greedy.h"
#include "Solve/Luby.h"
#include "Solve/JonesPlassmann.h"
#include "Solve/LDF.h"

using namespace std;

#define NUM_ALGORITHMS 4

void benchmarkColoring(Graph &G, const unique_ptr<Solver> &solver, bool reset = true) {
    DurationLogger dl {solver->name()};
    dl.start();
    solver->solve(G);
    dl.stop();

    G.isColored() ? cout << "Correct Coloring!" << endl : cout << "Incorrect Coloring!" << endl;

    cout << "Number of colors used: " << G.numColorsUsed() << endl;

    if(reset)
        G.resetColors();
}


int main(int argc, char **argv) {
    if(argc != 2) {
        cerr << "Missing algorithm number: 0 (ALL), 1 (Greedy), 2 (Luby), 3 (Jones Plassmann), 4 (LDF)\n";
        return 1;
    }

    int algorithm = std::stoi(argv[1]);
    if(algorithm > NUM_ALGORITHMS or algorithm < 0) {
        cerr << "Invalid algorithm number: 0 (all), 1 (greedy), 2 (Luby), 3 (Jones Plassmann), 4 (LDF)\n";
        return 2;
    }

//    string filename = "../assets/DIMACS/large/go_uniprot.gra";
    string filename = "../assets/DIMACS10/rgg_n_2_15_s0.graph";

    DurationLogger dl {"loading sequential"};
    dl.start();
    Graph G = loadGraph(filename, false);
    dl.stop();
    cout << "Max degree: " << G.getMaxDegree() << endl;

//    DurationLogger dl2 {"loading parallel"};
//    dl2.start();
//    Graph G2 = loadGraph(filename, true);
//    dl2.stop();
//    cout << "Max degree: " << G2.getMaxDegree() << endl;

    bool all = false;
    if(algorithm == 0)
        all = true;

    unique_ptr<Solver> solver;
    if(all or algorithm==1) {
        solver = std::make_unique<Greedy>();
        benchmarkColoring(G, solver, true);
    }
    if(all or algorithm==2) {
        solver = std::make_unique<Luby>();
        benchmarkColoring(G, solver, true);
    }
    if(all or algorithm==3) {
        solver = std::make_unique<JonesPlassmann>();
        benchmarkColoring(G, solver, true);
    }
    if(all or algorithm==4) {
        solver = std::make_unique<LDF>();
        benchmarkColoring(G, solver, true);
    }

    return 0;
}
