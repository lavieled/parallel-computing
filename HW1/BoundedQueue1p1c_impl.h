#pragma once

#include "BoundedQueue1p1c.h"

#include <atomic>

class BoundedQueue1p1c : public BoundedQueueAbstract_1p1c {
public:
    explicit BoundedQueue1p1c(int capacity)
        : capacity(capacity),
          head(0),
          tail(0),
          count(0),
          buffer(new int[capacity]) {}

    ~BoundedQueue1p1c() { delete[] buffer; }

    int size() override { return count.load(std::memory_order_acquire); }

    bool pop(int& val) override {
        if (count.load(std::memory_order_acquire) == 0) {
            return false; // queue empty
        }
        val = buffer[head];
        head = (head + 1) % capacity; // consumer only
        count.fetch_sub(1, std::memory_order_release);
        return true;
    }

    bool push(int v) override {
        if (count.load(std::memory_order_acquire) >= capacity) {
            return false; // queue full
        }
        buffer[tail] = v;
        tail = (tail + 1) % capacity; // producer only
        count.fetch_add(1, std::memory_order_release);
        return true;
    }

private:
    int capacity;
    int head; // consumer index
    int tail; // producer index
    std::atomic<int> count; // sync visibility
    int* buffer;
};
