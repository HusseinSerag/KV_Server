#pragma once
#include "List.h"
#include "Value.h"

class StringList: public List {
    std::vector<std::string> values;
    public:
    
    StringList(Value* val = NULL);
    int8_t read(std::vector<std::string>& request, Response& res) override;
    void execute(Storage* storage, Response& res) override;
};