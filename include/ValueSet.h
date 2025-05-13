
#pragma once
#include "Value.h"
#include <string>
#include "datastore.h"


template <typename T, typename V>
void Value::set(std::string& key, V value) {
    Storage* storage = Storage::getInstance();
    Value* val = new T(value);
    storage->table->set(key, val);

}
