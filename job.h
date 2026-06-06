#pragma once
#include <chrono>

enum class Prioridad {
    FREE = 0,
    PREMIUM = 1
};

enum class Estado {
    CREADO,
    EN_COLA,
    ASIGNADO_VRAM,
    FINALIZADO
};

struct Job {
    int id;
    Prioridad prioridad;
    Estado estado;
    std::chrono::steady_clock::time_point tiempoCreacion;

    Job(int _id, Prioridad _prioridad) {
        id = _id;
        prioridad = _prioridad;
        estado = Estado::CREADO;
        tiempoCreacion = std::chrono::steady_clock::now();
    }

    Job() {
        id = -1;
        prioridad = Prioridad::FREE;
        estado = Estado::CREADO;
    }
};