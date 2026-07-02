#include "thread_pool/thread_pool.hpp"
#include "logger.hpp"
#include <exception>

namespace dispatcher::thread_pool {

ThreadPool::~ThreadPool() {
    queue_->shutdown();
    for (auto &t : threads_) {
        t.join();
    }
}

void ThreadPool::run() {
    while (auto task = queue_->pop()) {
        try {
            (*task)();
        } catch (std::exception &ex) {
            Logger::Get().Log(std::string{"Error while processing task. Exception: "} + ex.what());
        }
    }
}

}  // namespace dispatcher::thread_pool