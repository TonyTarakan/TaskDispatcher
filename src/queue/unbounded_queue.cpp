#include "queue/unbounded_queue.hpp"

#include <functional>
#include <mutex>
#include <queue>
#include <semaphore>

namespace dispatcher::queue {

void UnboundedQueue::push(std::function<void()> task) {}

std::optional<std::function<void()>> UnboundedQueue::try_pop() {}

}  // namespace dispatcher::queue