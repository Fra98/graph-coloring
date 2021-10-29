//
// Created by fra on 18/10/21.
//

#ifndef SRC_UTILS_H
#define SRC_UTILS_H

#include <vector>
#include <random>
#include <algorithm>

// Random permutation from 1 to N
std::vector<int> randomPermutation(unsigned int N);
bool randomWithProb(double p);
int randomWithinRange(int a, int b);
unsigned int findPrimeWithinRange(unsigned int a, unsigned int b);



#endif //SRC_UTILS_H
