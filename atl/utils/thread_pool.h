#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <string_view>

namespace atl {

class AsyncGroup {
public:
    virtual ~AsyncGroup();
    virtual void Push(std::function<void()>&& async_task_function) = 0;
    virtual void Push(std::function<void()>&& async_task_function, std::function<void()>&& finish_callback) = 0;
};

class AsyncGroupImpl : public AsyncGroup {
public:
    AsyncGroupImpl(std::function<void()>&& group_finish_callback);
    virtual ~AsyncGroupImpl();
    void Push(std::function<void()>&& async_task_function) override;
    void Push(std::function<void()>&& async_task_function, std::function<void()>&& finish_callback) override;

public:
    bool IsAllFinished();

public:
    std::atomic<int> finish_count;
    std::function<void()> group_finish_callback;
    std::vector<std::pair<std::function<void()>, std::function<void()>>> task_list;
};

class AsyncTaskCallable {
private:
    struct EmptyCallback {
        void operator()() {}
    };

    struct CallableBase {
        virtual ~CallableBase();
        virtual void CallAsyncFunction() = 0;
        virtual void CallFinishCallback() = 0;
    };

    template<class FunctionType, class CallbackType>
    struct CallableImpl : public CallableBase {
        FunctionType async_function;
        CallbackType finish_callback;

        CallableImpl(FunctionType&& func, CallbackType&& callback)
            : async_function(std::forward<FunctionType>(func))
            , finish_callback(std::forward<CallbackType>(callback)) {
        }

        virtual void CallAsyncFunction() override {
            async_function();
        }

        virtual void CallFinishCallback() override {
            finish_callback();
        }
    };

public:
    AsyncGroup* group;
    std::unique_ptr<CallableBase> callable;

public:
    AsyncTaskCallable() noexcept;
    template<class FunctionType>
    AsyncTaskCallable(FunctionType&& func) {
        group = nullptr;
        callable = std::make_unique<CallableImpl<FunctionType, EmptyCallback>>(
            std::forward<FunctionType>(func), EmptyCallback());
    }
    template<class FunctionType, class CallbackType>
    AsyncTaskCallable(FunctionType&& func, CallbackType&& callback) {
        group = nullptr;
        callable = std::make_unique<CallableImpl<FunctionType, CallbackType>>(
            std::forward<FunctionType>(func),
            std::forward<CallbackType>(callback));
    }
    AsyncTaskCallable(AsyncTaskCallable&& other);
    AsyncTaskCallable& operator=(AsyncTaskCallable&& other);

    AsyncTaskCallable(const AsyncTaskCallable&) = delete;
    AsyncTaskCallable& operator=(const AsyncTaskCallable&) = delete;
};

class ThreadPool {
public:
    static AsyncGroup* CreateAsyncGroup(std::function<void()>&& group_finish_callback = nullptr);

public:
    ThreadPool();
    bool IsStopped() const { return !next_; }
    void Start(int pool_size = 0);

    template<class AsyncFunctionType>
    std::future<typename std::result_of<AsyncFunctionType()>::type> Push(AsyncFunctionType&& async_function) {
        using result_type = typename std::result_of<AsyncFunctionType()>::type;
        std::packaged_task<result_type()> task(std::move(async_function));
        std::future<result_type> future = task.get_future();
        std::lock_guard<std::mutex> lock(mtx_);
        tasks_.emplace(AsyncTaskCallable(std::move(task)));
        cv_.notify_one();
        return future;
    }

    template<class AsyncFunctionType, class CallbackType>
    void Push(AsyncFunctionType&& async_function,
              CallbackType&& callback_function) {
        std::lock_guard<std::mutex> lock(mtx_);
        tasks_.emplace(AsyncTaskCallable(std::forward<AsyncFunctionType>(async_function),
                                         std::forward<CallbackType>(callback_function)));
        cv_.notify_one();
    }

    template<class AsyncFunctionType, class CallbackType>
    void Push(AsyncFunctionType&& async_function,
              CallbackType&& callback_function,
              AsyncGroup* group) {
        std::lock_guard<std::mutex> lock(mtx_);
        tasks_.emplace(AsyncTaskCallable(std::forward<AsyncFunctionType>(async_function),
                                         std::forward<CallbackType>(callback_function)));
        tasks_.back().group = group;
        cv_.notify_one();
    }
    void Push(AsyncGroup* group);
    void Stop();
    void Wait();

private:
    void WorkThread();

private:
    std::mutex mtx_;
    std::condition_variable cv_;
    std::vector<std::thread> pool_;
    std::queue<AsyncTaskCallable> tasks_;
    std::atomic<bool> next_;
};

}
