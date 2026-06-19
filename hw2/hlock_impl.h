#ifndef HLOCK_IMPL_H
#define HLOCK_IMPL_H

#include "hlock.h"

#include <mutex>
#include <vector>

class HierarchicalMutex_impl : public HierarchicalMutex {
public:
    explicit HierarchicalMutex_impl(int lvl) : HierarchicalMutex(lvl), m_level(lvl) {}

    void lock() override {
        if (!heldLevels().empty() && heldLevels().back() >= m_level) {
            throw HierarchicalMutexException(); // wrong lock order
        }
        m_mutex.lock();
        heldLevels().push_back(m_level);
    }

    void unlock() override {
        m_mutex.unlock();
        heldLevels().pop_back(); // assume LIFO unlock
    }

    bool try_lock() override {
        if (!heldLevels().empty() && heldLevels().back() >= m_level) {
            return false;
        }
        if (!m_mutex.try_lock()) {
            return false;
        }
        heldLevels().push_back(m_level);
        return true;
    }

private:
    static std::vector<int>& heldLevels() {
        thread_local std::vector<int> levels; // per-thread lock stack
        return levels;
    }

    int m_level;
    std::mutex m_mutex;
};

#endif
