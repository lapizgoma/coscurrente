#include "productor.h"
#include "semaforo.h"
#include "messagequeue.h"
#include "logger.h"
#include <thread>
#include <chrono>

extern Semaforo semJobs;
extern std::mutex mtxProducidos;
extern int totalJobs;
extern int  producidos;
extern MessageQueue mq;
extern Logger logger;
extern bool soloPremiun;
extern int topePremium;

void productor() {
    while (true) {
        int id;

        mtxProducidos.lock();
        if (producidos >= totalJobs) {
            mtxProducidos.unlock();
            break;
        }
        id = ++producidos;
        mtxProducidos.unlock();

        Prioridad prio;
        if (topePremium > 0) {
            prio = (id <= topePremium) ? Prioridad::PREMIUM : Prioridad::FREE;
        } else {
            prio = soloPremiun
                ? Prioridad::PREMIUM
                : (id % 2 == 0 ? Prioridad::PREMIUM : Prioridad::FREE);
        }

        Job job(id, prio);

        logger.log(job);            

        job.estado = Estado::EN_COLA;
        logger.log(job);              
        mq.Enqueue(job);

        signal(semJobs);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
