#pragma once

#include "Connection.h"

#include <condition_variable>
#include <memory>
#include <mutex>

class ConnectionPool : public ConnectionPoolAbstract {
public:
    explicit ConnectionPool(size_t poolSize) : ConnectionPoolAbstract(poolSize) {
        for (size_t i = 0; i < poolSize; ++i) {
            pool.push(std::unique_ptr<Connection>(new Connection(static_cast<int>(i))));
        }
    }

    std::shared_ptr<Connection> borrowConnection() override {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !pool.empty(); }); // wait for free

        std::unique_ptr<Connection> conn = std::move(pool.front());
        pool.pop();
        lock.unlock();

        ConnectionPool* self = this;
        return std::shared_ptr<Connection>(
            conn.release(),
            [self](Connection* connection) {
                std::lock_guard<std::mutex> guard(self->mtx);
                self->pool.push(std::unique_ptr<Connection>(connection)); // auto return
                self->cv.notify_one(); // wake waiter
            });
    }

private:
    std::mutex mtx;
    std::condition_variable cv;
};
