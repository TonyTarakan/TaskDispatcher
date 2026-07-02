#include <gtest/gtest.h>

#include <future>

#include "queue/priority_queue.hpp"
#include "types.hpp"

using QueueCfg = std::unordered_map<dispatcher::TaskPriority, dispatcher::queue::QueueOptions>;

const QueueCfg CFG{{dispatcher::TaskPriority::High, {true, 2}}, {dispatcher::TaskPriority::Normal, {false, 0}}};

TEST(PriorityQueueCheck, EmptyPop) {
    dispatcher::queue::PriorityQueue q{CFG};

    auto future = std::async(std::launch::async, [&]() { return q.pop(); });

    auto status = future.wait_for(std::chrono::milliseconds(100));
    EXPECT_EQ(status, std::future_status::timeout);

    q.shutdown();

    status = future.wait_for(std::chrono::milliseconds(100));
    EXPECT_EQ(status, std::future_status::ready);
    EXPECT_FALSE(future.get().has_value());
}

TEST(PriorityQueueCheck, PriorityPushPop) {

    dispatcher::queue::PriorityQueue q{CFG};

    bool hv = false;
    bool nv = false;

    q.push(dispatcher::TaskPriority::Normal, [&]() { nv = true; });
    q.push(dispatcher::TaskPriority::High, [&]() { hv = true; });

    EXPECT_EQ(hv, false);
    EXPECT_EQ(nv, false);

    (*q.pop())();
    EXPECT_EQ(hv, true);
    EXPECT_EQ(nv, false);

    (*q.pop())();
    EXPECT_EQ(hv, true);
    EXPECT_EQ(nv, true);

    q.shutdown();
}