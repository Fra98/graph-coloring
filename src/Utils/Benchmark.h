#ifndef SRC_BENCHMARK_H
#define SRC_BENCHMARK_H

#include "../Graph/Graph.h"
#include "../Solve/Solver.h"
#include "DurationLogger.h"

#define ASSETS_DIR "../assets"
#define NUM_ALGORITHMS 4

std::pair<double, int> benchmarkColoring(Graph &G, const unique_ptr<Solver> &solver, bool reset) {
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

std::string getPath(const std::string &graphName) {
    for(auto const& entry : std::filesystem::recursive_directory_iterator(ASSETS_DIR))
        if(entry.is_regular_file() and entry.path().filename() == graphName)
            return entry.path().string();

    throw std::runtime_error("Graph " + graphName + " not found on assets folder");
}

// CSV UTILS
void printHeaderCSV(FILE *outFile) {
    fprintf(outFile, "Graph,V,E,MaxDegree,Algorithm,NumThreads,LoadTime,ColoringTime,NumColorsUsed\n");
}

void printStatsCSV(FILE *outFile, const string &graphName, Graph &G, uint maxDegree, const string &algorithm,
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


#endif //SRC_BENCHMARK_H
