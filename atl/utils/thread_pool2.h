#pragma once

#include "atl/utils/thread_pool.h"

namespace atl {

class ThreadPool2 {
public:
    static AsyncGroup* CreateAsyncGroup(std::function<void()>&& group_finish_callback = nullptr);

public:
    ThreadPool2();
    bool IsStopped() const { return !next_; }
    void Start(int pool_size = 0);

    template<class AsyncFunctionType>
    std::future<typename std::result_of<AsyncFunctionType()>::type> Push(AsyncFunctionType&& async_function) {
        uint64_t index = index_.fetch_add(1) % pool_size_;
        ThreadPool* pool = pool_[index];
        return pool->Push<AsyncFunctionType>(std::forward<AsyncFunctionType>(async_function));
    }

    template<class AsyncFunctionType, class CallbackType>
    void Push(AsyncFunctionType&& async_function,
              CallbackType&& callback_function) {
        uint64_t index = index_.fetch_add(1) % pool_size_;
        ThreadPool* pool = pool_[index];
        pool->Push(std::forward<AsyncFunctionType>(async_function),
                  std::forward<CallbackType>(callback_function));
    }
    void Push(AsyncGroup* group);
    void Stop();
    void Wait();

private:
    void WorkThread();

private:
    uint64_t pool_size_;
    std::atomic<bool> next_;
    std::atomic<uint64_t> index_;
    std::vector<ThreadPool*> pool_;
};

}
