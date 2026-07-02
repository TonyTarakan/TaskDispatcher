#include <future>
#include <gtest/gtest.h>

#include "queue/bounded_queue.hpp"

TEST(BoundedQueueCheck, EmptyPop) {
    dispatcher::queue::BoundedQueue q{2};
    ASSERT_FALSE(q.try_pop().has_value());
}

TEST(BoundedQueueCheck, PushPop) {
    dispatcher::queue::BoundedQueue q{2};
    bool v1 = false;
    bool v2 = false;
    q.push([&]() { v1 = true; });
    q.push([&]() { v2 = true; });

    EXPECT_EQ(v1, false);
    EXPECT_EQ(v2, false);
    (*q.try_pop())();
    EXPECT_EQ(v1, true);
    EXPECT_EQ(v2, false);
    (*q.try_pop())();
    EXPECT_EQ(v1, true);
    EXPECT_EQ(v2, true);
    ASSERT_FALSE(q.try_pop().has_value());
}

TEST(BoundedQueueCheck, OverflowBlock) {
    dispatcher::queue::BoundedQueue q{2};
    bool v1 = false;
    bool v2 = false;
    bool v3 = false;
    q.push([&]() { v1 = true; });
    q.push([&]() { v2 = true; });

    auto future = std::async(std::launch::async, [&]() { q.push([]() {}); });

    // Block check
    auto status = future.wait_for(std::chrono::milliseconds(100));
    EXPECT_EQ(status, std::future_status::timeout);

    // Free place
    q.try_pop();

    // Now should work
    status = future.wait_for(std::chrono::milliseconds(100));
    EXPECT_EQ(status, std::future_status::ready);
}
