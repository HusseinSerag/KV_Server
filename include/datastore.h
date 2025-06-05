#pragma once
#include <map>
#include <string>
#include "hashtable.h"
#include "Value.h"
#include "Serializable.h"


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

};

