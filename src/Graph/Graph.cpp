//
// Created by fra on 18/10/21.
//

#include "Graph.h"

Graph::Graph(int V, int E) : V(V), E(E) {
    _vertices.reserve(V);
    for(int i=0; i<V; i++)
        _vertices.emplace_back(Vertex{i+1});
}

void Graph::addEdge(int v1, int v2) {
    _vertices[v1-1].addEdge(v2);
}

void Graph::coloringSeqGreedy() {
    std::vector<int> idx = randomPermutation(V);

    for(int i=0; i<V;  i++) {
        Colors C;
        Vertex &v = _vertices[idx[i]-1];
        auto adjL = v.getAdjL();

        // C = { colors of all colored neighbors of v }
        for(int j : *adjL) {
            int cj = _vertices[j-1].getColor();
            if(cj != UNCOLORED)
                C.addColor(cj);
        }

        // Smallest color not in C
        int minCol = C.findMinCol();
        if(minCol == COLS_FULL)
            exit(-2);

        v.setColor(minCol);
    }
}

std::unique_ptr<Graph> loadGraph(const std::string &fileName) {
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
    v1 = 1;
    while(std::getline(fin, line)) {
        std::istringstream iss {line};
        while(iss >> v2)
            G.addEdge(v1, v2);
        v1++;
    }

    fin.close();

    return std::make_unique<Graph>(G);
}


