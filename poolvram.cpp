#include "poolvram.h"
#include "logger.h"
#include "semaforo.h"
#include <thread>
#include <iostream>
#include <chrono>
#include <vector>
#include <mutex>

extern Semaforo semVRAM; 
extern Logger logger; 
static std::vector<SlotVRAM> slots;
static int maxSlots = 0;
static std::mutex mtxPool;
static std::mutex mtxAsignacionUnica; 
static std::mutex mtxLiberacionUnica;

void initPoolVRAM(int cantidadSlots) {
    maxSlots = cantidadSlots; 
    slots.resize(static_cast<size_t>(maxSlots));
}

int asignarJob(const Job& nuevoJob) {
    if (nuevoJob.id == -1) {
        return -1;
    }
    mtxAsignacionUnica.lock();

    mtxPool.lock();
    int slotElegido = -1;
    for (int i = 0; i < maxSlots; ++i) {
        if (!slots[static_cast<size_t>(i)].ocupado) {
            slotElegido = i;
            break;
        }
    }

    if (slotElegido != -1) {
        slots[static_cast<size_t>(slotElegido)].job = nuevoJob;
        slots[static_cast<size_t>(slotElegido)].job.estado = Estado::ASIGNADO_VRAM;
        slots[static_cast<size_t>(slotElegido)].ocupado = true;

        logger.log(slots[static_cast<size_t>(slotElegido)].job);
        mtxPool.unlock(); 

        std::this_thread::sleep_for(std::chrono::milliseconds(450));
    } else {
        mtxPool.unlock();
    }

    mtxAsignacionUnica.unlock();
    return slotElegido;
}

void liberarSlot(int slotIndex) {
    mtxLiberacionUnica.lock();

    mtxPool.lock();
    slots[static_cast<size_t>(slotIndex)].job.estado = Estado::FINALIZADO;
    slots[static_cast<size_t>(slotIndex)].ocupado = false; 
    logger.log(slots[static_cast<size_t>(slotIndex)].job); 
    mtxPool.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    mtxLiberacionUnica.unlock();

    signal(semVRAM); 
}