#pragma once

#include "BoundedQueue.h"

#include <condition_variable>
#include <mutex>

class BoundedQueue : public BoundedQueueAbstract {
public:
    explicit BoundedQueue(int capacity)
        : buffer(new int[capacity]), capacity(capacity), count(0), head(0), tail(0) {}

    ~BoundedQueue() { delete[] buffer; }

    int size() override {
        std::lock_guard<std::mutex> lock(mtx);
        return count;
    }

    int pop() override {
        std::unique_lock<std::mutex> lock(mtx);
        not_empty.wait(lock, [this] { return count > 0; }); // wait for data
        int value = buffer[head];
        head = (head + 1) % capacity;
        --count;
        not_full.notify_one(); // wake producer
        return value;
    }

    void push(int v) override {
        std::unique_lock<std::mutex> lock(mtx);
        not_full.wait(lock, [this] { return count < capacity; }); // wait for space
        buffer[tail] = v;
        tail = (tail + 1) % capacity;
        ++count;
        not_empty.notify_one(); // wake consumer
    }

private:
    int* buffer; // fixed ring buffer
    int capacity;
    int count;
    int head;
    int tail;
    std::mutex mtx;
    std::condition_variable not_empty;
    std::condition_variable not_full;
};
