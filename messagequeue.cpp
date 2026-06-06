#include "messagequeue.h"
#include <algorithm>
#include <chrono>

static bool jobComparator(const Job& a, const Job& b,
                          std::chrono::steady_clock::time_point now) {
    auto waitA = std::chrono::duration_cast<std::chrono::milliseconds>(
                     now - a.tiempoCreacion).count();
    auto waitB = std::chrono::duration_cast<std::chrono::milliseconds>(
                     now - b.tiempoCreacion).count();

    bool aStarving = waitA > 5000;
    bool bStarving = waitB > 5000;

    if (aStarving != bStarving) return aStarving;

    if (a.prioridad != b.prioridad)
        return a.prioridad > b.prioridad;

    return a.tiempoCreacion < b.tiempoCreacion;
}

void MessageQueue::Enqueue(const Job& job) {
    mtx.lock();
    queue.push_back(job);
    auto now = std::chrono::steady_clock::now();
    std::sort(queue.begin(), queue.end(),
              [now](const Job& a, const Job& b) {
                  return jobComparator(a, b, now);
              });
    mtx.unlock();
}

Job MessageQueue::Dequeue() {
    mtx.lock();
    if (queue.empty()) {
        mtx.unlock();
        return Job();
    }
    Job job = queue.front();
    queue.erase(queue.begin());
    mtx.unlock();
    return job;
}

int MessageQueue::Count() {
    mtx.lock();
    int count = static_cast<int>(queue.size());
    mtx.unlock();
    return count;
}
