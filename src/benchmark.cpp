#include <iostream>
#include <memory>

#include "Utils/DurationLogger.h"
#include "Utils/Benchmark.h"
#include "Graph/Graph.h"
#include "Solve/Greedy.h"
#include "Solve/Luby.h"
#include "Solve/JonesPlassmann.h"
#include "Solve/LDF.h"

using namespace std;

const int vetNumThreads[] {2, 4, 8, 12, MAX_THREADS};
const std::array graphs {
        "rgg_n_2_15_s0.graph",
        "rgg_n_2_16_s0.graph",
//        "rgg_n_2_17_s0.graph",
//        "rgg_n_2_18_s0.graph",
//        "rgg_n_2_19_s0.graph",
//        "rgg_n_2_20_s0.graph",
//        "rgg_n_2_21_s0.graph",
//        "rgg_n_2_22_s0.graph",
//        "rgg_n_2_23_s0.graph",
//        "rgg_n_2_24_s0.graph",
//        "ba10k5d.gra",
//        "arXiv_sub_6000-1.gra",
//        "human_dag_uniq.gra",
//        "v100.gra",
//        "v1000.gra",
//        "citeseer.scc.gra",
//        "uniprotenc_150m.scc.gra"
};

int main(int argc, char **argv) {
    if(argc != 2)
        throw std::runtime_error("Arguments: cpuName");

    std::string cpu = std::string(argv[1]);
    std::string outPath = "../../Benchmarks/benchmarks_" + cpu + ".csv";
    FILE *outFile = fopen(outPath.c_str(), "w");
    if(outFile == nullptr)
        exit(1);

    std::vector<unique_ptr<Solver>> solvers;
    solvers.emplace_back(std::make_unique<Greedy>());
    for(auto numThreads: vetNumThreads) {
        if(numThreads >= int(std::thread::hardware_concurrency()))
            continue;
        solvers.emplace_back(std::make_unique<Luby>(numThreads));
        solvers.emplace_back(std::make_unique<JonesPlassmann>(numThreads));
        solvers.emplace_back(std::make_unique<LDF>(numThreads));
    }

    printHeaderCSV(outFile);

    for(auto& graphName : graphs) {
        DurationLogger dl{"Loading " + string(graphName)};
        dl.start();
        Graph G = loadGraph(getPath(graphName), false);
        auto loadTime = dl.stop();

        cout << "Graph name: " << graphName << endl;
        auto maxDegree = G.getMaxDegree();
        cout << "Max degree: " << maxDegree << endl;

        for(auto& solver: solvers) {
            auto[coloringTime, numColorsUsed] = benchmarkColoring(G, solver, true);
            printStatsCSV(outFile, graphName, G, maxDegree, solver->name(), solver->getNumThreads(),
                       loadTime, coloringTime, numColorsUsed);
        }
    }

    fclose(outFile);
    return 0;
}
