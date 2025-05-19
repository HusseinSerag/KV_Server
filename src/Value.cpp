
#include "Value.h"
#include "string"
#include "datastore.h"
#include "Response.h"
#include "exception/exception.h"
#include <string>


void Value::del(std::string& key) {
    Storage* storage = Storage::getInstance();
    if (!storage->table->remove(key))
    throw BaseException("not found", NOT_FOUND);
}
Value* Value::get(std::string& key) {
Storage* storage = Storage::getInstance();
    Value* value = storage->table->get(key);
    if (value == NULL) throw BaseException("not found", NOT_FOUND);
    return value;
}



