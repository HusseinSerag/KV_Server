#include "Server.h"
#include "datastore.h"
#include "config.h"
#include <iostream>
#include <string>
#include <exception>
#include "Logger.h"
#include <csignal>
#include <cstdlib>




void cleanup() {

    Storage::deleteInstance();
    Config::deleteInstance();
    Logger::close();
}
void handle_sigint(int signal) {


        Server::should_exit = true;
    


}
int main(int argc, char* argv[]) {
    std::signal(SIGINT, handle_sigint);
    Server server;
    Config* config = Config::getInstance();
   if(!config->setup(argv + 1,  argc - 1)){
       std::cerr << config->getErrors() << std::endl;
       return -1;
   }
    Logger::open();
    server.configure(std::stoi(config->get("port")));
    Storage::getInstance();
    server.start();
    cleanup();
    
    return 0;




}
