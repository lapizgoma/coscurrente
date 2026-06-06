#include "consumidor.h"
#include "semaforo.h"
#include "messagequeue.h"
#include "poolvram.h"
#include <thread>
#include <chrono>

extern std::mutex mtxConsumidor;
extern Semaforo semVRAM;
extern Semaforo semJobs;
extern int consumidos;
extern int totalJobs;
extern MessageQueue mq;

void consumidor() {
    while (true) {
        mtxConsumidor.lock();
        if (consumidos >= totalJobs) {
            mtxConsumidor.unlock();
            break;
        }
        mtxConsumidor.unlock();

        wait(semVRAM);
        wait(semJobs);

        Job job = mq.Dequeue();
        if (job.id == -1) {
            signal(semVRAM);
            signal(semJobs);
            continue;
        }

        int slot = asignarJob(job);

        std::this_thread::sleep_for(std::chrono::milliseconds(600));

        liberarSlot(slot);

        mtxConsumidor.lock();
        consumidos++;
        mtxConsumidor.unlock();
    }
}
