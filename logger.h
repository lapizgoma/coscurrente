#pragma once
#include "job.h"
#include <fstream>
#include <mutex>

class Logger {
public:
    Logger();
    ~Logger();

    void init();
    void log(const Job &job);
    void close();

private:
    std::ofstream file;
    std::mutex loggerMtx;
    const char* filename = "sistema.log";
};