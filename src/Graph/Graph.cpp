//
// Created by fra on 18/10/21.
//

#include "Graph.h"

Graph::Graph(int V, int E) : V(V), E(E) {
    _vertices.reserve(V);
    for(int i=0; i<V; i++)
        _vertices.emplace_back(Vertex{i});
}

unsigned int Graph::getV() const {
    return V;
}

unsigned int Graph::getE() const {
    return E;
}

void Graph::addEdge(int v1, int v2) {
    _vertices[v1].addEdge(v2);
}

bool Graph::isColored() {
    for(int i=0; i<V; i++) {
        Vertex &v = _vertices[i];

        int v_col = v.getColor();
        if(v_col == UNCOLORED)
            return false;

        auto adjL = v.getAdjL();
        for(int j : *adjL) {
            int v2_col = _vertices[j].getColor();
            if (v2_col == v_col || v2_col == UNCOLORED)
                return false;
        }
    }

    return true;
}

bool Graph::isColoredPar() {
    std::atomic_bool colored = true;

    std::for_each(
            std::execution::par_unseq,
            _vertices.begin(),
            _vertices.end(),
            [this, &colored](Vertex &v) {
                int v_col = v.getColor();
                if(v_col == UNCOLORED) {
                    colored.store(false);
                    return;
                }

                auto adjL = v.getAdjL();
                for(int j : *adjL) {
                    int v2_col = _vertices[j].getColor();
                    if (v2_col == v_col || v2_col == UNCOLORED) {
                        colored.store(false);
                        return;
                    }
                }
            }
            );

    return colored;
}

bool Graph::isMIS(const boost::dynamic_bitset<> &vMap) {
    for(int i=0; i<V; i++) {
        auto adjL = _vertices[i].getAdjL();
        for(int j : *adjL)
            if(vMap[i] && vMap[j])
                return false;
    }
    return true;
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
    v1 = 0;
    while(std::getline(fin, line)) {
        std::istringstream iss {line};
        while(iss >> v2)
            G.addEdge(v1, v2-1);
        v1++;
    }

    fin.close();

    return std::make_unique<Graph>(G);
}


