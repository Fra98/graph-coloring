//
// Created by fra on 18/10/21.
//

#include <iostream>
#include "Utils.h"

std::vector<int> randomPermutation(unsigned int N) {
    std::vector<int> v(N);
    for (int i = 0; i < N; i++)
        v[i] = i;

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

int randomWithinRange(int a, int b) {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 g(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(a, b);

    return distrib(g);
}

unsigned int findPrimeWithinRange(unsigned int a, unsigned int b) {
    if (a <= 2) {
        a = 2;
        if (b >= 2)
            a++;
    }

    if (a % 2 == 0)
        a++;

    for(auto i=a; i<=b; i=i+2) {
        bool flag = true;
        for(auto j=2; j*j <= i; ++j)
            if(i % j == 0) {
                flag = false;
                break;
            }
        if(flag)
            return i;
    }

    return 0;   // no prime found
}