#pragma once
#include "queue/queue.hpp"
#include <condition_variable>
#include <queue>

namespace dispatcher::queue {

class BoundedQueue : public IQueue {
    // здесь ваш код
public:
    explicit BoundedQueue(int capacity) : capacity_(capacity) {}

    void push(std::function<void()> task) override;

    std::optional<std::function<void()>> try_pop() override;

    ~BoundedQueue() override;

private:
    std::queue<std::function<void()>> q_;
    std::mutex mutex_;
    std::condition_variable not_full_;
    int capacity_;
};

}  // namespace dispatcher::queue