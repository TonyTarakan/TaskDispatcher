#pragma once

#include "queue/priority_queue.hpp"
#include <functional>
#include <thread>
#include <vector>

namespace dispatcher::thread_pool {

class ThreadPool final {
public:
    explicit ThreadPool(std::shared_ptr<queue::PriorityQueue> pq, size_t thread_count) : queue_(pq) {
        for (size_t i = 0; i < thread_count; ++i) {
            threads_.emplace_back([this]() { run(); });
        }
    }

    ~ThreadPool();

private:
    void run();

private:
    std::shared_ptr<queue::PriorityQueue> queue_;
    std::vector<std::thread> threads_;
};

}  // namespace dispatcher::thread_pool
