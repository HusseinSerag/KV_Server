
#pragma once

#include "Value.h"

template <typename T>
class NumberValue: public Value {

    T value;
    uint16_t tag;
    void init(std::true_type);
    void init(std::false_type);
    public:

    explicit NumberValue(T val);
    void increment();
    void decrement();
    void divide(T val);
    void multiply(T val);
    Value* clone() const;
    std::string toString() const ;
     void save(std::ostream& out);
        void load(std::istream& in);

};


#include "NumberValue.tpp"
