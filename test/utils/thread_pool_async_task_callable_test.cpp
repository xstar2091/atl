#include <gtest/gtest.h>

#include <atomic>
#include "atl/utils/thread_pool.h"

TEST(AsyncTaskCallable, ConstructorDefault) {
    atl::AsyncTaskCallable task;
    EXPECT_TRUE(task.group == nullptr);
    EXPECT_TRUE(task.callable == nullptr);
}

// 测试使用async_func构造AsyncTaskCallable
void TestConstructor_Func_Nullptr() {
    int num = 1;
    atl::AsyncTaskCallable task;
    auto async_func = [&num](){
        num++;
    };

    EXPECT_EQ(1, num);
    task = atl::AsyncTaskCallable(async_func);
    EXPECT_EQ(1, num);
    task.callable->CallAsyncFunction();
    EXPECT_EQ(2, num);
    task.callable->CallFinishCallback();
    EXPECT_EQ(2, num);
}

// 测试使用nullptr, finish_callback构造AsyncTaskCallable
void TestConstructor_Func_FinishCallback() {
    int num = 1;
    atl::AsyncTaskCallable task;
    auto async_func = [&num](){
        num++;
    };
    auto finish_callback = [&num](){
        num++;
    };

    EXPECT_EQ(1, num);
    task = atl::AsyncTaskCallable(std::bind(async_func), std::bind(finish_callback));
    EXPECT_EQ(1, num);
    task.callable->CallAsyncFunction();
    EXPECT_EQ(2, num);
    task.callable->CallFinishCallback();
    EXPECT_EQ(3, num);
}

TEST(AsyncTaskCallable, ConstructorTwo) {
    TestConstructor_Func_Nullptr();
    TestConstructor_Func_FinishCallback();
}

TEST(AsyncTaskCallable, MoveConstructor) {
    int num = 1;
    auto async_func = [&num](){
        num++;
    };
    auto finish_callback = [&num](){
        num++;
    };

    atl::AsyncTaskCallable task(atl::AsyncTaskCallable(std::bind(async_func), std::bind(finish_callback)));
    EXPECT_EQ(1, num);
    task.callable->CallAsyncFunction();
    EXPECT_EQ(2, num);
    task.callable->CallFinishCallback();
    EXPECT_EQ(3, num);
}
