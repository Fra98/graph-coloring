#ifndef SRC_UTILS_H
#define SRC_UTILS_H

#include <vector>
#include <random>
#include <algorithm>

std::vector<int> randomPermutation(unsigned int N);             // random permutation from 0 to N-1
bool randomWithProb(double p);
int randomWithinRange(int a, int b);
unsigned int findPrimeWithinRange(unsigned int a, unsigned int b);
unsigned int nextHighestPowerOf2(unsigned int v);

#endif //SRC_UTILS_H
