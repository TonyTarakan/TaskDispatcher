#include "task_dispatcher.hpp"

namespace dispatcher {

void TaskDispatcher::schedule(TaskPriority priority, std::function<void()> task) {
    pqueue_->push(priority, std::move(task));
}

const QueueCfg TaskDispatcher::DEFAULT_CFG_ = {{TaskPriority::High, {true, 1000}}, {TaskPriority::Normal, {false, 0}}};

}  // namespace dispatcher