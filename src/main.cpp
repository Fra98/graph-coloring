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
    DurationLogger dl {solver->name() + " (threads: " + to_string(solver->getNumThreads()) + ")"};
    dl.start();
    solver->solve(G);
    dl.stop();

    G.isColored() ? cout << "Correct Coloring!" << endl : cout << "Incorrect Coloring!" << endl;

    cout << "Number of colors used: " << G.numColorsUsed() << endl;

    if(reset)
        G.resetColors();
}

int main(int argc, char **argv) {
    if(argc != 3) {
        cerr << "Expected 3 arguments\n";
        return 1;
    }

    int algorithm = std::stoi(argv[1]);
    if(algorithm > NUM_ALGORITHMS or algorithm < 0) {
        cerr << "Invalid algorithm number: 0 (all), 1 (greedy), 2 (Luby), 3 (Jones Plassmann), 4 (LDF)\n";
        return 2;
    }

    int numThreads = std::stoi(argv[2]);

//    string path = "../assets/DIMACS/large/cit-Patents.scc.gra";
    string path = "../assets/DIMACS10/rgg_n_2_17_s0.graph";
    cout << "Graph name: " << std::filesystem::path(path).filename() << endl;

    DurationLogger dl {"loading sequential"};
    dl.start();
    Graph G = loadGraph(path, false);
    dl.stop();
    cout << "Max degree: " << G.getMaxDegree() << endl;

//    DurationLogger dl2 {"loading parallel"};
//    dl2.start();
//    Graph G2 = loadGraph(path, true);
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
        solver = std::make_unique<Luby>(numThreads);
        benchmarkColoring(G, solver, true);
    }
    if(all or algorithm==3) {
        solver = std::make_unique<JonesPlassmann>(numThreads);
        benchmarkColoring(G, solver, true);
    }
    if(all or algorithm==4) {
        solver = std::make_unique<LDF>(numThreads);
        benchmarkColoring(G, solver, true);
    }

    return 0;
}
