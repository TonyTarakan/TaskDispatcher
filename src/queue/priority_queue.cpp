#include "queue/priority_queue.hpp"
#include "queue/bounded_queue.hpp"
#include "queue/unbounded_queue.hpp"
#include "types.hpp"
#include <algorithm>
#include <memory>
#include <mutex>

namespace dispatcher::queue {

PriorityQueue::PriorityQueue(std::unordered_map<TaskPriority, QueueOptions> config) {
    std::for_each(config.begin(), config.end(), [this](const auto &conf) {
        auto opts = conf.second;
        if (opts.bounded) {
            auto q = std::make_unique<BoundedQueue>(opts.capacity.value());
            map_.emplace(conf.first, std::move(q));
        } else {
            auto q = std::make_unique<UnboundedQueue>(0);
            map_.emplace(conf.first, std::move(q));
        }
    });
}

void PriorityQueue::push(TaskPriority priority, std::function<void()> task) {
    map_[priority]->push(std::move(task));
    not_empty_.notify_one();
}

std::optional<std::function<void()>> PriorityQueue::pop() {
    auto high_task = map_[TaskPriority::High]->try_pop();
    if (high_task)
        return high_task;

    auto norm_task = map_[TaskPriority::Normal]->try_pop();

    // TODO:
    // block on pop until shutdown is called
    // after that return std::nullopt on empty queue
}

void PriorityQueue::shutdown() {}

}  // namespace dispatcher::queue