#include "StringListValue.h"
#include "Value.h"

enum ValueType StringListValue::getType() {
    return ValueType::LIST_STRING;
}

std::string StringListValue::toString() const {
   return value->toString();
}

int StringListValue::length() const {
    return value->getSize();
}
Value* StringListValue::clone() const {
    return new StringListValue();
} 

RedBlackTree<StringValue *>* StringListValue::getValue(){
    return this->value;
}

StringListValue::StringListValue() {
    value  = new RedBlackTree<StringValue *>();
}


StringListValue::~StringListValue() {
    if(value){
        delete value;
        value = nullptr;
    }
}