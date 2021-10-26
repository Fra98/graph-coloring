//
// Created by fra on 18/10/21.
//

#include <iostream>
#include "Utils.h"

std::vector<int> randomPermutation(unsigned int N) {
    std::vector<int> v(N);
    for (int i = 0; i < N; i++)
        v[i] = i + 1;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(v.begin(), v.end(), g);

    return v;
}

bool randomWithProb(double p) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::discrete_distribution<> distrib({ 1-p, p });

    return distrib(g);
}