#include <queue>
#include <string>
#include <semaphore>
#include <mutex>
#include "ConcurrentQueue.h"

ConcurrentQueue::ConcurrentQueue() {
    if(sem_init(&sem,0, 0) != 0) {
        throw std::runtime_error("could not init semaphore");
    }
}
ConcurrentQueue::~ConcurrentQueue() {
    sem_destroy(&sem);
}
        void ConcurrentQueue::put(std::string message) {
            {
                std::lock_guard<std::mutex> g(mtx);
                q.push(message);
            }
            sem_post(&sem);

        }
        std::string ConcurrentQueue::consume() {
            sem_wait(&sem);
            {

                 std::lock_guard<std::mutex> g(mtx);
             
                std::string msg = q.front();
                q.pop();
              
                return msg;
            }
        }
