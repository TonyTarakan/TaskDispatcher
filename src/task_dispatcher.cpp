#include "task_dispatcher.hpp"

namespace dispatcher {

void TaskDispatcher::schedule(TaskPriority priority, std::function<void()> task) {
    pqueue_->push(priority, std::move(task));
}

}  // namespace dispatcher