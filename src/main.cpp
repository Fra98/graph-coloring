#include <iostream>

#include "Graph/Graph.h"

using namespace std;

int main() {
    unique_ptr<Graph> G = loadGraph("../assets/test.graph");


    return 0;
}
