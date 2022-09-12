#include "atl/utils/thread_pool2.h"

namespace atl {

ThreadPool2::ThreadPool2()
    : pool_size_(0)
    , next_(false)
    , index_(0) {}

AsyncGroup* ThreadPool2::CreateAsyncGroup(std::function<void()>&& group_finish_callback) {
    return ThreadPool::CreateAsyncGroup(std::forward<std::function<void()>>(group_finish_callback));
}

void ThreadPool2::Start(int pool_size) {
    if (pool_size <= 0) {
        pool_size = static_cast<int>(std::thread::hardware_concurrency());
    }
    
    next_.store(true);
    index_.store(0);
    pool_size_ = static_cast<uint64_t>(pool_size);
    for (int i = 0; i < pool_size; i++) {
        pool_.push_back(new ThreadPool());
    }
    for (auto pool : pool_) {
        pool->Start(1);
    }
}

void ThreadPool2::Push(AsyncGroup* group) {
    AsyncGroupImpl* impl = static_cast<AsyncGroupImpl*>(group);
    for (auto& pair : impl->task_list) {
        ThreadPool* pool = pool_[index_.fetch_add(1) % pool_size_];
        pool->Push(std::move(pair.first), std::move(pair.second), group);
    }
}

void ThreadPool2::Stop() {
    next_.store(false);
    for (auto pool : pool_) {
        pool->Stop();
    }
}

void ThreadPool2::Wait() {
    for (auto pool : pool_) {
        pool->Wait();
    }
}

}
