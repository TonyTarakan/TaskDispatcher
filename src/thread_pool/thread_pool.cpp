#include "thread_pool/thread_pool.hpp"

namespace dispatcher::thread_pool {

ThreadPool::~ThreadPool() {
    queue_->shutdown();
    for (auto &t : threads_) {
        t.join();
    }
}

void ThreadPool::run() {
    while (auto task = queue_->pop()) {
        (*task)();
    }
}

}  // namespace dispatcher::thread_pool