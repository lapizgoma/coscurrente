#include "logger.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>

Logger::Logger() {
    file.open(filename, std::ios::app);
}

Logger::~Logger() {
    close();
}

void Logger::init() {
    file.open(filename, std::ios::app);
}

void Logger::log(const Job &job) {
    if (!file.is_open()) {
        file.open(filename, std::ios::app);
    }

    loggerMtx.lock();

    time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm *ltm = localtime(&now);

    char buffer[128];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    std::string timestamp_str = buffer;

    std::string state_str;
    switch (job.estado)
    {
        case Estado::CREADO:
            state_str = "CREADO";
            break;
        case Estado::EN_COLA:
            state_str = "EN_COLA";
            break;
        case Estado::ASIGNADO_VRAM:
            state_str = "ASIGNADO_VRAM";
            break;
        case Estado::FINALIZADO:
            state_str = "FINALIZADO";
            break;
        default:
            state_str = "ESTADO_NO_CONTEMPLADO";
            break;
    }


    std::string priority_str;
    switch (job.prioridad) 
    {
        case Prioridad::FREE: 
            priority_str = "FREE";
            break;
        case Prioridad::PREMIUM: 
            priority_str = "PREMIUM";
            break;
        default: 
            priority_str = "PRIORIDAD_NO_CONTEMPLADA";
            break;
    }

    file << "[" << timestamp_str << "] - " << "Job " << job.id << " - " << priority_str << " - " << "Evento (" << state_str << ")" << "\n";

    file.flush();
    loggerMtx.unlock();
}

void Logger::close() {
    if (file.is_open()) {
        file.close();
    }
}
