#include "NumberListValue.h"
#include "Value.h"


    template <typename T>
 void NumberListValue<T>::init(std::true_type) { this->type = LIST_DOUBLE; }  // For double

 template <typename T>
    void NumberListValue<T>::init(std::false_type) { this->type = LIST_INT64; } // For int64_t


template <class T>
enum ValueType NumberListValue<T>::getType() {
    return this->type;
}
template <class T>
std::string NumberListValue<T>::toString() const {
   return value->toString();
}

template <class T>
int NumberListValue<T>::length() const {
    return value->getSize();
}
template <class T>
Value* NumberListValue<T>::clone() const {
    return new NumberListValue<T>();
} 

template <class T>
RedBlackTree<NumberValue<T> *>* NumberListValue<T>::getValue(){
    return this->value;
}

template <class T>
NumberListValue<T>::NumberListValue() {
    value  = new RedBlackTree<NumberValue<T> *>();
        static_assert(
        std::is_same<T, double>::value || std::is_same<T, int64_t>::value,
        "NumberListValue only supports double or int64_t"
    );
    // Call the appropriate overload
    init(std::is_same<T, double>{});
}


template <class T>
NumberListValue<T>::~NumberListValue() {
    if(value){
        delete value;
        value = nullptr;
    }
}

template <class T>
void write(NumberValue<T> * str, std::ostream& out ){
    str->save(out);
}
template <class T>
void NumberListValue<T>::save(std::ostream& out){
    int16_t tag = this->type == LIST_DOUBLE ? 0x11 : 0x10;
    out.write(reinterpret_cast<char*>(&tag),sizeof(tag));
    uint32_t len = (uint32_t)(this->getValue()->getSize());
    out.write(reinterpret_cast<char *>(&len),sizeof(len));
    // basically we write this amount of data to memory
    this->getValue()->inorder_cb(write<T>, out);
}
template <class T>
void NumberListValue<T>::load(std::istream& in){
        uint32_t len;
        in.read(reinterpret_cast<char *>(&len),sizeof(len));
        for(int i = 0; i < len ; i++){
            NumberValue<T>* val = new NumberValue<T>(0);
            uint16_t tag;
             in.read(reinterpret_cast<char *>(&tag),sizeof(tag));
              
                  val->load(in);
                  this->insert(val,this->getValue());
                
        }
     }