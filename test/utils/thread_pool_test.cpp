#include <gtest/gtest.h>

#include <atomic>
#include "atl/utils/thread_pool.h"

TEST(ThreadPool, Start) {
    atl::ThreadPool pool;
    EXPECT_TRUE(pool.IsStopped());

    pool.Start();
    EXPECT_FALSE(pool.IsStopped());

    pool.Stop();
    EXPECT_TRUE(pool.IsStopped());

    pool.Wait();
    EXPECT_TRUE(pool.IsStopped());
}

TEST(ThreadPool, PushBackgroundWorker) {
    int num = 1;
    auto func = [&num]() { num++; };
    atl::ThreadPool pool;
    pool.Start();

    EXPECT_EQ(1, num);
    pool.Push(func);
    pool.Stop();
    pool.Wait();
    EXPECT_EQ(2, num);
}

TEST(ThreadPool, PushWaitInt) {
    int num = 1;
    auto func = [&num]() -> int { num++; return num; };
    atl::ThreadPool pool;
    pool.Start();

    EXPECT_EQ(1, num);
    std::future<int> future = pool.Push(func);
    int result = future.get();
    EXPECT_EQ(2, result);
    pool.Stop();
    pool.Wait();
    EXPECT_EQ(2, num);
}

TEST(ThreadPool, PushWaitVoid) {
    int num = 1;
    auto func = [&num]() { num++; };
    atl::ThreadPool pool;
    pool.Start();

    EXPECT_EQ(1, num);
    std::future<void> future = pool.Push(func);
    future.get();
    EXPECT_EQ(2, num);
    pool.Stop();
    pool.Wait();
    EXPECT_EQ(2, num);
}

TEST(ThreadPool, PushWaitMulti) {
    std::atomic<int> num(1);
    auto func = [&num]() -> int { return num.fetch_add(1); };
    atl::ThreadPool pool;
    pool.Start();

    EXPECT_EQ(1, num);
    std::future<int> future1 = pool.Push(func);
    std::future<int> future2 = pool.Push(func);
    std::future<int> future3 = pool.Push(func);
    std::future<int> future4 = pool.Push(func);
    std::future<int> future5 = pool.Push(func);
    int result1 = future1.get();
    int result2 = future2.get();
    int result3 = future3.get();
    int result4 = future4.get();
    int result5 = future5.get();
    EXPECT_EQ(1, result1);
    EXPECT_EQ(2, result2);
    EXPECT_EQ(3, result3);
    EXPECT_EQ(4, result4);
    EXPECT_EQ(5, result5);
    pool.Stop();
    pool.Wait();
    EXPECT_EQ(6, num);
}

class PushWaitAsyncTestClass {
public:
    PushWaitAsyncTestClass()
        : count(0) {
        num1 = 1;
        num2 = 2;
        num3 = 3;
    }

    void Test1() {
        count.fetch_add(1);
        num1 += 1;
    }
    void Test1Callback() {
        EXPECT_EQ(2, num1);
        count.fetch_add(1);
    }
    void Test2() {
        count.fetch_add(1);
        num2 += 1;
    }
    void Test2Callback() {
        EXPECT_EQ(3, num2);
        count.fetch_add(1);
    }
    void Test3() {
        count.fetch_add(1);
        num3 += 1;
    }
    void Test3Callback() {
        EXPECT_EQ(4, num3);
        count.fetch_add(1);
    }

    int GetCount() { return count.load(); }

private:
    int num1;
    int num2;
    int num3;
    std::atomic<int> count;
};

TEST(ThreadPool, PushWaitAsyncClass) {
    PushWaitAsyncTestClass test_class;
    atl::ThreadPool pool;
    pool.Start();
    pool.Push(std::bind(&PushWaitAsyncTestClass::Test1, &test_class),
              std::bind(&PushWaitAsyncTestClass::Test1Callback, &test_class));
    pool.Push(std::bind(&PushWaitAsyncTestClass::Test2, &test_class),
              std::bind(&PushWaitAsyncTestClass::Test2Callback, &test_class));
    pool.Push(std::bind(&PushWaitAsyncTestClass::Test3, &test_class),
              std::bind(&PushWaitAsyncTestClass::Test3Callback, &test_class));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    pool.Stop();
    pool.Wait();
    EXPECT_EQ(6, test_class.GetCount());
}

TEST(ThreadPool, PushWaitAsyncLambda) {
    int num0 = 0;
    int num1 = 1;
    int num2 = 2;
    int num3 = 3;
    int num4 = 4;
    std::atomic<int> count(0);
    auto func = [&count](int& num) {
        count.fetch_add(1);
        num += 1;
    };
    auto callback = [&count](int expected, int& actual) {
        count.fetch_add(1);
        EXPECT_EQ(expected, actual);
    };
    atl::ThreadPool pool;
    pool.Start();
    pool.Push(std::bind(func, std::ref(num0)), std::bind(callback, 1, std::ref(num0)));
    pool.Push(std::bind(func, std::ref(num1)), std::bind(callback, 2, std::ref(num1)));
    pool.Push(std::bind(func, std::ref(num2)), std::bind(callback, 3, std::ref(num2)));
    pool.Push(std::bind(func, std::ref(num3)), std::bind(callback, 4, std::ref(num3)));
    pool.Push(std::bind(func, std::ref(num4)), std::bind(callback, 5, std::ref(num4)));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    pool.Stop();
    pool.Wait();
    EXPECT_EQ(10, count.load());
}

TEST(ThreadPool, PushWaitGroup) {
    std::atomic<int> count(0);
    std::vector<int> nums;
    for (int i = 0; i < 5; i++) nums.emplace_back(i);
    auto func = [&nums, &count](int index) {
        nums[index] += 1;
        count.fetch_add(1);
    };
    auto callback = [&nums, &count](int index) {
        count.fetch_add(1);
        int expected = index + 1;
        int actual = nums[index];
        EXPECT_EQ(expected, actual);
    };
    auto group_callback = [&count]() {
        EXPECT_EQ(10, count.load());
        count.fetch_add(1);
    };
    atl::ThreadPool pool;
    pool.Start();
    atl::AsyncGroup* group = atl::ThreadPool::CreateAsyncGroup(group_callback);
    group->Push(std::bind(func, 0), std::bind(callback, 0));
    group->Push(std::bind(func, 1), std::bind(callback, 1));
    group->Push(std::bind(func, 2), std::bind(callback, 2));
    group->Push(std::bind(func, 3), std::bind(callback, 3));
    group->Push(std::bind(func, 4), std::bind(callback, 4));
    pool.Push(group);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    pool.Stop();
    pool.Wait();
    EXPECT_EQ(11, count.load());
}
