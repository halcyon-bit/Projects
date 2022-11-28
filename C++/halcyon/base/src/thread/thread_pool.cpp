#include "base/thread/thread_pool.h"

#include <cassert>

using namespace halcyon::base;

ThreadPool::ThreadPool() noexcept
{}

ThreadPool::~ThreadPool()
{
    shutDown(false);
    // 确保线程退出
    for (auto& each : threads_) {
        each.join();
    }
}

bool ThreadPool::start(int32_t num_threads)
{
    if (num_threads <= 0) {
        return false;
    }

    if (started_) {
        return true;
    }
    started_ = true;

    assert(getTotalThreadNum() == 0);
    // 创建线程
    while (num_threads-- > 0) {
        threads_.emplace_back(&ThreadPool::threadProc, this);
    }
    return true;
}

void ThreadPool::shutDown(bool stop_now)
{
    bool expect = false;
    if (shutdown_.compare_exchange_strong(expect, true, std::memory_order_acq_rel)) {
        shutdown_now_.store(stop_now, std::memory_order_release);
        cv_.notify_all();
    }
}

void ThreadPool::threadProc()
{
    for (;;) {
        TaskSPtr task;
        {
            waiting_threads_.fetch_add(1, std::memory_order_acq_rel);

            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [this] {
                return (shutdown_ || !tasks_.empty());
            });

            waiting_threads_.fetch_sub(1, std::memory_order_acq_rel);

            if (shutdown_.load(std::memory_order_acquire) && tasks_.empty()) {
                break;
            }

            if (shutdown_now_.load(std::memory_order_acquire)) {
                break;
            }

            task = std::move(tasks_.front());
            tasks_.pop();
        }
        task->run();
    }
}