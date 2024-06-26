#pragma once
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>

static std::mutex render_mut;

class ThreadPool {
public:
    void Start();
    void QueueJob(const std::function<void()> job);
    void Stop();
    bool busy();

private:
    void ThreadLoop();
    int busycounter = 0;                     // Keeps track of how many threads are busy
    bool should_terminate = false;           // Tells threads to stop looking for jobs
    std::mutex queue_mutex;                  // Prevents data races to the job queue
    std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination 
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;
};