#include "StringListValue.h"
#include "Value.h"

enum ValueType StringListValue::getType() {
    return type;
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
    this->type = ValueType::LIST_STRING;
}


StringListValue::~StringListValue() {
    if(value){
        delete value;
        value = nullptr;
    }
}


void write(StringValue * str, std::ostream& out ){
    str->save(out);
}
void StringListValue::save(std::ostream& out){
    int16_t tag = 0x12;
    out.write(reinterpret_cast<char*>(&tag),sizeof(tag));
    uint32_t len = (uint32_t)(this->getValue()->getSize());
    out.write(reinterpret_cast<char *>(&len),sizeof(len));
    
    // basically we write this amount of data to memory
    this->getValue()->inorder_cb(write, out);
}
     void StringListValue::load(std::istream& in){
        uint32_t len;
        in.read(reinterpret_cast<char *>(&len),sizeof(len));
        for(int i = 0; i < len ; i++){
            StringValue* val = new StringValue("");
            uint16_t tag;
             in.read(reinterpret_cast<char *>(&tag),sizeof(tag));
              if(tag == 0x02){
                  val->load(in);
                  this->insert(val,this->getValue());
                }
        }
     }