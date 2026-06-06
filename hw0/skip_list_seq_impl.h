#ifndef SKIP_LIST_SEQ_IMPL_H
#define SKIP_LIST_SEQ_IMPL_H

#include "skip_list_seq.h"

#include <climits>
#include <cstdlib>
#include <vector>

// skip list
class SkipListImpl : public SkipList {
public:
    explicit SkipListImpl(int maxLevel)
        : SkipList(maxLevel), m_head(new Node(INT_MIN, m_maxLevel)) {}

    ~SkipListImpl() {
        Node* curr = m_head;
        while (curr != NULL) {
            Node* next = curr->next[0];
            delete curr;
            curr = next;
        }
    }

    virtual void insert(int value) {
        std::vector<Node*> update(static_cast<std::size_t>(m_maxLevel + 1), NULL);
        Node* curr = m_head;

        // find predecessors at each level
        for (int level = m_maxLevel; level >= 0; --level) {
            while (curr->next[level] != NULL && curr->next[level]->value < value) {
                curr = curr->next[level];
            }
            update[level] = curr;
        }

        curr = curr->next[0];
        if (curr != NULL && curr->value == value) {
            return;
        }

        int level = random_level();
        Node* node = new Node(value, level);

        for (int i = 0; i <= level; ++i) {
            node->next[i] = update[i]->next[i];
            update[i]->next[i] = node;
        }
    }

    virtual bool search(int value) {
        Node* curr = m_head;

        for (int level = m_maxLevel; level >= 0; --level) {
            while (curr->next[level] != NULL && curr->next[level]->value < value) {
                curr = curr->next[level];
            }
        }

        curr = curr->next[0];
        return curr != NULL && curr->value == value;
    }

    virtual void remove(int value) {
        std::vector<Node*> update(static_cast<std::size_t>(m_maxLevel + 1), NULL);
        Node* curr = m_head;

        for (int level = m_maxLevel; level >= 0; --level) {
            while (curr->next[level] != NULL && curr->next[level]->value < value) {
                curr = curr->next[level];
            }
            update[level] = curr;
        }

        curr = curr->next[0];
        if (curr == NULL || curr->value != value) {
            return;
        }

        // remove all node lvls
        for (int level = 0; level <= m_maxLevel; ++level) {
            if (update[level]->next[level] != curr) {
                break;
            }
            update[level]->next[level] = curr->next[level];
        }

        delete curr;
    }

private:
    struct Node {
        Node(int value, int level) : value(value), next(static_cast<std::size_t>(level + 1), NULL) {}
        int value;
        std::vector<Node*> next;
    };

    //  for random - add while is odd
    int random_level() const {
        int level = 0;
        while (level < m_maxLevel && (std::rand() % 2) == 1) {
            ++level;
        }
        return level;
    }

    Node* m_head;
};

#endif
