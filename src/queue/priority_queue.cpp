#include "queue/priority_queue.hpp"
#include "queue/bounded_queue.hpp"
#include "queue/unbounded_queue.hpp"
#include "types.hpp"
#include <algorithm>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>

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

    std::unique_lock lock{mutex_};

    std::optional<std::function<void()>> task{std::nullopt};

    not_empty_.wait(lock, [&task, this]() {
        task = map_[TaskPriority::High]->try_pop();
        if (task)
            return true;

        task = map_[TaskPriority::Normal]->try_pop();
        if (task)
            return true;

        if (shutdown_)
            return true;

        return false;
    });

    return task;
}

void PriorityQueue::shutdown() {
    shutdown_.store(true);
    not_empty_.notify_all();
}

}  // namespace dispatcher::queue