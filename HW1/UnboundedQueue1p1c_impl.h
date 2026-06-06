#pragma once

#include "UnboundedQueue1p1c.h"

struct QueueNode {
    int value;
    QueueNode* next;
};

class UnboundedQueue1p1c : public UnboundedQueue1p1cAbstract {
public:
    UnboundedQueue1p1c() {
        head = new QueueNode;
        head->value = 0;
        head->next = nullptr;
        tail = head; // fixed dummy node
    }

    ~UnboundedQueue1p1c() {
        QueueNode* curr = head;
        while (curr != nullptr) {
            QueueNode* next = curr->next;
            delete curr;
            curr = next;
        }
    }

    bool pop(int& val) override {
        if (head == tail) {
            return false; // only dummy left
        }
        QueueNode* node = head->next; // pop head->next
        val = node->value;
        if (node == tail) {
            tail = head; // reset empty tail
        }
        head->next = node->next;
        delete node;
        return true;
    }

    void push(int value) override {
        QueueNode* node = new QueueNode;
        node->value = value;
        node->next = nullptr;
        tail->next = node; // append after tail
        tail = node;
    }

    int size() const override {
        int count = 0;
        for (QueueNode* curr = head->next; curr != nullptr; curr = curr->next) {
            ++count;
        }
        return count;
    }

private:
    QueueNode* head; // never moves
    QueueNode* tail;
};
