#pragma once
#include <iostream>
#include <string>
template <typename K, typename V>
class SinglyLinkedList;


template <typename K, typename V>
class Node {

    Node* next;
    K key;
    V val;
    public:
        Node(K key, V val, Node* next);

        V& getValue();
        K getKey();
        Node* getNext();
        void setValue(V val);
        void setKey(K key);
        void setNext(Node* next);
        friend class SinglyLinkedList<K,V>;

};


template <typename K, typename V>
class SinglyLinkedList {
    Node<K,V>* head;
    int size;

    public:
        SinglyLinkedList();
        ~SinglyLinkedList();
        int getSize();
        Node<K,V>* getHead();
        void appendToFront(K key, V val);
        Node<K,V>* search(K key);
        int deleteBySearch(K key);
        void appendToFrontWithoutCreating(Node<K,V>* n);
        void setHead(Node<K,V>* n);

} ;


#include "linked_list.tpp"



