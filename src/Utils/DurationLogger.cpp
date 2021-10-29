//
// Created by francesco on 02/04/21.
//

#include "DurationLogger.h"

DurationLogger::DurationLogger(const string &name) {
    cout << ">>>>>>>>>>>>>>> Starting " << name << endl;
    _name = name;
    _start = clock();
}

DurationLogger::~DurationLogger() {
    double duration = (double) (clock() - _start) / CLOCKS_PER_SEC;
    cout << ">>>>>>>>>>>>>>> Ending " << _name << ", Duration: " << duration << endl;
}
