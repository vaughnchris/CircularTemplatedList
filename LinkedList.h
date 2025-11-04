//
// Created by Christopher Vaughn on 10/24/25.
//

#ifndef CIRCULARTEMPLATEDLIST_LINKEDLIST_H
#define CIRCULARTEMPLATEDLIST_LINKEDLIST_H

#include "Media.h"
#include <iostream>
template <typename T>
struct Node {
    T*    data;
    Node* next;

    explicit Node(T* media) : data(media), next(nullptr) {}
};

// =======================
// LinkedList (templated, singly-linked circular)
// Stores T* (e.g., Media*) and supports polymorphic play()
// =======================
template <typename T>
class LinkedList {
private:
    Node<T>* head;     // first element (nullptr if empty)
    Node<T>* tail;     // last element (tail->next == head when non-empty)
    Node<T>* current;  // for playNext() traversal

public:
    // Constructor: empty circular list
    LinkedList() : head(nullptr), tail(nullptr), current(nullptr) {}

    // Destructor: delete all nodes AND the owned T objects they point to
    ~LinkedList() {
        if (!head) return;

        // Break the cycle by iterating once around
        Node<T>* cur = head->next;
        while (cur != head) {
            Node<T>* nxt = cur->next;
            delete cur->data; // polymorphic delete via virtual destructor on base
            delete cur;
            cur = nxt;
        }
        // delete head last
        delete head->data;
        delete head;

        head = tail = current = nullptr;
        std::cout << "\n[Playlist cleanup complete. All memory deallocated.]\n";
    }

    // Insert at END; maintain circular invariant tail->next == head
    void insert(T* newMedia) {
        Node<T>* newNode = new Node<T>(newMedia);

        if (!head) {
            // first node
            head = tail = newNode;
            tail->next = head;     // make circular
            current = head;        // start playback from head
        } else {
            tail->next = newNode;  // old tail points to new
            tail = newNode;        // advance tail
            tail->next = head;     // keep circle intact
        }
    }

    // Display whole list once around
    void displayList() const {
        if (!head) {
            std::cout << "\n[Playlist is empty.]\n";
            return;
        }

        std::cout << "\n-- Current Playlist --\n";
        const Node<T>* cur = head;
        int index = 1;
        do {
            // polymorphism: toString() will dispatch to Song/Podcast implementation
            std::cout << index++ << ". " << cur->data->toString() << std::endl;
            cur = cur->next;
        } while (cur != head);
        std::cout << "----------------------\n";
    }

    // Play the item at 'current' (or head if current not set)
    void playCurrent() const {
        if (!head) {
            std::cout << "[Playlist is empty. Nothing to play.]\n";
            return;
        }
        (current ? current : head)->data->play();
    }

    // Advance one step (wraps automatically because it's circular) and play
    void playNext() {
        if (!head) {
            std::cout << "[Playlist is empty. Nothing to play.]\n";
            return;
        }
        if (!current) current = head; // safety
        current = current->next;      // one hop; wraps from tail to head
        current->data->play();
    }
};

#endif //CIRCULARTEMPLATEDLIST_LINKEDLIST_H
