
#include "Value.h"
#include "string"
#include "datastore.h"
#include "Response.h"
#include "exception/NotFoundException.h"
#include <string>


void Value::del(std::string& key) {
    Storage* storage = Storage::getInstance();
    if (!storage->table->remove(key))
    throw NotFoundException();
}
Value* Value::get(std::string& key) {
Storage* storage = Storage::getInstance();
    Value** value = storage->get(key);
    if (value == NULL) throw NotFoundException();
    return *value;
}


 bool Value::isListType(ValueType& type) {
        switch(type) {
            case ValueType::LIST_DOUBLE:
            case ValueType::LIST_INT64:
            case ValueType::LIST_STRING:
            return true;
            default: return false;
        }
 }


