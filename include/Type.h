
#pragma once

#include "Response.h"
#include "Connection.h"
#include "datastore.h"
#include <string>
#include <vector>
#include <cstdint>
#include <cstring>
#include "Value.h"
// commands keys values size capacity all

namespace Generic {

    enum GenericCommands {

        KEYS, SIZE, CAPACITY, TABLE, UNKNOWN, GET, DEL

    };
}

class Type {

protected:
    std::string command;
    std::string key;
    
    public:
    static enum Generic::GenericCommands _parseCommand(const std::string& command);

    virtual int8_t read(std::vector<std::string>& request, Response& res);
     static void write(std::vector<uint8_t>& output, Response& res);
     static void isKeyValid(const std::string& key);
    virtual void execute(Storage* storage, Response& res);
    virtual ~Type();
    std::string& getCommand();
    static void parse_set(std::vector<std::string>& cmd, NumberKind& t);
    

    Type();


};




