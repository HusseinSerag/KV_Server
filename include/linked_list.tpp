#include <iostream>
#include <string>


template <typename K, typename V>
Node<K,V>::Node(K key, V val, Node* next) : next(next), key(key), val(val) {}


template <typename K, typename V>
V Node<K,V>::getValue() { return val; }


template <typename K, typename V>
Node<K,V>* Node<K,V>::getNext() { return next; }


template <typename K, typename V>
K Node<K,V>::getKey() {return this->key; }

template <typename K, typename V>
void Node<K,V>::setValue(V val) { this->val = val; }

template <typename K, typename V>
void Node<K,V>::setNext(Node* next) { this->next = next; }

template <typename K, typename V>
void Node<K,V>::setKey(K key) {this->key = key; }



// SinglyLinkedList methods
template <typename K, typename V>
SinglyLinkedList<K,V>::SinglyLinkedList() : head(nullptr), size(0) {}

template <typename K, typename V>
SinglyLinkedList<K,V>::~SinglyLinkedList() {
    Node<K,V>* p = head;
    while (head != nullptr) {
        p = head->next;
        delete head;
        head = p;
    }
}

template <typename K, typename V>
int SinglyLinkedList<K,V>::getSize() {
    return size;
}

template <typename K, typename V>
 Node<K,V>* SinglyLinkedList<K,V>::getHead() {

    return head;

 }
template <typename K, typename V>
void SinglyLinkedList<K,V>::appendToFront(K key, V val) {
    Node<K,V>* n = new Node<K,V>(key,val,nullptr);
    if (head == nullptr) {
        head = n;
    } else {
        n->next = head;
        head = n;
    }
    size++;
}

template <typename K, typename V>
Node<K,V>* SinglyLinkedList<K,V>::search(K key) {
    Node<K,V>* p = head;
    while (p != nullptr) {
        if (p->key == key) return p;
        p = p->next;
    }
    return nullptr;
}

template <typename K, typename V>
int SinglyLinkedList<K,V>::deleteBySearch(K key) {
    Node<K,V>* p = head;

    if (p != nullptr && p->key == key) {
        head = p->next;
        delete p;
        size--;
        return 1;
    }

    while (p != nullptr && p->next != nullptr) {
        if (p->next->key == key) {
            Node<K,V>* toDelete = p->next;
            p->next = toDelete->next;
            delete toDelete;
            size--;
            return 1;
        }
        p = p->next;
    }
    return 0;
}








