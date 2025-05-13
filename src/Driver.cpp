#include "Server.h"
#include "datastore.h"
#include "config.h"
#include <iostream>
#include <string>
#include <exception>
#include "Logger.h"
int main(int argc, char* argv[]) {

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
    Storage::deleteInstance();
    Config::deleteInstance();
    Logger::close();
    return 0;




}
