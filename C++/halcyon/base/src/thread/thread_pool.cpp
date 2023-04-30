#include "base/thread/thread_pool.h"

#include <list>
#include <mutex>
#include <queue>
#include <thread>
#include <cassert>

BASE_BEGIN_NAMESPACE

/// 线程池(调整任务类型为 ThreadTask)
class ThreadPoolImpl : noncopyable
{
public:
    /**
     * @brief   构造函数
     */
    explicit ThreadPoolImpl() noexcept;

    /**
     * @brief   析构函数
     */
    ~ThreadPoolImpl();

    /**
     * @brief       启动线程池
     * @param[in]   线程数量
     * @return      是否启动成功
     */
    bool start(int32_t num_threads);

    /**
     * @brief   获取正在处于等待状态的线程个数
     */
    size_t getWaitingThreadNum() const
    {
        return waiting_threads_.load(std::memory_order_acquire);
    }

    /**
     * @brief   获取线程池中当前线程的总个数
     */
    size_t getTotalThreadNum() const
    {
        return threads_.size();
    }

private:
    friend class ThreadPool;

private:
    /**
     * @brief   添加任务至队列中
     */
    TaskSPtr addTask(TaskSPtr task)
    {
        if (shutdown_.load(std::memory_order_acquire)) {
            // 处于关闭状态
            return nullptr;
        }

        {
            // 添加任务
            std::lock_guard<std::mutex> lock(mutex_);
            tasks_.emplace(task);
        }
        cv_.notify_one();
        return task;
    }

    /**
     * @brief       停止线程池
     * @param[in]   是否立即结束（不在运行剩余任务）
     */
    void shutDown(bool stop_now);

    /**
     * @brief   线程函数
     */
    void threadProc();

private:
    //! 所有的任务线程
    std::list<std::thread> threads_;

    // 这里并没有用 BlockingQueue, 是因为其多线程退出不好处理
    std::mutex mutex_;
    //! 任务队列
    std::queue<TaskSPtr> tasks_;
    std::condition_variable cv_;

    //! 处于等待状态的线程
    std::atomic<size_t> waiting_threads_{ 0 };

    //! 中止线程池(等待剩余任务运行完成)
    std::atomic<bool> shutdown_{ false };
    //! 中止线程池(放弃任务的运行)
    std::atomic<bool> shutdown_now_{ false };
    //! 是否启动
    bool started_{ false };
};

BASE_END_NAMESPACE

using namespace halcyon::base;

ThreadPoolImpl::ThreadPoolImpl() noexcept
{}

ThreadPoolImpl::~ThreadPoolImpl()
{
    shutDown(false);
    // 确保线程退出
    for (auto& each : threads_) {
        each.join();
    }
}

bool ThreadPoolImpl::start(int32_t num_threads)
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
        threads_.emplace_back(&ThreadPoolImpl::threadProc, this);
    }
    return true;
}

void ThreadPoolImpl::shutDown(bool stop_now)
{
    bool expect = false;
    if (shutdown_.compare_exchange_strong(expect, true, std::memory_order_acq_rel)) {
        shutdown_now_.store(stop_now, std::memory_order_release);
        cv_.notify_all();
    }
}

void ThreadPoolImpl::threadProc()
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

ThreadPool::ThreadPool() noexcept
    : impl_(new ThreadPoolImpl)
{}

ThreadPool::~ThreadPool()
{
    delete impl_;
}

bool ThreadPool::start(int32_t num_threads)
{
    return impl_->start(num_threads);
}

size_t ThreadPool::getWaitingThreadNum() const
{
    return impl_->getWaitingThreadNum();
}

size_t ThreadPool::getTotalThreadNum() const
{
    return impl_->getTotalThreadNum();
}

void ThreadPool::shutDown()
{
    impl_->shutDown(false);
}

void ThreadPool::shutDownNow()
{
    impl_->shutDown(true);
}

TaskSPtr ThreadPool::addTask(TaskSPtr task)
{
    return impl_->addTask(task);
}
