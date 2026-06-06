#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include "semaforo.h"
#include "logger.h"
#include "messagequeue.h"
#include "poolvram.h"
#include "productor.h"
#include "consumidor.h"

Semaforo semJobs;
Semaforo  semVRAM;
std::mutex mtxProducidos;
std::mutex  mtxConsumidor;
int totalJobs  = 0;
int producidos = 0;
int consumidos = 0;
Logger logger;
MessageQueue mq;
bool soloPremiun = false;

int main(int argc, char* argv[]) {

    int numProductores  = 3;
    int numConsumidores = 3;
    totalJobs           = 10;

    if (argc >= 4) {
        numProductores  = std::stoi(argv[1]);
        numConsumidores = std::stoi(argv[2]);
        totalJobs       = std::stoi(argv[3]);
    }
    if (argc >= 5) {
        soloPremiun = (std::stoi(argv[4]) == 1);
    }

    std::cout << "=========================================\n";
    std::cout << "  Productores  : " << numProductores  << "\n";
    std::cout << "  Consumidores : " << numConsumidores << "\n";
    std::cout << "  Total Jobs   : " << totalJobs       << "\n";
    std::cout << "  Solo Premium : " << (soloPremiun ? "SI" : "NO") << "\n";
    std::cout << "  Log          : sistema.log\n";
    std::cout << "=========================================\n\n";

    initPoolVRAM(5);
    init(semJobs, 0);  
    init(semVRAM, 5);   

    std::vector<std::thread> consumidoresVec;
    std::vector<std::thread> productoresVec;

    for (int i = 0; i < numConsumidores; i++)
        consumidoresVec.emplace_back(consumidor);

    for (int i = 0; i < numProductores; i++)
        productoresVec.emplace_back(productor);

    for (auto& t : productoresVec) t.join();

    for (int i = 0; i < numConsumidores; i++) {
        signal(semVRAM);
        signal(semJobs);
    }

    for (auto& t : consumidoresVec) t.join();

    std::cout << "\n=========================================\n";
    std::cout << "  FIN\n";
    std::cout << "=========================================\n";
    std::cout << "  Jobs producidos : " << producidos << "\n";
    std::cout << "  Jobs consumidos : " << consumidos << "\n";
    std::cout << "  Consistencia    : "
              << (producidos == consumidos && consumidos == totalJobs ? "OK" : "ERROR") << "\n";
    std::cout << "=========================================\n";

    return 0;
}
