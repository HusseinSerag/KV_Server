

#pragma once
#include "Value.h"


class StringValue: public Value {

    std::string value;

    public:

    explicit StringValue(std::string val);
    int length();
    void concat(std::string val);
    Value* clone() const;
    std::string toString() const ;
    void save(std::ostream& out);
        void load(std::istream& in);

};


