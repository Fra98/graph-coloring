#ifndef SRC_JOBS_HPP
#define SRC_JOBS_HPP

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <optional>

template<class T>
class Jobs {
private:
    std::queue<T> _jobsQueue;
    int _bufferLimit;
    std::mutex _m;
    std::condition_variable _cvQueue;
    std::condition_variable _cvFullBuffer;
    std::atomic_bool _prodFinished = {false};

public:
    explicit Jobs(int bufferLimit);

    Jobs(const Jobs&) = delete;             // disable copying

    Jobs& operator=(const Jobs&) = delete;  // disable assignment

    void put(T&& job);

    std::optional<T> get();

    void notifyProdFinished();

    [[nodiscard]] bool hasProducerFinished() const;
};

template<class T>
Jobs<T>::Jobs(int bufferLimit) : _bufferLimit(bufferLimit) {}

template<class T>
void Jobs<T>::put(T&& job) {
    std::unique_lock<std::mutex> ul {_m};
    while (_jobsQueue.size() >= _bufferLimit) {
        _cvFullBuffer.wait(ul);
    }

    _jobsQueue.push(std::move(job));
    _cvQueue.notify_one();
}

template<class T>
std::optional<T> Jobs<T>::get() {
    std::unique_lock<std::mutex> ul {_m};

    _cvQueue.wait(ul, [this]() { return !_jobsQueue.empty() || (_jobsQueue.empty() && hasProducerFinished());});

    // Check if last notify means no remaining jobs
    if(_jobsQueue.empty() && hasProducerFinished())
        return std::nullopt;  // null object

    T job = std::move(_jobsQueue.front());
    _jobsQueue.pop();

    // Wake up producer if stuck on full buffer
    _cvFullBuffer.notify_one();

    return job;
}

template<class T>
void Jobs<T>::notifyProdFinished() {
    _prodFinished.store(true);
    _cvQueue.notify_all();  // wake up eventual threads waiting with no jobs remaining
}

template<class T>
bool Jobs<T>::hasProducerFinished() const {
    return _prodFinished.load();
}


#endif //SRC_JOBS_HPP
