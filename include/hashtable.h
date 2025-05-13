
#pragma once
#include "linked_list.h"
#include <deque>
#include <string>
template <typename K, typename V>
class Hashtable {
private:
    SinglyLinkedList<K, V>* arr;
    int capacity;
    int size;
    const static float LOAD_FACTOR_THRESHOLD;
    void resize();

public:
    Hashtable(int capacity = 16);
    ~Hashtable();

    void set(K key, V val);
    V get(K key);
    int remove(K key);
    std::deque<K> keys();
    int getSize();
    int getCapacity();
    std::string table_toString();
    SinglyLinkedList<K, V>* getArr();

};

#include "hashtable.tpp" // Include the template implementation


