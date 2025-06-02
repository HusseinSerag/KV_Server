#pragma once
#include "NumberValue.h"
#include <string>
#include <fstream>

    template <typename T>
 void NumberValue<T>::init(std::true_type) { this->type = DOUBLE; }  // For double

 template <typename T>
    void NumberValue<T>::init(std::false_type) { this->type = INT64; } // For int64_t
template <typename T>
NumberValue<T>::NumberValue(T val):value(val) {
    static_assert(
        std::is_same<T, double>::value || std::is_same<T, int64_t>::value,
        "NumberValue only supports double or int64_t"
    );

    // Call the appropriate overload
    init(std::is_same<T, double>{});
    
}

template <typename T>
enum ValueType NumberValue<T>::getType() {
    return this->type;
}
template <typename T>
void NumberValue<T>::increment(double n){
    this->value+=n;
}

template <typename T>
void NumberValue<T>::decrement(double n) {
    this->value-=n;
}

template <typename T>
void NumberValue<T>::multiply(T val){
    this->value *= val;
}

template <typename T>
Value* NumberValue<T>::clone() const {

    return new NumberValue<T>(value);
}

template <typename T>
void NumberValue<T>::divide(T val) {
    this->value /= val;
}

template <typename T>
std::string NumberValue<T>::toString() const {
    return std::to_string(this->value);
}

template <typename T>
void NumberValue<T>::save(std::ostream& out){
    int16_t tag = this->getType() == ValueType::DOUBLE ? 0x01 : 0x00;
    out.write(reinterpret_cast<char *>(&tag),sizeof(int16_t));
    size_t len = sizeof(T);
    out.write(reinterpret_cast<char *>(&len), sizeof(size_t));
    out.write(reinterpret_cast<char *>(&value), len);
}

template <typename T>
        void NumberValue<T>::load(std::istream& in){
            size_t len = 0;
            in.read(reinterpret_cast<char *>(&len), sizeof(size_t));
            in.read(reinterpret_cast<char *>(&value), len);
        }
