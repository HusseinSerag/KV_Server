
#pragma once
#include "Type.h"
#include "helpers.h"
#include "Scalar.h"
enum NumberCommand {

    SET, MULT, INC, DEC,  DIVIDE, UNKNOWN

};

template <typename T>
class Number : public Scalar {
    double value;
public:

    int8_t read(std::vector<std::string> & command, Response& res) ;
   // void write(std::vector<uint8_t>& out_buffer, Response& res);
    void execute(Storage* storage, Response& res);
    static enum NumberCommand parseCommand(const std::string& command);
    Number(const std::string& val);
    Number();
    double getValue() const;
    Value* toValue();
    
};


#include "Number.tpp"
