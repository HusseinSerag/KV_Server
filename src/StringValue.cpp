
#include "StringValue.h"
#include <string>
#include <fstream>

StringValue::StringValue(std::string val):value(val) {
    this->type = STRING;
}
enum ValueType StringValue::getType() {
    return this->type;
}
Value* StringValue::clone() const {

    return new StringValue(value);
}


int StringValue::length() {

    return this->value.length();

}

void StringValue::concat(std::string val){
    this->value += val;
}


std::string StringValue::toString() const {
    return value;
}

void StringValue::save(std::ostream& out) {
    int16_t tag = 0x02;
    out.write(reinterpret_cast<char*>(&tag),sizeof(tag));
    uint32_t len = (uint32_t)(value.size());
    out.write(reinterpret_cast<char *>(&len),sizeof(len));
    out.write(value.data(),len);

}

void StringValue::load(std::istream& in){

    uint32_t len;
    in.read(reinterpret_cast<char *>(&len),sizeof(len));
    value.resize(len);
    in.read(&value[0], len);


}

