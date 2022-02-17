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
    explicit DurationLogger(const string &name) {
        _name = name;
    }

    ~DurationLogger() = default;

    void start() {
        cout << ">>>>>>>>>>>>>>> Starting " << _name << endl;
        _start = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double>(end - _start).count();
        cout << ">>>>>>>>>>>>>>> Ending " << _name << ", Duration: " << duration << endl;
    }
};


#endif //SRC_DURATIONLOGGER_H
