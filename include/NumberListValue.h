#pragma once
#include "RB-Tree.h"
#include "NumberValue.h"
#include "ListValue.h"

template <typename T>
class NumberListValue: public ListValue {

    RedBlackTree<NumberValue<T> *>* value;
    void init(std::true_type);
    void init(std::false_type);
    public:

    NumberListValue();
     enum ValueType getType() override;
     std::string toString() const override;
     Value* clone() const override;
     RedBlackTree<NumberValue<T> *>* getValue();
     ~NumberListValue();
     int length() const;
     void save(std::ostream& out);
     void load(std::istream& in);
    
};

#include "NumberListValue.tpp"