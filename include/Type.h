
#pragma once

#include "Response.h"
#include "Connection.h"
#include "datastore.h"
#include <string>
#include <vector>
#include <cstdint>
#include <cstring>
// commands keys values size capacity all

namespace Generic {

    enum GenericCommands {

        KEYS, SIZE, CAPACITY, TABLE, UNKNOWN

    };
}

class Type {

protected:
    std::string command;
    std::string key;
    static enum Generic::GenericCommands _parseCommand(std::string command);

public:

    virtual int8_t read(const uint8_t* start, const uint8_t* end, Response& res);
    virtual void write(std::vector<uint8_t>& output, Response& res);
    virtual void execute(Storage* storage, Response& res);
    virtual ~Type();
    Type();


};




