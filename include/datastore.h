#pragma once
#include <map>
#include <string>
#include "hashtable.h"
#include "Value.h"
#include "Serializable.h"
#include "chrono"


// implement singeleton pattern for storage
class Storage: public Serializable {
    Storage();
    ~Storage();
    static Storage* instance;
    void load(std::istream &in);
    void save(std::ostream& out);
    bool isEnabled;
    Hashtable<std::string,std::chrono::steady_clock::time_point>* expires;
    public:
    Hashtable<std::string, Value*>* table;
    static Storage* getInstance();
    static void deleteInstance();
    void write();
    Value** get(const std::string& key);
    int remove(const std::string& key);
    void set(const std::string& key, Value* val, int ttl = -1);
    void setTTL(const std::string& key, int ttl);

};

