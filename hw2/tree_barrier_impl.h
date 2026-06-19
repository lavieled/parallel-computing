#ifndef TREE_BARRIER_IMPL_H
#define TREE_BARRIER_IMPL_H

#include "tree_barrier.h"

#include <atomic>
#include <vector>

class BinaryTreeBarrier : public BinaryTreeBarrierAbstract {
public:
    explicit BinaryTreeBarrier(int numThreads)
        : m_numThreads(numThreads), m_nodes(static_cast<std::size_t>(2 * numThreads - 1)) {
        for (int i = 0; i < 2 * numThreads - 1; ++i) {
            m_nodes[static_cast<std::size_t>(i)].count.store(children(i));
            m_nodes[static_cast<std::size_t>(i)].lockSense.store(0);
        }
    }

    void barrier() override {
        const int leaf = m_numThreads - 1 + thread_id;
        combine(leaf);
        m_sense = (m_sense == 0) ? 1 : 0; // flip phase for reuse
    }

private:
    struct Node {
        std::atomic<int> count;     // arrivals left at this node
        std::atomic<int> lockSense; // sense-reversing wakeup flag
    };

    bool isLeaf(int idx) const { return idx >= m_numThreads - 1; }

    int children(int idx) const { return isLeaf(idx) ? 1 : 2; }

    void combine(int idx) {
        Node& node = m_nodes[static_cast<std::size_t>(idx)];

        if (node.count.fetch_sub(1) == 1) { // last arriver
            if (idx > 0) {
                combine((idx - 1) / 2); // only combiner goes up
            }
            node.count.store(children(idx));
            node.lockSense.store(node.lockSense.load() == 0 ? 1 : 0); // release waiters
        }

        while (node.lockSense.load() != m_sense) { // spin until released
        }
    }

    static thread_local int m_sense;

    int m_numThreads;
    std::vector<Node> m_nodes;
};

thread_local int BinaryTreeBarrier::m_sense = 1;

#endif
