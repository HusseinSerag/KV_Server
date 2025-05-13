

#include <iostream>
#include "helpers.h"
#include <string>
#include <deque>

template <typename K, typename V>
const float Hashtable<K,V>::LOAD_FACTOR_THRESHOLD = 0.75f;
template <typename K, typename V>
Hashtable<K, V>::Hashtable(int capacity) {
    this->capacity = capacity > 0 ? capacity : 16;
    arr = new SinglyLinkedList<K, V>[this->capacity]();
    size = 0;
}

template <typename K, typename V>
Hashtable<K, V>::~Hashtable() {
    delete[] arr;
}
template<typename K, typename V>
void Hashtable<K,V>::resize() {

    SinglyLinkedList<K, V>* oldArr = arr;
    int oldCap = capacity;
    capacity *= 2;
    arr = new SinglyLinkedList<K, V>[capacity]();
    size = 0;
    for(int i = 0; i < oldCap ; i++){
        Node<K,V>* p = oldArr[i].getHead();
        while(p != NULL){
        this->set(p->getKey(),p->getValue());
        p = p->getNext();
        }
    }
    delete[] oldArr;


}
template <typename K, typename V>
void Hashtable<K, V>::set(K key, V val) {
    int ind = Helper::DJBHash(key.c_str(), key.size()) % this->capacity; // Replace this with proper hashing in future
    SinglyLinkedList<K, V>& l = arr[ind];

    Node<K, V>* node = l.search(key);

    if (node != NULL) {
        delete node->getValue();
        node->setValue(val);
    } else {
        l.appendToFront(key, val);
        size++;
        if((float) size / capacity > LOAD_FACTOR_THRESHOLD) {
            resize();
        }
    }
}

template <typename K, typename V>
V Hashtable<K, V>::get(K key) {
    int ind =  Helper::DJBHash(key.c_str(), key.size()) % this->capacity;
    SinglyLinkedList<K, V>& l = arr[ind];
    Node<K, V>* node = l.search(key);

    if (node != NULL) {
        return node->getValue();
    }
    return NULL;
}

template <typename K, typename V>
int Hashtable<K, V>::remove(K key) {
    int ind =  Helper::DJBHash(key.c_str(), key.size()) % this->capacity;
    SinglyLinkedList<K, V>& l = arr[ind];
    int res = l.deleteBySearch(key);
    if (res) size--;
    return res;
}

template <typename K, typename V>
std::deque<K> Hashtable<K,V>::keys() {
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
    int Hashtable<K,V>::getSize() {
        return this->size;
    }

    template <typename K, typename V>
    int Hashtable<K,V>::getCapacity() {
        return this->capacity;
    }

template<typename K, typename V>
std::string Hashtable<K,V>::table_toString() {

    std::string out = "\n";
    for(int i = 0; i < this->capacity; i++){
        SinglyLinkedList<K,V>& l = arr[i];
            for(Node<K,V>* p = l.getHead(); p != NULL; p = p->getNext()){
                out += "[" + p->getKey() + "]:" + p->getValue()->toString() + "\n";
            }
    }
    return out;

}

template<typename K, typename V>
SinglyLinkedList<K, V>* Hashtable<K,V>::getArr() {
    return arr;
}
