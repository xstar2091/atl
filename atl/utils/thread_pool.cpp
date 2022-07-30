#include "atl/utils/thread_pool.h"


namespace atl {

AsyncGroup::~AsyncGroup() {}

AsyncGroupImpl::AsyncGroupImpl(std::function<void()>&& group_finish_callback)
    : AsyncGroup() {
    if (group_finish_callback) {
        this->group_finish_callback = std::forward<std::function<void()>>(group_finish_callback);
    }
}

AsyncGroupImpl::~AsyncGroupImpl() {}

void AsyncGroupImpl::Push(std::function<void()>&& async_task_function, std::function<void()>&& finish_callback) {
    this->task_list.emplace_back(std::make_pair(std::forward<std::function<void()>>(async_task_function),
                                                std::forward<std::function<void()>>(finish_callback)));
}

bool AsyncGroupImpl::IsAllFinished() const {
    int finished_count = this->finish_count.load();
    int total_count = static_cast<int>(this->task_list.size());
    return finished_count == total_count;
}

AsyncTaskCallable::CallableBase::~CallableBase() {}

AsyncTaskCallable::AsyncTaskCallable() noexcept {
    group = nullptr;
}

AsyncTaskCallable::AsyncTaskCallable(AsyncTaskCallable&& other) {
    group = other.group;
    callable = std::move(other.callable);
    other.group = nullptr;
}

AsyncTaskCallable& AsyncTaskCallable::operator=(AsyncTaskCallable &&other) {
    group = other.group;
    callable = std::move(other.callable);
    other.group = nullptr;
    return *this;
}

AsyncGroup* ThreadPool::CreateAsyncGroup(std::function<void()>&& group_finish_callback) {
    return new AsyncGroupImpl(std::forward<std::function<void()>>(group_finish_callback));
}

void ThreadPool::Start(int pool_size) {
    if (pool_size <= 0) {
        pool_size = static_cast<int>(std::thread::hardware_concurrency());
    }
    next_ = true;
    for (int i = 0; i < pool_size; i++) {
        pool_.push_back(std::thread(&ThreadPool::WorkThread, this));
    }
}

void ThreadPool::Push(atl::AsyncGroup* group) {
    atl::AsyncGroupImpl* impl = static_cast<atl::AsyncGroupImpl*>(group);
    std::lock_guard<std::mutex> lock(mtx_);
    for (auto& pair : impl->task_list) {
        tasks_.emplace(AsyncTaskCallable(std::move(pair.first), std::move(pair.second)));
        tasks_.back().group = group;
    }
    cv_.notify_all();
}

void ThreadPool::Stop() {
    std::lock_guard<std::mutex> lock(mtx_);
    next_ = false;
    while (!tasks_.empty()) {
        tasks_.pop();
    }
    cv_.notify_all();
}

void ThreadPool::Wait() {
    for (auto& thrd : pool_) {
        thrd.join();
    }
}

void ThreadPool::WorkThread() {
    while (next_) {
        {
            std::unique_lock<std::mutex> lock(mtx_);
            cv_.wait(lock, [this]() -> bool {
                return !this->tasks_.empty() || !next_;
            });
        }
        while (next_) {
            AsyncTaskCallable task;
            std::lock_guard<std::mutex> lock(mtx_);
            if (!tasks_.empty()) {
                task = std::move(tasks_.front());
                tasks_.pop();
                task.callable->CallAsyncFunction();
                task.callable->CallFinishCallback();
                if (task.group == nullptr) {
                    continue;
                }
                AsyncGroupImpl* impl = static_cast<AsyncGroupImpl*>(task.group);
                impl->FinishOne();
                if (impl->IsAllFinished()) {
                    if (impl->group_finish_callback) {
                        impl->group_finish_callback();
                    }
                    delete impl;
                }
            }
        }
    }
}

}
