# Parallel Graph Coloring

This project implements different well-known [graph coloring](https://en.wikipedia.org/wiki/Graph_coloring) algorithms, specifically vertex coloring.

Sequential algorithms:
- Greedy [[Wiki]](https://en.wikipedia.org/wiki/Graph_coloring#Greedy_coloring)

Parallel algorithms:
- Luby [[A simple parallel algorithm for the Maximal Independent Set problem, M. Luby, 1985]](http://compalg.inf.elte.hu/~tony/Oktatas/Osztott-algoritmusok/mis_1986_luby.pdf)
- Jones & Plassmann [[A parallel graph coloring heuristic, M. Jones and T. Plassmann, 1992]](https://www.researchgate.net/publication/2768023_A_Parallel_Graph_Coloring_Heuristic)
- LDF (Largest Degree First) [[A Comparison of Parallel Graph Coloring Algorithms, J. R. Allwright, 1995]](https://www.researchgate.net/publication/2296563_A_Comparison_of_Parallel_Graph_Coloring_Algorithms)

This is a short manual. Refer to DOCUMENTATION.md to read the full description of the project, containing all design implementation choices.


## Build (UNIX)

Requirements:
- CMake (version >= 3.16)
- Make
- [Boost library](https://www.boost.org/) (dynamic_bitset required)


```
cd src && mkdir build && cd build
cmake ..
make
cd ../..
```

## Run

```
cd src/build
./main [--help | -h] [--algo ALGO] [--graph GRAPH] [--threads THREADS] [--out OUT]`
```

All the following arguments are OPTIONAL:

    --help             Show help menu
    --algo ALGO        Coloring algorithm. Choices: greedy, luby, jp, LDF (default: all)
    --graph GRAPH      Name of the graph (default: all graphs in the assets folder)
    --threads THREADS  Number of threads used in parallel algorithms (default: [2,4,8,12,MAX] if available)
    --out OUT          Name of the output file (default: benchmarks.csv)


