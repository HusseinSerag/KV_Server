#pragma once
#include "RB-Tree.h"
#include "StringValue.h"
#include "ListValue.h"
class StringListValue: public ListValue {

    RedBlackTree<StringValue *>* value;
    public:
    StringListValue();
     enum ValueType getType() override;
     std::string toString() const override;
     Value* clone() const override;
     RedBlackTree<StringValue *>* getValue();
     ~StringListValue();
     int length() const;
     void save(std::ostream& out);
     void load(std::istream& in);
    
};