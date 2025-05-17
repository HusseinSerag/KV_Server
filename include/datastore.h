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
    public:
    Hashtable<std::string, Value*>* table;
    static Storage* getInstance();
    static void deleteInstance();
    void write();

};

