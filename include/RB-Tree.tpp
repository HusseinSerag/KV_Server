#include <iostream>
#include "RB-Tree.h"


template <typename T>
class RedBlackTree;
  
    template <typename T>
    RBNode<T>::RBNode(T val, RBNode* left, RBNode* right, RBNode* parent, enum Color color, int size):val(val), left(left),right(right),parent(parent), color(color), size(size) {} 
    template <typename T>
     T  RBNode<T>::getVal() {
        return val;
     }
     template <typename T>
     RBNode<T>* RBNode<T>::getLeft() {
        return left;
     }
     template <typename T>
     RBNode<T>* RBNode<T>::getRight() {
        return right;
     }
     template <typename T>
     RBNode<T>* RBNode<T>::getParent() {
        return parent;
     }
     template <typename T>
     enum Color RBNode<T>::getColor() {
      return color;
     }
     template <typename T>
     int RBNode<T>::getSize() {
      return size;
     }
     template <typename T>
     void RBNode<T>::setVal(T val) {
        this->val = val;
     }
     template <typename T>
     void RBNode<T>::setLeft(RBNode* left){
      this->left = left;
     }
     template <typename T>
     void RBNode<T>::setRight(RBNode* right){
      this->right = right;
     }
     template <typename T>
     void RBNode<T>::setParent(RBNode* parent) {
      this->parent = parent;
     }
     template <typename T>
     void RBNode<T>::setColor(enum Color color){
      this->color = color;
     }
     template <typename T>
     void RBNode<T>::switchColor(){
      this->color = this->color == RED ? BLACK : RED;
     }
     template <typename T>
     void RBNode<T>::setSize(int size) {
      this->size = size;
     }

    






   template <typename T>
   void RedBlackTree<T>::rotate_left(RBNode<T> * node) {
      bool isLeft = node->parent->left == node;
      RBNode<T> * rh = node->right;
      node->right = rh->left;
      if(rh->left != NIL){
         rh->left->parent = node;
      }
      rh->parent = node->parent;
      
      if(node->parent == NIL){
         root = rh;
      }
       else if(isLeft)  
       rh->parent->left = rh;
      else 
      rh->parent->right = rh;

      node->parent = rh;
      rh->left = node;
      rh->size = node->size;
      node->size = node->left->size + node->right->size + 1;
   }
   template <typename T>
   void RedBlackTree<T>::rotate_right(RBNode<T>* node) {
    bool isLeft = node->parent->left == node;
    RBNode<T>* lf = node->left;

    node->left = lf->right;
    if (lf->right != NIL) {
        lf->right->parent = node;
    }

    lf->parent = node->parent;

    if (node->parent == NIL) {
        root = lf;
    } else if (isLeft) {
        lf->parent->left = lf;
    } else {
        lf->parent->right = lf;
    }

    node->parent = lf;
    lf->right = node;
    lf->size = node->size;
      node->size = node->left->size + node->right->size + 1;
    
}
template <typename T>
   void RedBlackTree<T>::insertion_violation(RBNode<T> * node) {
      RBNode<T> * parent = nullptr;
         RBNode<T> * grand_parent = nullptr;
         
         while(node != root && node->color == RED && node->parent->color == RED){
            parent = node->parent;
            grand_parent = parent->parent;
            
            if(grand_parent->left == parent){
               RBNode<T> * uncle = grand_parent->right;
               if(uncle != NIL && uncle->color == RED){
                  // recolor
                  parent->color = BLACK;
                  uncle->color = BLACK;
                  grand_parent->color = RED; 
                  node = grand_parent;
               } else {
                  if(node == parent->right){
                     rotate_left(parent);
                     node = parent;
                     parent = node->parent;
                  }
                  // rotate and recolor
                  rotate_right(grand_parent);
                  std::swap(parent->color, grand_parent->color);
                  node = parent;
               }
            } else if(grand_parent->right == parent) {
               RBNode<T> * uncle = grand_parent->left;
                if(uncle != NIL && uncle->color == RED){
                  // recolor
                  parent->color = BLACK;
                  uncle->color = BLACK;
                  grand_parent->color = RED; 
                  node = grand_parent;
               } else {
                  if(node == parent->left){
                     rotate_right(parent);
                     node = parent;
                     parent = node->parent;
                  }
                  rotate_left(grand_parent);
                  std::swap(parent->color, grand_parent->color);
                  node = parent;
               }
            }
         }
         root->color = BLACK;
   }
  template <typename T>
   void RedBlackTree<T>::delete_violation(RBNode<T>* node)   {
        while (node != root && node->color == BLACK) {
            if (node == node->parent->left) {
               RBNode<T>* sibling = node->parent->right;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotate_left(node->parent);
                    sibling = node->parent->right;
                }
                if ((sibling->left == NIL
                     || sibling->left->color == BLACK)
                    && (sibling->right == NIL
                        || sibling->right->color
                               == BLACK)) {
                    sibling->color = RED;
                    node = node->parent;
                }
                else {
                    if (sibling->right == NIL
                        || sibling->right->color == BLACK) {
                        if (sibling->left !=  NIL)
                            sibling->left->color = BLACK;
                        sibling->color = RED;
                        rotate_right(sibling);
                        sibling = node->parent->right;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->right !=  NIL)
                        sibling->right->color = BLACK;
                    rotate_left(node->parent);
                    node = root;
                }
            }
            else {
               RBNode<T>* sibling = node->parent->left;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotate_right(node->parent);
                    sibling = node->parent->left;
                }
                if ((sibling->left == NIL
                     || sibling->left->color == BLACK)
                    && (sibling->right == NIL
                        || sibling->right->color
                               == BLACK)) {
                    sibling->color = RED;
                    node = node->parent;
                }
                else {
                    if (sibling->left == NIL
                        || sibling->left->color == BLACK) {
                        if (sibling->right != NIL)
                            sibling->right->color = BLACK;
                        sibling->color = RED;
                        rotate_left(sibling);
                        sibling = node->parent->left;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->left != NIL)
                        sibling->left->color = BLACK;
                    rotate_right(node->parent);
                    node = root;
                }
            }
        }
        node->color = BLACK;
    }
    template <typename T>
    void RedBlackTree<T>::_insert(RBNode<T>* n) {
         
         
         if(root == NIL){
            n->color = BLACK;
            root = n;
            root->left = root->right = root->parent = NIL;
            size++;
            return;
         } 
         // find its place
         RBNode<T>* temp = root;
         RBNode<T>* parent = NIL;
         while(temp != NIL){
            // check
            parent = temp;
            temp->size++;
            if(n->val < temp->val){
               temp = temp->left;
            } else {
               temp = temp->right;
            }
         }
            
         
         // insert
         if(n->val < parent->val)
            parent->left = n;
         else 
            parent->right = n;
         // check for violations
         n->parent = parent;
         insertion_violation(n);
         size++; 
      }
   template <typename T>
       RBNode<T>* RedBlackTree<T>::_remove(RBNode<T>* toDelete){
         // search for value first
         
         if(toDelete == NIL){
            return toDelete;
         }
         enum Color toDeleteColor = toDelete->color;
         RBNode<T>* replacement = NIL;
         

         // perform normal BST delete
         if(toDelete->left == NIL && toDelete->right == NIL){
            // detach 
            if(toDelete == root){
               root = NIL;
            }
            else if(toDelete->parent ->left == toDelete){
               toDelete->parent ->left = NIL;
            } else {
               toDelete->parent ->right = NIL;
            }
            toDelete->left = toDelete->right = toDelete->parent = NIL;
         }  else if(toDelete->left != NIL && toDelete->right != NIL){
               // Step 1: Find max of left subtree (predecessor)
    RBNode<T>* max = maximum(toDelete->left);
    replacement = max;

    // Step 2: Save its child (only left possible)
    RBNode<T>* child = max->left;

    // Step 3: Replace max with its child if max is not directly toDelete->left
    if (max->parent != toDelete) {
        if (max->parent->left == max)
            max->parent->left = child;
        else
            max->parent->right = child;

        if (child != NIL)
            child->parent = max->parent;

        // Move toDelete's left subtree under max
        max->left = toDelete->left;
        if (toDelete->left != NIL)
            toDelete->left->parent = max;
    }

    // Step 4: Always move toDelete's right subtree under max
    max->right = toDelete->right;
    if (toDelete->right != NIL)
        toDelete->right->parent = max;

    // Step 5: Rewire max's parent
    max->parent = toDelete->parent;

    // Step 6: Update toDelete's parent pointer
    if (toDelete == root) {
        root = max;
    } else if (toDelete->parent->left == toDelete) {
        toDelete->parent->left = max;
    } else {
        toDelete->parent->right = max;
    }

    // Step 7: Detach toDelete completely
    toDelete->left = toDelete->right = toDelete->parent = NIL;

         } else {
            // only one child exist swap child 
            RBNode<T>* parent = toDelete->parent;
            RBNode<T>* child = toDelete->left != NIL ? toDelete->left : toDelete->right;
            if (toDelete == root) {
               root = child;
               if (child != NIL)
                     child->parent = NIL;
            } else {
               if (parent->left == toDelete)
                     parent->left = child;
               else
                     parent->right = child;
               if (child != NIL)
                     child->parent = parent;
            }

            // Detach
            replacement = child;
            toDelete->left = toDelete->right = toDelete->parent = NIL;
         }
         
         if(toDeleteColor == RED || replacement->color == RED){
            replacement->color = BLACK;
         } else if(toDeleteColor == BLACK && replacement->color == BLACK){
            // double black problem
            delete_violation(replacement);
         }
         size--;
         return toDelete;

         
      }
   
  template <typename T>
      RedBlackTree<T>::~RedBlackTree() {
         deleteTree(root);
         delete NIL;
      }
      template <typename T>
      RedBlackTree<T>::RedBlackTree() {
         NIL = new RBNode<T>(T(),nullptr,nullptr,nullptr, BLACK, 0);
         root = NIL;
         size = 0;
      }template <typename T>
      void RedBlackTree<T>::insert(T val){
         RBNode<T>* n = new RBNode<T>(val, NIL,NIL, NIL, RED, 1);
         _insert(n);
      }template <typename T>
      void RedBlackTree<T>::remove(T val) {
         RBNode<T>* toDelete = search(val);
         RBNode<T>* n = _remove(toDelete);
         if(n)
         delete n;
      }template <typename T>
      void RedBlackTree<T>::updateByIndex(T val, int ind){
        RBNode<T>* node = rank(ind);
        if(node != NIL){
         node = _remove(node);
         node->val = val;
         _insert(node);
        }
         
      }template <typename T>
      void RedBlackTree<T>::removeByIndex(int ind){
         RBNode<T>* node = rank(ind);
        if(node != NIL){
         node = _remove(node);
        }

      }
      template <typename T>
      RBNode<T> *  RedBlackTree<T>::minimum(RBNode<T> * root) {
         RBNode<T> * temp = root;
         while(temp->left != NIL){
         temp = temp->left;
      }
      return temp;
      }template <typename T>
       RBNode<T> *  RedBlackTree<T>::maximum(RBNode<T> * root) {
         RBNode<T> * temp = root;
         while(temp->right != NIL){
         temp = temp->right;
      }
      return temp;

   }
   template <typename T>
       std::string  RedBlackTree<T>::toString() const {
         std::string output;
         output += "[";
         inorder(root,output);
         output.pop_back();
         output += "]";
         return output;
       }
      template <typename T>
        void RedBlackTree<T>::inorder( RBNode<T> * root, std::string& s) const{
      if(root == NIL){
         return;
      }
      inorder(root->left,s);
      //  cb(root->val);
      std::string v = *(root->getVal());
       s+= " " + v + "," ;
      inorder(root->right,s);
   }
