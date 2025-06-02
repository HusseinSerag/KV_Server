
#pragma once

#include "Value.h"

template <typename T>
class NumberValue: public Value {

    T value;
    void init(std::true_type);
    void init(std::false_type);
    public:

    explicit NumberValue(T val);
    void increment(double n);
    void decrement(double n);
    void divide(T val);
    void multiply(T val);
    Value* clone() const;
    std::string toString() const ;
    void save(std::ostream& out);
    void load(std::istream& in);
    enum ValueType getType();
     operator std::string() const{
        return std::to_string(value);
    }
    bool operator==(const NumberValue<T>& str){
        return str.value == value;
    }
    bool operator<(const NumberValue<T>& str){
        return value < str.value;
    }
     bool operator>(const NumberValue<T>& str){
        return value > str.value;
    }
};

#include "NumberValue.tpp"
