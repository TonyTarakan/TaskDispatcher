#pragma once

#include <memory>
#include <utility>

#include "queue/priority_queue.hpp"
#include "thread_pool/thread_pool.hpp"
#include "types.hpp"

namespace dispatcher {

using QueueCfg = std::unordered_map<TaskPriority, queue::QueueOptions>;

class TaskDispatcher {
public:
    explicit TaskDispatcher(size_t thread_count, QueueCfg config = DEFAULT_CFG_)
        : pqueue_(std::make_shared<queue::PriorityQueue>(config)), tpool_(pqueue_, thread_count) {}
    void schedule(TaskPriority priority, std::function<void()> task);
    ~TaskDispatcher() = default;

private:
    std::shared_ptr<queue::PriorityQueue> pqueue_;
    thread_pool::ThreadPool tpool_;

private:
    static const QueueCfg DEFAULT_CFG_;
};

}  // namespace dispatcher