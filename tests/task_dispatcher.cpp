#include <gtest/gtest.h>

#include <array>
#include <thread>
#include <vector>

#include "task_dispatcher.hpp"

using namespace dispatcher;

TEST(TaskDispatcherCheck, SingleThreadSimple) {
    constexpr int CNT = 10;
    std::array<int, CNT> arr{};

    {
        TaskDispatcher td{1};
        for (int i = 0; i < CNT; ++i) {
            td.schedule(TaskPriority::Normal, [&arr, i]() { arr[i] = i; });
        }
    }

    for (int i = 0; i < CNT; ++i) {
        EXPECT_EQ(arr[i], i);
    }
}

TEST(TaskDispatcherCheck, SingleThreadPrio) {
    constexpr int CNT = 10;
    constexpr int HV = 22;
    constexpr int NV = 11;
    std::vector<int> vec;
    std::mutex mutex;
    vec.reserve(CNT * 2 + 1);

    {
        TaskDispatcher td{1};

        td.schedule(TaskPriority::High, []() { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });

        for (int i = 0; i < CNT; ++i) {
            td.schedule(TaskPriority::Normal, [&vec, &mutex, NV]() {
                std::lock_guard lock(mutex);
                vec.emplace_back(NV);
            });
        }

        for (int i = 0; i < CNT; ++i) {
            td.schedule(TaskPriority::High, [&vec, &mutex, HV]() {
                std::lock_guard lock(mutex);
                vec.emplace_back(HV);
            });
        }
    }

    for (int i = 0; i < CNT; ++i) {
        EXPECT_EQ(vec.at(i), HV);
    }
    for (int i = CNT; i < CNT * 2; ++i) {
        EXPECT_EQ(vec.at(i), NV);
    }
}