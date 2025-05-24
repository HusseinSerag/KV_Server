#pragma once
#include <iostream>
enum Color {RED, BLACK};

template <typename T>
class RedBlackTree;

template <typename T>
class RBNode {
    T val;
    RBNode* left;
    RBNode* right;
    RBNode* parent;
    enum Color color;
    int size;
    public:
    RBNode(T val, RBNode* left, RBNode* right, RBNode* parent, enum Color color, int size); 
    
     T  getVal();
     RBNode* getLeft();
     RBNode* getRight();
     RBNode* getParent() ;
     enum Color getColor();
     int getSize();
     void setVal(T val);
     void setLeft(RBNode* left);
     void setRight(RBNode* right);
     void setParent(RBNode* parent);
     void setColor(enum Color color);
     void switchColor();
     void setSize(int size);
     friend class RedBlackTree<T>;
};

template <typename T>
class RedBlackTree {

    RBNode<T>* root;
    RBNode<T>* NIL;
    int size;

   
    void rotate_left(RBNode<T> * node);
    void rotate_right(RBNode<T>* node) ;
    void insertion_violation(RBNode<T> * node);
    void delete_violation(RBNode<T>* node)  ;
    void _insert(RBNode<T>* n) ;
    RBNode<T>* _remove(RBNode<T>* toDelete);
    public:
      ~RedBlackTree();
      RedBlackTree();
      RBNode<T> * getRoot();
   int getSize();
   const RBNode<T>* getNil() ;
    void insert(T val);
    void remove(T val) ;
    void updateByIndex(T val, int ind);
    void removeByIndex(int ind);
    RBNode<T> *  minimum(RBNode<T> * root);
    RBNode<T> *  maximum(RBNode<T> * root);
    std::string toString() const;
    void inorder(RBNode<T> * root,std::string& out) const;
    RBNode<T> * search(T val);
   RBNode<T>*  rank(int val);
   void print(RBNode<T>* root, std::string indent, bool last);
   void deleteTree(RBNode<T>* root) ;
};





#include "RB-Tree.tpp"