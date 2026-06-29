#pragma once
#include "queue/queue.hpp"
#include <condition_variable>

namespace dispatcher::queue {

class UnboundedQueue : public IQueue {
    // здесь ваш код
public:
    explicit UnboundedQueue(int capacity);

    void push(std::function<void()> task) override;

    std::optional<std::function<void()>> try_pop() override;

    ~UnboundedQueue() override;

private:
    std::queue<std::function<void()>> q_;
    std::mutex mutex_;
    std::condition_variable not_full_;
};

}  // namespace dispatcher::queue