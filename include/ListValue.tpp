#include "Value.h"
#include "RB-Tree.h"
#include "StringValue.h"
#include "ListValue.h"
#include "Value.h"
#include "cstring"

    
    template <typename T>
    void ListValue::insert(T val,RedBlackTree<T>* value){
        // if doesn't exist then just create
        value->insert(val);
    }
    template <class T>
    void ListValue::updateAtIndex(T val, int ind,RedBlackTree<T>* value) {
        value->updateByIndex(val, ind);
    }

    template <typename T>
    void ListValue::removeAtIndex(int ind,RedBlackTree<T>* value) {
        value->removeByIndex(ind);
    }
    template <typename T>
    T ListValue::getAtIndex(int ind,RedBlackTree<T>* value) {
       return value->rank(ind)->getVal();
    }
    
    
    // template <typename T>
    // Value* ListValue<T>::clone() const{}
    // template <typename T>
    // std::string ListValue<T>::toString() const {
    //     return "RB tree";
    // }
    
  




