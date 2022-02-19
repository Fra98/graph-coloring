#include "Graph.h"

Graph loadGraph_GRAPH(const std::string &fileName) {
    std::ifstream fin {fileName};
    std::string line;
    int numV, numE, v1, v2;

    if (!fin.is_open()) {
        std::cout << "Could not open the file\n";
        exit(-1);
    }

    // Get V and E on first line
    std::getline(fin, line);
    std::stringstream(line) >> numV >> numE;
    Graph G {numV, numE};

    // Read vertex adjacency list line by line
    v1 = 0;
    while(std::getline(fin, line)) {
        std::istringstream iss {line};
        while(iss >> v2)
            G.addEdge(v1, v2-1);
        v1++;
    }

    fin.close();
    return G;
}

Graph parallel_loadGraph_GRAPH(const std::string &fileName) {
    size_t numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> consumers;
    using pair_t = std::pair<int, std::string>;
    int BUFFER_LIMIT = 50;
    Jobs<pair_t> jobs {BUFFER_LIMIT};

    std::ifstream fin {fileName};
    std::string line;
    int numV, numE;

    if (!fin.is_open()) {
        std::cout << "Could not open the file\n";
        exit(-1);
    }

    // Get V and E on first line
    std::getline(fin, line);
    std::stringstream(line) >> numV >> numE;
    Graph G {numV, numE};

    consumers.reserve(numThreads-1);
    for(int i=0; i<numThreads-1; i++)
        consumers.emplace_back(std::thread([&jobs, &G]() {
            while(true) {
                std::optional<pair_t> job = jobs.get();
                if(!job)
                    break;

                int v2;
                std::istringstream iss {job->second};
                while(iss >> v2)
                    G.addEdge(job->first, v2-1);
            }
        }));

    // Read vertex adjacency list line by line
    int v1 = 0;
    while(std::getline(fin, line)) {
        jobs.put(std::pair<int, std::string>(v1, line));
        v1++;
    }
    jobs.notifyProdFinished();

    fin.close();

    for (auto& a : consumers)
        a.join();

    return G;
}

Graph loadGraph_GRA(const std::string &fileName) {
    std::ifstream fin {fileName};
    std::string line;
    int numV, numE=0, v1=0, v2;
    size_t delimiterPos;
    const std::string DELIMITER = ": ";

    if (!fin.is_open()) {
        std::cout << "Could not open the file\n";
        exit(-1);
    }

    std::getline(fin, line);
    std::stringstream(line) >> numV;
    Graph G {numV, numE};

    while(std::getline(fin, line)) {
        delimiterPos = line.find(DELIMITER);
        std::istringstream iss {line.substr(delimiterPos+2)};
        while(iss >> v2) {
            G.addEdge(v1, v2);
            G.addEdge(v2, v1);
            numE+=2;
        }
        v1++;
    }
    G.setEdgeNumber(numE);
    fin.close();
    return G;
}

Graph loadGraph(const std::string &fileName, bool multithreading) {
    //checking the extension to call the right loading function.
    auto ext = std::filesystem::path(fileName).extension();
    if(ext == ".gra") {                             // DIMACS
        if (multithreading) {
            std::cerr << "Parallel graph loading not available on DIMACS graph files (.gra)";
            std::exit(3);
        }
        return loadGraph_GRA(fileName);
    }
    else if(ext == ".graph" || ext == ".txt") {     // DIMACS10
        if(multithreading)
            return parallel_loadGraph_GRAPH(fileName);
        else
            return loadGraph_GRAPH(fileName);
    }

    std::cerr << "Wrong input format" << std::endl;
    exit(4);
}