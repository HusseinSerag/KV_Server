

#include <iostream>
#include "helpers.h"
#include <string>
#include <deque>
#include <cstddef>
#include "xxhash.h"
#include <shared_mutex>
#include <mutex>


template <typename K, typename V>
const float Hashtable<K,V>::LOAD_FACTOR_THRESHOLD = 0.70f;


template <typename K, typename V>
int Hashtable<K, V>::getLockIndex(const K& key) {
    return hash(key.c_str(), key.size()) % NUM_LOCKS;
}

template <typename K, typename V>
Hashtable<K, V>::Hashtable(int capacity) {
    this->capacity = capacity > 0 ? Helper::nextPower2((size_t)capacity) : 16;
    arr = new SinglyLinkedList<K, V>[this->capacity]();
    size = 0;
}

template <typename K, typename V>
Hashtable<K, V>::~Hashtable() {
    for (int i = 0; i < capacity; ++i) {
    SinglyLinkedList<K, V>& list = arr[i];
    Node<K, V>* current = list.getHead();
    while (current) {
        Node<K, V>* next = current->getNext();
        if constexpr (std::is_pointer_v<V>) {
            delete current->getValue();
        }           
        current = next;
    }
}
delete[] arr;  
}

template<typename K, typename V>
 unsigned int Hashtable<K,V>::DJBHash(const char* str, unsigned int length){
        unsigned int hash = 5381;
        unsigned int i = 0;
        for(i = 0; i < length; ++str, ++i)
        {
            hash = ((hash << 5) + hash) + (*str);
        }
        return hash;
    }

    template<typename K,typename V>
    unsigned int Hashtable<K,V>::hash(const char* str, unsigned int length){
        return XXH3_64bits(str, length);

    }

template<typename K, typename V>
void Hashtable<K,V>::unsafe_set(Node<K,V> * n) {
    
    int ind = hash(n->getKey().c_str(), n->getKey().size()) & (this->capacity - 1);
    arr[ind].appendToFrontWithoutCreating(n);
    size++;
}
template<typename K, typename V>
void Hashtable<K,V>::resize() {
      std::unique_lock resizeGuard(resizeMutex);

        std::array<std::unique_lock<std::shared_mutex>,NUM_LOCKS> lockGuards;
     for(int i = 0; i < NUM_LOCKS; i++){
         lockGuards[i] = std::unique_lock(locks[i]);
     }
    
    SinglyLinkedList<K, V>* oldArr = arr;
    int oldCap = capacity;
    capacity *= 2;
    arr = new SinglyLinkedList<K, V>[capacity]();
    size = 0;
    for(int i = 0; i < oldCap ; i++){
        Node<K,V>* p = oldArr[i].getHead();
        Node<K,V>* prev = NULL;
        while(p != NULL){
            prev = p->getNext();
        this->unsafe_set(p);
        p = prev;
        }
        oldArr[i].setHead(NULL);
    }
    delete[] oldArr;


}
template <typename K, typename V>
void Hashtable<K, V>::set(K key, V val) {
    bool shouldResize = false;
   {   
         std::shared_lock resizeGuard(resizeMutex);
         int lockIndex = getLockIndex(key);
         std::unique_lock lock(locks[lockIndex]);
    

        int ind = hash(key.c_str(), key.size()) & (this->capacity - 1); 
        SinglyLinkedList<K, V>& l = arr[ind];

        Node<K, V>* node = l.search(key);

        if (node != NULL) {
            if constexpr (std::is_pointer_v<V>) {
                delete node->getValue();
            } 
            node->setValue(val);
        } else {
            l.appendToFront(key, val);
            size++;
            if((float) size / capacity > LOAD_FACTOR_THRESHOLD) {
                shouldResize = true;
            }
        }
    }
    if(shouldResize){
        resize();
    }
}

template <typename K, typename V>
V* Hashtable<K, V>::get(K key) {
     std::shared_lock resizeGuard(resizeMutex);
     int lockIndex = getLockIndex(key);
     std::shared_lock lock(locks[lockIndex]);
    int ind =  hash(key.c_str(), key.size()) & (this->capacity - 1);
    SinglyLinkedList<K, V>& l = arr[ind];
    Node<K, V>* node = l.search(key);
    if (node != NULL) {
        return &(node->getValue());
    }
    return NULL;
}

template <typename K, typename V>
int Hashtable<K, V>::remove(K key) {
    int ind =  hash(key.c_str(), key.size()) & (this->capacity - 1);
     std::shared_lock resizeGuard(resizeMutex);
     int lockIndex = getLockIndex(key);
     std::unique_lock lock(locks[lockIndex]);
    SinglyLinkedList<K, V>& l = arr[ind];
    int res = l.deleteBySearch(key);
    if (res) size--;
    return res;
}

template <typename K, typename V>
std::deque<K> Hashtable<K,V>::keys() {
     std::shared_lock resizeGuard(resizeMutex);
     std::array<std::unique_lock<std::shared_mutex>,NUM_LOCKS> lockGuards;
     for(int i = 0; i < NUM_LOCKS; i++){
         lockGuards[i] = std::unique_lock(locks[i]);
     }
    
    std::deque<K> out;

    for(int i = 0; i < this->capacity; i++){
        SinglyLinkedList<K,V>& l = arr[i];
            for(Node<K,V>* p = l.getHead(); p != NULL; p = p->getNext()){
                out.push_back(p->getKey());
            }
    }
    return out;

}

template <typename K, typename V>
    unsigned int Hashtable<K,V>::getSize() {
        return this->size;
    }

    template <typename K, typename V>
    int Hashtable<K,V>::getCapacity() {
        return this->capacity;
    }

template<typename K, typename V>
std::string Hashtable<K,V>::table_toString() {
     std::shared_lock resizeGuard(resizeMutex);
 std::array<std::unique_lock<std::shared_mutex>,NUM_LOCKS> lockGuards;
     for(int i = 0; i < NUM_LOCKS; i++){
         lockGuards[i] = std::unique_lock(locks[i]);
     }
    std::string out = "\n";
    for(int i = 0; i < this->capacity; i++){
        SinglyLinkedList<K,V>& l = arr[i];
            for(Node<K,V>* p = l.getHead(); p != NULL; p = p->getNext()){
                std::string msg;
                if constexpr (std::is_pointer_v<V>) {
                    msg = p->getValue()->toString();
                } else {
                    std::to_string(p->getValue());
                }
                out += "[" + p->getKey() + "]:" + msg + "\n";
            }
    }
    return out;

}

template<typename K, typename V>
SinglyLinkedList<K, V>* Hashtable<K,V>::getArr() {
    return arr;
}

