#include <gtest/gtest.h>

#include "queue/unbounded_queue.hpp"

TEST(UnboundedQueueCheck, EmptyPop) {
    dispatcher::queue::UnboundedQueue q{0};
    ASSERT_FALSE(q.try_pop().has_value());
}

TEST(UnboundedQueueCheck, PushPop) {
    dispatcher::queue::UnboundedQueue q{0};
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