template <typename T>
   RBNode<T> * RedBlackTree<T>::getRoot() {
      return root;
   }template <typename T>
   int RedBlackTree<T>::getSize() {
      return size;
   }
template <typename T>
    RBNode<T> * RedBlackTree<T>::search(T val) {
       RBNode<T> * temp = root;
       while(temp != NIL){
         if(val == temp->val){
           return temp;
         } else if(val > temp->val){
            temp = temp->right;
         } else if(val < temp->val){
            temp = temp->left;
         }
      }
      return NIL;
    }
template <typename T>
    const RBNode<T>* RedBlackTree<T>::getNil() {
      return NIL;
    }template <typename T>
   RBNode<T>*  RedBlackTree<T>::rank(int val){
      RBNode<T>* temp = root;

    while (temp != NIL) {
        int leftSize = temp->left->size;

        if (val == leftSize + 1) {
            return temp;
        } else if (val <= leftSize) {
            temp = temp->left;
        } else {
            val = val - (leftSize + 1);
            temp = temp->right;
        }
    }

    return NIL;  // Rank out of bounds
      
   }
template <typename T>
   void RedBlackTree<T>::print(RBNode<T>* root, std::string indent, bool last){
      if(root != NIL){
         std::cout << indent;
         if(last) {
            std::cout << "R----";
            indent += "   ";
         } else {
            std::cout << "L----";
            indent += "|  ";
         }
         std::string sColor = (root->color == RED)?"RED":"BLACK";
         std::cout << root->val << "(" << sColor << ")" << std::endl;
         print(root->left,indent,false);
         print(root->right,indent,true);
      }
   }

   template <typename T>
   void RedBlackTree<T>::deleteTree(RBNode<T>* root) {
      // post order to delete
      if(root == NIL) return;
      deleteTree(root->left);
      deleteTree(root->right);
      delete root->val;
      delete root;
   }



