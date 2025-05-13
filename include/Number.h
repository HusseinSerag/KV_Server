
#pragma once
#include "Type.h"
#include "helpers.h"

enum NumberCommand {

    SET, DEL, MULT, INC, DEC, GET, DIVIDE, UNKNOWN

};

template <typename T>
class Number : public Type {
    T value;
public:

    int8_t read(const uint8_t* start, const uint8_t* end, Response& res) ;
   // void write(std::vector<uint8_t>& out_buffer, Response& res);
    void execute(Storage* storage, Response& res);
    static enum NumberCommand parseCommand(std::string command);
};


#include "Number.tpp"
