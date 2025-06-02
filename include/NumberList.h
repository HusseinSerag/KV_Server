#pragma once
#include "List.h"
#include "Value.h"
#include "datastore.h"
#include "Response.h"

template <typename T>
class NumberList: public List {
    std::vector<double> values;
    public:
    NumberList(Value* val = NULL);
    int8_t read(std::vector<std::string>& request, Response& res) override;
    void execute(Storage* storage, Response& res) override;
};

#include "NumberList.tpp"