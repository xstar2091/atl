#include <gtest/gtest.h>

#include <atomic>
#include "atl/utils/thread_pool.h"

TEST(AsyncGroup, Push) {
    std::atomic<int> num(0);
    atl::AsyncGroup* group = atl::ThreadPool::CreateAsyncGroup();
    atl::AsyncGroupImpl* impl = static_cast<atl::AsyncGroupImpl*>(group);

    auto async_func = [&num]() {
        num.fetch_add(1);
    };
    group->Push(async_func, nullptr);
    EXPECT_EQ(1, static_cast<int>(impl->task_list.size()));
    group->Push(async_func, nullptr);
    EXPECT_EQ(2, static_cast<int>(impl->task_list.size()));
    group->Push(async_func, nullptr);
    EXPECT_EQ(3, static_cast<int>(impl->task_list.size()));
    delete group;
}

TEST(AsyncGroupImpl, FinishOne) {
    atl::AsyncGroup* group = atl::ThreadPool::CreateAsyncGroup();
    atl::AsyncGroupImpl* impl = static_cast<atl::AsyncGroupImpl*>(group);

    group->Push(nullptr, nullptr);
    group->Push(nullptr, nullptr);
    group->Push(nullptr, nullptr);

    EXPECT_EQ(0, impl->finish_count.load());
    impl->FinishOne();
    EXPECT_EQ(1, impl->finish_count.load());
    impl->FinishOne();
    EXPECT_EQ(2, impl->finish_count.load());
    impl->FinishOne();
    EXPECT_EQ(3, impl->finish_count.load());
    impl->FinishOne();
    EXPECT_EQ(4, impl->finish_count.load());
    impl->FinishOne();
    EXPECT_EQ(5, impl->finish_count.load());
    delete group;
}

TEST(AsyncGroupImpl, IsAllFinished) {
    atl::AsyncGroup* group = atl::ThreadPool::CreateAsyncGroup();
    atl::AsyncGroupImpl* impl = static_cast<atl::AsyncGroupImpl*>(group);

    group->Push(nullptr, nullptr);
    group->Push(nullptr, nullptr);
    group->Push(nullptr, nullptr);

    EXPECT_FALSE(impl->IsAllFinished());
    impl->FinishOne();
    EXPECT_FALSE(impl->IsAllFinished());
    impl->FinishOne();
    EXPECT_FALSE(impl->IsAllFinished());
    impl->FinishOne();
    EXPECT_TRUE(impl->IsAllFinished());
    impl->FinishOne();
    EXPECT_FALSE(impl->IsAllFinished());
    impl->FinishOne();
    EXPECT_FALSE(impl->IsAllFinished());
    delete group;
}
