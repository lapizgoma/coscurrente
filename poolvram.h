#pragma once
#include "job.h"
#include <vector>
#include <mutex>

struct SlotVRAM {
    Job job;
    bool ocupado = false;
};

void initPoolVRAM(int cantidadSlots);
int  asignarJob(const Job& nuevoJob);
void liberarSlot(int slotIndex);