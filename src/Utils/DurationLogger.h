//
// Created by francesco on 02/04/21.
//

#ifndef LAB1_PROJECT_DURATIONLOGGER_H
#define LAB1_PROJECT_DURATIONLOGGER_H

#include <iostream>
#include <string>

using namespace std;

class DurationLogger {
private:
    clock_t _start;
    string _name;

public:
    explicit DurationLogger(const string &name);
    ~DurationLogger();
};


#endif //LAB1_PROJECT_DURATIONLOGGER_H
