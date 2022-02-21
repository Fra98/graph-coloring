#include <iostream>

#include "Graph/Graph.h"
#include "Utils/DurationLogger.h"
#include "Utils/Benchmark.h"
#include "Solve/Greedy.h"
#include "Solve/Luby.h"
#include "Solve/JonesPlassmann.h"
#include "Solve/LDF.h"

using namespace std;

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
    bool all = (algorithm == 0);

    std::vector<unique_ptr<Solver>> solvers;
    if(all or algorithm==1)
        solvers.emplace_back(std::make_unique<Greedy>());
    if(all or algorithm==2)
        solvers.emplace_back(std::make_unique<Luby>(numThreads));
    if(all or algorithm==3)
        solvers.emplace_back(std::make_unique<JonesPlassmann>(numThreads));
    if(all or algorithm==4)
        solvers.emplace_back(std::make_unique<LDF>(numThreads));

//    string graphName = "cit-Patents.scc.gra";
    string graphName = "rgg_n_2_17_s0.graph";

    DurationLogger dl {"loading sequential"};
    dl.start();
    Graph G = loadGraph(getPath(graphName));
    dl.stop();

    cout << "Graph name: " << graphName << endl;
    cout << "Max degree: " << G.getMaxDegree() << endl;

    for(auto& solver : solvers)
        benchmarkColoring(G, solver, true);

    return 0;
}
