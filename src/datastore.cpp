
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
#include "Value.h"
#include "StringListValue.h"
#include "NumberListValue.h"
#include "chrono"
#include "Server.h"
Storage* Storage::instance = nullptr;

Storage* Storage::getInstance() {
            if(instance == nullptr){
                Storage::instance = new Storage();
            }
            return Storage::instance;
}

Value** Storage::get(const std::string& key){
    Value** val = this->table->get(key);
    // if(val == NULL) return val;
    // std::chrono::steady_clock::time_point* exp = this->expires->get(key);
    // if(exp != NULL){
    //     if(std::chrono::steady_clock::now() >= *exp){
    //         this->table->remove(key);

    //         if(this->expires->remove(key)){
    //             this->writeExpiry();
    //         }
    //         this->writeData();
    //         return NULL;
    //     }
    // }
     return val; 
   
}

int Storage::remove(const std::string& key){
    int n = this->table->remove(key);
     if(this->expires->remove(key)){
            this->writeExpiry();
        }
    this->writeData();
    return n;
}
   void Storage::setTTL(const std::string& key, int ttl) {
        if(ttl >= 0){

            this->expires->set(key,std::chrono::steady_clock::now() + std::chrono::seconds(ttl));
            this->writeExpiry();
        }
        else {

           if( this->expires->remove(key)){
            this->writeExpiry();
           }
        }
   }
void Storage::set(const std::string& key, Value* val, int ttl){
    this->table->set(key,val);
    this->writeData();

    setTTL(key,ttl);
}

Storage::Storage(){
    
    table = nullptr;
    expires = nullptr;
// try to load from disk first
Config* config = Config::getInstance();
// if persistence is on then read try to read from file
const std::string& persistence = config->get("persistence");
isEnabled = persistence == "1";
if(isEnabled) {
    const std::string& file = config->get("data_file");
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
if(!expires)
expires = new Hashtable<std::string,std::chrono::steady_clock::time_point>(16);

}


Storage::~Storage() {

    if(table){
        delete table;
        table = nullptr;
    }
    if(expires){
        delete expires;
        expires = nullptr;
    }
    
}

void Storage::deleteInstance() {
        if(Storage::instance){

            delete Storage::instance;
            Storage::instance = nullptr;
        }
}

void Storage::writeExpiry(){
    Config* config = Config::getInstance();
    
    if(isEnabled) {
        const std::string& file = config->get("data_file");
        std::filesystem::path filepath(file);
        if(filepath.has_parent_path()){
            std::filesystem::create_directories(filepath.parent_path());
        }
        std::ofstream out(filepath, std::ios::binary | std::ios::trunc);
        this->saveExpiry(out);
        out.close();
    }
}
void Storage::writeData(){
    Config* config = Config::getInstance();
    
    if(isEnabled) {
        const std::string& file = config->get("data_file");
        std::filesystem::path filepath(file);
        if(filepath.has_parent_path()){
            std::filesystem::create_directories(filepath.parent_path());
        }
        std::ofstream out(filepath, std::ios::binary | std::ios::trunc);
        this->saveData(out);
        out.close();
    }
}

void Storage::saveExpiry(std::ostream& out){
    if(!out){
            std::cerr << "Failed to open binary file.\n";
            return;
        }
    //write expires

   auto expires_arr = expires->getArr();
   int capacity = expires->getCapacity();
   out.write(reinterpret_cast<char*>(&capacity),sizeof(int));
   // write capacity number of lists
   for(int i = 0; i < capacity; i++){
        auto *head = expires_arr[i].getHead();
        // write len of linked list
        int len = expires_arr[i].getSize();
        out.write(reinterpret_cast<char *>(&len), sizeof(int));
        while(head != NULL){
             // write key then value
      // write size [value] size [value]
        const std::string& key = head->getKey();
        size_t key_len = key.size();
        out.write(reinterpret_cast<char*>(&key_len), sizeof(size_t));
        out.write(key.c_str(), key_len);
             std::chrono::steady_clock::time_point val = head->getValue();
            size_t s = sizeof(val);
            out.write(reinterpret_cast<char*>(&s), sizeof(size_t));
            out.write(reinterpret_cast<char*>(&val),s);
            head = head->getNext();
        }

   }
}

void Storage::expiry_ttl() {
    int i = 0;
  
    while(!Server::should_exit){
 
        std::this_thread::sleep_for(std::chrono::seconds(1));
        i++;
        if(expires->getSize() > 0){
            for(int i = 0; i < expires->getCapacity();i++){
               SinglyLinkedList<std::string,std::chrono::steady_clock::time_point>& l = expires->getArr()[i];
               auto head = l.getHead();
               while(head != NULL){
                 auto next = head->getNext();
                    if(std::chrono::steady_clock::now() >= head->getValue()){
                            table->remove(head->getKey());
                            if(expires->remove(head->getKey())){
                               writeExpiry();
                            }
                            writeData();
                        }
                        head = next;
                    }
               }

               
            
        }
    }
};


void Storage::saveData(std::ostream& out) {

        if(!out){
            std::cerr << "Failed to open binary file.\n";
            return;
        }
      // write table  
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
    // read table
int capacity;
in.read(reinterpret_cast<char *>(&capacity), sizeof(int));
if(table) {
    delete table;
    table = nullptr;
}
table = new Hashtable<std::string, Value*>(capacity);
//SinglyLinkedList<std::string,Value*>* arr = table->getArr();

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
    int16_t tag;
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

    } else if(tag ==  0x12) {
        val = new StringListValue();
    } else if(tag == 0x10){
        val = new NumberListValue<int64_t>();
    } else if(tag == 0x11){
        val = new NumberListValue<double>();
    }
    if(val){
    val->load(in);
    }
    table->set(key, val);
        j++;
    }
}

// read expiry
in.read(reinterpret_cast<char *>(&capacity), sizeof(int));
if(expires) {
    delete expires;
    expires = nullptr;
}
expires = new Hashtable<std::string,std::chrono::steady_clock::time_point>(capacity);

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
    size_t size_of_time;
    in.read(reinterpret_cast<char *>(&size_of_time),sizeof(size_of_time));

    std::chrono::steady_clock::time_point val;
    in.read(reinterpret_cast<char *>(&val),size_of_time);
    expires->set(key, val);
        j++;
    }
}



}
