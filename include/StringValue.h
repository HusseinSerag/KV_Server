

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
        enum ValueType getType();
    operator std::string() const{
        return value;
    }
    bool operator==(const StringValue& str){
        return str.value == value;
    }
    bool operator <(const StringValue& str){
        return value < str.value;
    }
     bool operator >(const StringValue& str){
        return value > str.value;
    }

};


