#include "Value.h"
#include "RB-Tree.h"
#include "StringValue.h"

class StringListValue: public Value {

    RedBlackTree<StringValue *> value;

    public:

    StringListValue(){


    }
    void insert(){

    }
    void updateAtIndex() {

    }
    void removeAtIndex() {
        
    }
    void getAtIndex() {}
    int size(){
        return value.getSize();
    }
    void concat(std::string val);
    Value* clone() const;
    std::string toString() const ;
    void save(std::ostream& out);
        void load(std::istream& in);

};


