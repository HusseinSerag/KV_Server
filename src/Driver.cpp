#include "Server.h"
#include "datastore.h"
#include "config.h"
#include <iostream>
#include <string>
#include <exception>
#include "Logger.h"
#include <csignal>
#include <cstdlib>
#include "thread_gaurd.h"
#include "ConcurrentQueue.h"
#include "command_handler.h"



void cleanup() {

    Storage::deleteInstance();
    Config::deleteInstance();
    Logger::close();
}
void handle_sig(int signal) {
        Server::should_exit = true;
}
  

void handleLogging(ConcurrentQueue* queue) {
    Logger::open();

    while(true) {
        auto msg  = queue->consume();
        Logger::log(msg);
    }

}

int main(int argc, char* argv[]) {
    std::signal(SIGINT, handle_sig);
    std::signal(SIGABRT,handle_sig);
    Server server;
    Config* config = Config::getInstance();
   if(!config->setup(argv + 1,  argc - 1)){
       std::cerr << config->getErrors() << std::endl;
       return -1;
   }
   
   server.configure(std::stoi(config->get("port")));
     ConcurrentQueue* queue = new ConcurrentQueue();
   Storage* storage = Storage::getInstance();
   
     std::thread loggingThread(handleLogging, queue);
     
    CommandHandler::setQueue(queue);
     
    std::thread thr(&Storage::expiry_ttl, storage);
    thread_gaurd gaurd(thr, 0);
     thread_gaurd g(loggingThread,0);
      
   
    server.start();
    cleanup();
    delete queue;

    return 0;
}
