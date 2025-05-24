#pragma once
#include "List.h"

class StringList: public List {
    std::vector<std::string> values;
    public:
    int8_t read(std::vector<std::string>& request, Response& res) override;
    void execute(Storage* storage, Response& res) override;
};