#include <iostream>
#include <memory>

#include "Utils/DurationLogger.h"
#include "Utils/Benchmark.h"
#include "Graph/Graph.h"
#include "Solve/Greedy.h"
#include "Solve/Luby.h"
#include "Solve/JonesPlassmann.h"
#include "Solve/LDF.h"


std::optional<std::string> getCmdOption(const std::vector<std::string> &args, const std::string &option) {
    auto itr = std::find(args.begin(), args.end(), option);
    if (itr != args.end() && ++itr != args.end())
        return *itr;
    return std::nullopt;
}

bool hasOption(const std::vector<std::string> &args, const std::string &option) {
    return std::find(args.begin(), args.end(), option) != args.end();
}

const char* usage = "Usage: main [--help | -h] [--algo ALGO] [--graph GRAPH] [--threads THREADS] [--out OUT]\n\n"
                    "OPTIONALS arguments:\n"
                    "\t--help             Show help menu\n"
                    "\t--algo ALGO        Coloring algorithm. Choices: greedy, luby, jp, LDF (default: all)\n"
                    "\t--graph GRAPH      Name of the graph (default: all graphs in the assets folder)\n"
                    "\t--threads THREADS  Number of threads used in parallel algorithms (default: [2,4,8,12,MAX] if available)\n"
                    "\t--out OUT          Name of the output file (default: benchmarks.csv)\n";

void parseArguments(const std::vector<std::string> &args, std::string &algo, std::vector<string> &paths,
                    std::vector<int> &vetNumThreads, std::string &outFile) {
    std::optional<string> opt;

    if(hasOption(args, "--help") || hasOption(args, "-h")) {
        std::cout << usage << std::endl;
        std::exit(0);
    }

    opt = getCmdOption(args, "--algo");
    if(opt) {
        if(std::find(ALGORITHMS.begin(), ALGORITHMS.end(), opt.value()) != ALGORITHMS.end())
            algo = { opt.value() };
        else
            throw std::runtime_error("Invalid coloring algorithm. Choices: greedy, luby, jp, LDF");
    }
    else
        algo = "all";   // default

    opt = getCmdOption(args, "--graph");
    if(opt)
        paths = { getPath(opt.value()) };
    else
        paths = getAllPaths();  // default

    opt = getCmdOption(args, "--threads");
    if(opt) {
        int numThreads = { std::stoi(opt.value()) };
        if(numThreads > std::thread::hardware_concurrency())
            throw std::runtime_error("Hardware concurrency exceeded");
        vetNumThreads = { numThreads };
    }
    else
        vetNumThreads = { 2, 4, 8, 12, MAX_THREADS };  // default

    opt = getCmdOption(args, "--out");
    if(opt)
        outFile = std::string(BENCHMARK_DIR) + opt.value();
    else
        outFile = std::string(BENCHMARK_DIR) + "results.csv";  // default
}


int main(int argc, char **argv) {
    std::vector<std::string> args(argv, argv + argc);
    std::string algo, outPath;
    std::vector<string> paths;
    std::vector<int> vetNumThreads;

    parseArguments(args, algo, paths, vetNumThreads, outPath);

    std::vector<unique_ptr<Solver>> solvers;
    if(algo == "all" || algo == "greedy")
        solvers.emplace_back(std::make_unique<Greedy>());
    for(auto numThreads: vetNumThreads) {
        if(numThreads > int(std::thread::hardware_concurrency()))
            continue;
        if(algo == "all" || algo == "luby")
            solvers.emplace_back(std::make_unique<Luby>(numThreads));
        if(algo == "all" || algo == "jp")
            solvers.emplace_back(std::make_unique<JonesPlassmann>(numThreads));
        if(algo == "all" || algo == "LDF")
            solvers.emplace_back(std::make_unique<LDF>(numThreads));
    }

    FILE *out = fopen(outPath.c_str(), "w");
    if(out == nullptr)  std::exit(1);

    printHeaderCSV(out);

    for(auto& path : paths) {
        std::string graphName = std::filesystem::path(path).filename().string();
        DurationLogger dl{ "Loading " + graphName };
        dl.start();
        Graph G = loadGraph(path);
        auto loadTime = dl.stop();

        auto maxDegree = G.getMaxDegree();
        cout << "Max degree: " << maxDegree << endl;

        for(auto& solver: solvers) {
            auto[coloringTime, numColorsUsed] = benchmarkColoring(G, solver, true);
            printStatsCSV(out, graphName, G, maxDegree, solver->name(), solver->getNumThreads(),
                       loadTime, coloringTime, numColorsUsed);
        }
    }

    fclose(out);
    return 0;
}
