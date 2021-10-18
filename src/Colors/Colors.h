//
// Created by fra on 18/10/21.
//

#ifndef SRC_COLORS_H
#define SRC_COLORS_H

#include <vector>

#define MAX_SIZE 32
#define COLS_FULL 1000

class Colors {
public:
    std::vector<bool> colors;

    Colors();

    int findMinCol();

    void addColor(int col);
};

#endif //SRC_COLORS_H
