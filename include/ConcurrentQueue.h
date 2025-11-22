#pragma once
#include <queue>
#include <string>
#include <semaphore.h>
#include <mutex>
#include <climits>
class ConcurrentQueue {


    std::queue<std::string> q;
    std::mutex mtx;
    sem_t sem;


    public:
        ConcurrentQueue();
        ~ConcurrentQueue();
        void put(std::string message);
        std::string consume();
};