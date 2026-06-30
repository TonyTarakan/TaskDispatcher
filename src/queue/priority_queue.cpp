#include "queue/priority_queue.hpp"

namespace dispatcher::queue {

PriorityQueue::PriorityQueue(std::unordered_map<TaskPriority, QueueOptions> config) {}

// здесь ваш код
void PriorityQueue::push(TaskPriority priority, std::function<void()> task) {}

std::optional<std::function<void()>> PriorityQueue::pop() {}

void PriorityQueue::shutdown() {}

}  // namespace dispatcher::queue