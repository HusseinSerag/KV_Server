
#pragma once
#include "linked_list.h"
#include <deque>
#include <string>
#include <shared_mutex>
template <typename K, typename V>
class Hashtable {
private:
    SinglyLinkedList<K, V>* arr;
    int capacity;
    unsigned int size;
    const static int NUM_LOCKS = 64;
    const static float LOAD_FACTOR_THRESHOLD;
    mutable std::shared_mutex locks[NUM_LOCKS];
    mutable std::shared_mutex resizeMutex;

    int getLockIndex(const K& key);
    void resize();
    void unsafe_set(Node<K,V> * n);
    // Algorithm produced by Professor Daniel J. Bernstein
    unsigned int DJBHash(const char* str, unsigned int length);
    unsigned hash(const char* str, unsigned int length);


public:
    Hashtable(int capacity = 16);
    ~Hashtable();

    void set(K key, V val);
    V* get(K key);
    int remove(K key);
    std::deque<K> keys();
    unsigned int getSize();
    int getCapacity();
    std::string table_toString();
    SinglyLinkedList<K, V>* getArr();
    
   

};

#include "hashtable.tpp" // Include the template implementation


