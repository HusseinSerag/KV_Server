#pragma once
#include "Value.h"
#include "RB-Tree.h"
#include "StringValue.h"

class ListValue: public Value {
      public:

      template <typename T>
   static  void insert(T val, RedBlackTree<T>* value);
    template <typename T>
    static void updateAtIndex(T val, int ind,RedBlackTree<T>* value);
    template <typename T>
    static void removeAtIndex(int ind,RedBlackTree<T>* value);

    template <typename T>
    static T getAtIndex(int ind,RedBlackTree<T>* value);
    
   void save(std::ostream& out);
    void load(std::istream& in);
 

};



#include "ListValue.tpp"