//
// Created by francesco on 02/04/21.
//

#ifndef SRC_DURATIONLOGGER_H
#define SRC_DURATIONLOGGER_H

#include <iostream>
#include <string>
#include <chrono>

using namespace std;

class DurationLogger {
private:
    std::chrono::system_clock::time_point _start;
    string _name;

public:
    explicit DurationLogger(const string &name);
    ~DurationLogger();
};


#endif //SRC_DURATIONLOGGER_H
