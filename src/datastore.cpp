
#include "datastore.h"
#include <map>
#include <iostream>
#include <string>
#include "hashtable.h"
#include "Value.h"
#include "linked_list.h"
#include <fstream>
#include "NumberValue.h"
#include "StringValue.h"
#include "config.h"
#include <filesystem>
#include "Logger.h"
Storage* Storage::instance = nullptr;

    Storage* Storage::getInstance() {
            if(instance == nullptr){
                Storage::instance = new Storage();
            }
            return Storage::instance;
        }

Storage::Storage(){

    table = nullptr;
// try to load from disk first
Config* config = Config::getInstance();
// if persistence is on then read try to read from file
int persistence = std::stoi(config->get("persistence"));
if(persistence) {

    std::string file = config->get("data_file");
    std::ifstream in(file,std::ios::binary);
    if(in){
        
        this->load(in);
        in.close();
    } else {
        std::string logMsg = "Opening file: " + file + " on program startup failed. Probably the file doesn't exist yet!";
        Logger::log(logMsg);
        
    }
}

if(!table)
this->table = new Hashtable<std::string, Value*>(16);

}


Storage::~Storage() {

    if(table){
        delete table;
        table = nullptr;
    }
}

void Storage::deleteInstance() {
        if(Storage::instance){

            delete Storage::instance;
            Storage::instance = nullptr;
        }
}

void Storage::write(){
    Config* config = Config::getInstance();
    int persistence = std::stoi(config->get("persistence"));
    if(persistence) {
        std::string file = config->get("data_file");
        std::filesystem::path filepath(file);
        if(filepath.has_parent_path()){
            std::filesystem::create_directories(filepath.parent_path());
        }
        std::ofstream out(filepath, std::ios::binary | std::ios::trunc);
        this->save(out);
        out.close();
    }
}
void Storage::save(std::ostream& out) {

        if(!out){
            std::cerr << "Failed to open binary file.\n";
            return;
        }
   SinglyLinkedList<std::string, Value*>* arr = table->getArr();
   // write capacity of array
   int capacity = table->getCapacity();
   out.write(reinterpret_cast<char*>(&capacity),sizeof(int));
   // write capacity number of lists
   for(int i = 0; i < capacity; i++){
        Node<std::string,Value*> *head = arr[i].getHead();
        // write len of linked list
        int len = arr[i].getSize();
        out.write(reinterpret_cast<char *>(&len), sizeof(int));
        while(head != NULL){
             // write key then value
      // write size [value] size [value]
        const std::string& key = head->getKey();
        size_t key_len = key.size();
        out.write(reinterpret_cast<char*>(&key_len), sizeof(size_t));
        out.write(key.c_str(), key_len);

            Value* val = head->getValue();
            val->save(out);

            head = head->getNext();
        }

   }

}

void Storage::load(std::istream& in) {
int capacity;
in.read(reinterpret_cast<char *>(&capacity), sizeof(int));
if(table) {
    delete table;
    table = nullptr;
}
table = new Hashtable<std::string, Value*>(capacity);
SinglyLinkedList<std::string,Value*>* arr = table->getArr();

for(int i = 0; i < capacity; i++){
    int ll_len;
    in.read(reinterpret_cast<char *>(&ll_len),sizeof(int));
    int j = 0;
    while(j < ll_len){
        // read key and value and insert
    size_t key_len;
    in.read(reinterpret_cast<char *>(&key_len),sizeof(size_t));

    std::string key(key_len, '\0');
    in.read(&key[0],key_len);

    // read value here based on tag
    uint16_t tag;
    in.read(reinterpret_cast<char*>(&tag),sizeof(tag));
    Value* val = NULL;
    if(tag == 0x01){
        //
         val = new NumberValue<double>(0);

    } else if(tag == 0x00){
        val = new NumberValue<int64_t>(0);
    }
    else if(tag == 0x02) {

        val = new StringValue("");

    }
    if(val){
    val->load(in);
    }
    table->set(key, val);
        j++;
    }
}

}
