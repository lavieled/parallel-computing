# HW1 — Concurrency & smart pointers

## Implemented

| Exercise | File | Idea |
|----------|------|------|
| 1 | `BoundedQueue_impl.h` | Blocking bounded FIFO (`mutex` + `condition_variable`) |
| 2 | `BoundedQueue1p1c_impl.h` | SPSC bounded queue, try push/pop, `atomic` count only |
| 3 | `UnboundedQueue1p1c_impl.h` | C linked list + fixed dummy node, 1P1C |
| 4 | `ConnectionPool_Impl.h` | `shared_ptr` custom deleter, pool wait/notify |

Provided abstracts: `BoundedQueue.h`, `BoundedQueue1p1c.h`, `UnboundedQueue1p1c.h`, `Connection.h`.

## Gradescope upload

Submit only the four `*_impl.h` / `*_Impl.h` files.
