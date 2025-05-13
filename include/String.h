
#pragma once

#include "Type.h"

namespace Str {
 enum StringCommand {
    SET, DEL, GET, CONCAT, LENGTH, UNKNOWN,
};
}
class String : public Type {

    std::string value;
public:
    int8_t read(const uint8_t* start, const uint8_t* end, Response& res);
    void execute(Storage* storage, Response& res);
    static enum Str::StringCommand parseCommand(std::string command);

};


