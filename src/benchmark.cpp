#include <iostream>
#include <memory>

#include "Utils/DurationLogger.h"
#include "Graph/Graph.h"
#include "Solve/Greedy.h"
#include "Solve/Luby.h"
#include "Solve/JonesPlassmann.h"
#include "Solve/LDF.h"

using namespace std;

#define NUM_ALGORITHMS 4

int vetNumThreads[5] = {2, 4, 8, 12, MAX_THREADS};
std::array graphs = {
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


std::pair<double, int> benchmarkColoring(Graph &G, const unique_ptr<Solver> &solver, bool reset = false) {
    DurationLogger dl {solver->name() + " (threads: " + to_string(solver->getNumThreads()) + ")"};
    dl.start();
    solver->solve(G);
    auto duration = dl.stop();

    G.isColored() ? cout << "Correct Coloring!" << endl : throw std::runtime_error("Incorrect Coloring");

    auto numColorsUsed = G.numColorsUsed();
    cout << "Number of colors used: " << numColorsUsed << endl;

    if(reset)
        G.resetColors();

    return {duration, numColorsUsed};
}

void printStats(FILE *outFile, const string &graphName, Graph &G, uint maxDegree, const string &algorithm,
                size_t numThreads,double loadTime, double coloringTime, int numColorsUsed) {
    fprintf(outFile, "%s,%d,%d,%d,%s,%lu,%.6f,%.6f,%d\n",
           graphName.c_str(),
           G.getV(),
           G.getE(),
           maxDegree,
           algorithm.c_str(),
           numThreads,
           loadTime,
           coloringTime,
           numColorsUsed);
}

std::string getPath(const std::string &graphName) {
    for(auto const& entry : std::filesystem::recursive_directory_iterator("../assets"))
        if(entry.is_regular_file() and entry.path().filename() == graphName)
                return entry.path().string();
}

int main(int argc, char **argv) {
    if(argc != 2)
        throw std::runtime_error("Arguments: cpuName");

    std::string cpu = std::string(argv[1]);
    std::string outPath = "../../Benchmarks/benchmarks_" + cpu + ".csv";
    FILE *outFile = fopen(outPath.c_str(), "w");
    if(outFile == nullptr)
        exit(1);

    // CSV Header
    fprintf(outFile, "Graph,V,E,MaxDegree,Algorithm,NumThreads,LoadTime,ColoringTime,NumColorsUsed\n");

    for(auto& graphName : graphs) {
        cout << "Graph name: " << graphName << endl;
        std::string path = getPath(graphName);

        DurationLogger dl{"Loading " + string(graphName)};
        dl.start();
        Graph G = loadGraph(path, false);
        auto loadTime = dl.stop();

        auto maxDegree = G.getMaxDegree();
        cout << "Max degree: " << maxDegree << endl;

        // Sequential
        std::unique_ptr<Solver> solverG = std::make_unique<Greedy>();
        auto [coloringTimeG, numColorsUsedG] = benchmarkColoring(G, solverG, true);
        printStats(outFile, graphName, G, maxDegree, solverG->name(), solverG->getNumThreads(),
                   loadTime, coloringTimeG, numColorsUsedG);

        // Parallel
        for(auto numThreads: vetNumThreads) {
            std::vector<unique_ptr<Solver>> solvers;
            solvers.emplace_back(std::make_unique<Luby>(numThreads));
            solvers.emplace_back(std::make_unique<JonesPlassmann>(numThreads));
            solvers.emplace_back(std::make_unique<LDF>(numThreads));

            for(auto& solver: solvers) {
                auto[coloringTime, numColorsUsed] = benchmarkColoring(G, solver, true);
                printStats(outFile, graphName, G, maxDegree, solver->name(), solver->getNumThreads(),
                           loadTime, coloringTime, numColorsUsed);
            }
        }
    }

    fclose(outFile);
    return 0;
}
