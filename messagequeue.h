#pragma once
#include <vector>
#include <mutex>
#include "job.h"

class MessageQueue {
public:
    void Enqueue(const Job& job);
    Job Dequeue();
    int Count();

private:
    std::vector<Job> queue;
    std::mutex mtx;
};
