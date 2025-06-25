
#pragma once
#include "Value.h"
#include <string>
#include "datastore.h"


template <typename T, typename V>
void Value::set(std::string& key, V value, int ttl) {
    Storage* storage = Storage::getInstance();
    Value* val = new T(value);
    storage->set(key, val,ttl);

}
