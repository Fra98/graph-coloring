#ifndef SRC_DURATIONLOGGER_H
#define SRC_DURATIONLOGGER_H

#include <iostream>
#include <string>
#include <chrono>

using namespace std;

class DurationLogger {
private:
    std::chrono::steady_clock::time_point _start;
    string _name;

public:
    explicit DurationLogger(const string &name) {
        _name = name;
    }

    ~DurationLogger() = default;

    void start() {
        cout << ">>>>>>>>>>>>>>> Starting " << _name << endl;
        _start = std::chrono::steady_clock::now();
    }

    double stop() {
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration<double>(end - _start).count();
        cout << ">>>>>>>>>>>>>>> Ending " << _name
            << ", Duration: " << duration << " seconds" << endl;
        return duration;
    }
};


#endif //SRC_DURATIONLOGGER_H
