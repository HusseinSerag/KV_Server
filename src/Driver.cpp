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
void handle_sig(int signal) {
        Server::should_exit = true;
}

// SinglyLinkedList<K,V>& l = arr[i];
//             for(Node<K,V>* p = l.getHead(); p != NULL; p = p->getNext()){
//                 std::string msg;
//                 if constexpr (std::is_pointer_v<V>) {
//                     msg = p->getValue()->toString();
//                 } else {
//                     std::to_string(p->getValue());
//                 }
//                 out += "[" + p->getKey() + "]:" + msg + "\n";
//             }

void expiry_ttl(Storage* storage) {
    int i = 0;
  
    while(!Server::should_exit){
 
        std::this_thread::sleep_for(std::chrono::seconds(1));
        i++;
        if(storage->expires->getSize() > 0){
            for(int i = 0; i < storage->expires->getCapacity();i++){
               SinglyLinkedList<std::string,std::chrono::steady_clock::time_point>& l = storage->expires->getArr()[i];
               auto head = l.getHead();
               while(head != NULL){
                 auto next = head->getNext();
                    if(std::chrono::steady_clock::now() >= head->getValue()){
                            storage->table->remove(head->getKey());
                            if(storage->expires->remove(head->getKey())){
                               storage->writeExpiry();
                            }
                            storage->writeData();
                        }
                        head = next;
                    }
               }

               
            
        }
    }
};
    

int main(int argc, char* argv[]) {
    std::signal(SIGINT, handle_sig);
    std::signal(SIGABRT,handle_sig);
    Server server;
    Config* config = Config::getInstance();
   if(!config->setup(argv + 1,  argc - 1)){
       std::cerr << config->getErrors() << std::endl;
       return -1;
   }
    Logger::open();
    server.configure(std::stoi(config->get("port")));
    Storage::getInstance();
    std::thread thr(&expiry_ttl, Storage::getInstance() );
    server.start();
    cleanup();
    thr.join();
    
    return 0;




}
